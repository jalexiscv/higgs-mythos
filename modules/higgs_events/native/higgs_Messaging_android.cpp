/*
  ==============================================================================

   This file is part of the Higgs Mythos framework.
   Copyright (c) Raw Material Software Limited

   Higgs Mythos is free software: you can redistribute it and/or modify
   it under the terms of the GNU Affero General Public License as published
   by the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   Higgs Mythos is distributed in the hope that it will be useful, but
   WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
   GNU Affero General Public License for more details.

   You should have received a copy of the GNU Affero General Public License
   along with Higgs Mythos. If not, see <https://www.gnu.org/licenses/>.

  ==============================================================================
*/

namespace higgs
{

//==============================================================================
namespace Android
{
    class Runnable : public higgs::AndroidInterfaceImplementer
    {
    public:
        virtual void run() = 0;

    private:
        jobject invoke (jobject proxy, jobject method, jobjectArray args) override
        {
            auto* env = getEnv();
            auto methodName = higgs::higgsString ((jstring) env->CallObjectMethod (method, JavaMethod.getName));

            if (methodName == "run")
            {
                run();
                return nullptr;
            }

            // invoke base class
            return AndroidInterfaceImplementer::invoke (proxy, method, args);
        }
    };

    struct Handler
    {
        Handler() = default;
        ~Handler() { clearSingletonInstance(); }

        HIGGS_DECLARE_SINGLETON_INLINE (Handler, false)

        bool post (jobject runnable)
        {
            return (getEnv()->CallBooleanMethod (nativeHandler.get(), AndroidHandler.post, runnable) != 0);
        }

        GlobalRef nativeHandler { LocalRef<jobject> { getEnv()->NewObject (AndroidHandler, AndroidHandler.constructor) } };
    };
}

//==============================================================================
struct AndroidMessageQueue final : private Android::Runnable
{
    HIGGS_DECLARE_SINGLETON_SINGLETHREADED_INLINE (AndroidMessageQueue, true)

    AndroidMessageQueue() = default;

    ~AndroidMessageQueue() override
    {
        HIGGS_ASSERT_MESSAGE_THREAD
        clearSingletonInstance();
    }

    bool post (MessageManager::MessageBase::Ptr&& message)
    {
        queue.add (std::move (message));

        // this will call us on the message thread
        return handler.post (self.get());
    }

private:

    void run() override
    {
        for (;;)
        {
            MessageManager::MessageBase::Ptr message (queue.removeAndReturn (0));

            if (message == nullptr)
                break;

            message->messageCallback();
        }
    }

    ReferenceCountedArray<MessageManager::MessageBase, CriticalSection> queue;
    Android::Handler handler;

    // the this pointer to this class in Java land
    GlobalRef self { CreateJavaInterface (this, "java/lang/Runnable") };
};

//==============================================================================
void MessageManager::doPlatformSpecificInitialisation() { AndroidMessageQueue::getInstance(); }
void MessageManager::doPlatformSpecificShutdown()       { AndroidMessageQueue::deleteInstance(); }

bool MessageManager::postMessageToSystemQueue (MessageManager::MessageBase* const message)
{
    return AndroidMessageQueue::getInstance()->post (message);
}

//==============================================================================
void MessageManager::broadcastMessage (const String&)
{
}

void MessageManager::runDispatchLoop()
{
}

void MessageManager::stopDispatchLoop()
{
    struct QuitCallback final : public CallbackMessage
    {
        QuitCallback() {}

        void messageCallback() override
        {
            auto* env = getEnv();
            auto activity = getCurrentActivity();

            if (activity != nullptr)
            {
                jmethodID quitMethod = env->GetMethodID (AndroidActivity, "finishAndRemoveTask", "()V");

                if (quitMethod != nullptr)
                {
                    env->CallVoidMethod (activity.get(), quitMethod);
                    return;
                }

                quitMethod = env->GetMethodID (AndroidActivity, "finish", "()V");
                jassert (quitMethod != nullptr);
                env->CallVoidMethod (activity.get(), quitMethod);
            }
            else
            {
                jassertfalse;
            }
        }
    };

    (new QuitCallback())->post();
    quitMessagePosted = true;
}

//==============================================================================
class HiggsAppLifecycle final : public ActivityLifecycleCallbacks
{
public:
    using CreateApp = higgs::HIGGSApplicationBase* (*)();

    explicit HiggsAppLifecycle (CreateApp initSymbolAddr)
        : createApplicationSymbol (initSymbolAddr)
    {
    }

    void onActivityCreated (jobject, jobject) override
    {
        checkCreated();
    }

    void onActivityDestroyed (jobject activity) override
    {
        auto* env = getEnv();

        // if the main activity is being destroyed, only then tear-down HIGGS
        if (env->IsSameObject (getMainActivity().get(), activity) != 0)
        {
            HIGGSApplicationBase::appWillTerminateByForce();
            JNIClassBase::releaseAllClasses (env);

            LocalRef<jclass> systemClass { env->FindClass ("java/lang/System") };
            jmethodID exitMethod = env->GetStaticMethodID (systemClass.get(), "exit", "(I)V");
            env->CallStaticVoidMethod (systemClass, exitMethod, 0);
        }
    }

    void onActivityStarted (jobject) override
    {
        checkCreated();
    }

    void onActivityPaused (jobject) override
    {
        if (auto* app = HIGGSApplicationBase::getInstance())
            app->suspended();
    }

    void onActivityResumed (jobject) override
    {
        checkInitialised();

        if (auto* app = HIGGSApplicationBase::getInstance())
            app->resumed();
    }

    static HiggsAppLifecycle& getInstance (CreateApp initSymbolAddr)
    {
        static HiggsAppLifecycle higgsAppLifecycle (initSymbolAddr);
        return higgsAppLifecycle;
    }

private:
    void checkCreated()
    {
        if (HIGGSApplicationBase::getInstance() == nullptr)
        {
            DBG (SystemStats::getHIGGSVersion());

            HIGGSApplicationBase::createInstance = createApplicationSymbol;

            initialiser.emplace();

            if (! HIGGSApplicationBase::createInstance())
                jassertfalse; // you must supply an application object for an android app!

            jassert (MessageManager::getInstance()->isThisTheMessageThread());
        }
    }

    void checkInitialised()
    {
        checkCreated();

        if (! hasBeenInitialised)
        {
            if (auto* app = HIGGSApplicationBase::getInstance())
            {
                hasBeenInitialised = app->initialiseApp();

                if (! hasBeenInitialised)
                    exit (app->shutdownApp());
            }
        }
    }

    std::optional<ScopedHiggsInitialiser_GUI> initialiser;
    GlobalRef myself;
    CreateApp createApplicationSymbol{};
    bool hasBeenInitialised = false;
    ActivityLifecycleCallbackForwarder forwarder { GlobalRef { getAppContext() }, this };
};

//==============================================================================
File higgs_getExecutableFile();

void higgs_higgsEventsAndroidStartApp();
void higgs_higgsEventsAndroidStartApp()
{
    auto dllPath = higgs_getExecutableFile().getFullPathName();
    auto addr = reinterpret_cast<higgs::HIGGSApplicationBase* (*)()> (DynamicLibrary (dllPath)
                                                                     .getFunction ("higgs_CreateApplication"));

    if (addr != nullptr)
        HiggsAppLifecycle::getInstance (addr);
}

} // namespace higgs
