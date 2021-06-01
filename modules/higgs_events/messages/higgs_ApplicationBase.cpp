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

HIGGSApplicationBase::CreateInstanceFunction HIGGSApplicationBase::createInstance = nullptr;
HIGGSApplicationBase* HIGGSApplicationBase::appInstance = nullptr;

#if HIGGS_IOS
void* HIGGSApplicationBase::iOSCustomDelegate = nullptr;
#endif

void HIGGSApplicationBase::setApplicationReturnValue (const int newReturnValue) noexcept
{
    appReturnValue = newReturnValue;
}

// This is called on the Mac and iOS where the OS doesn't allow the stack to unwind on shutdown.
void HIGGSApplicationBase::appWillTerminateByForce()
{
    HIGGS_AUTORELEASEPOOL
    {
        {
            const std::unique_ptr<HIGGSApplicationBase> app (appInstance);

            if (app != nullptr)
                app->shutdownApp();
        }

        MessageManager::deleteInstance();
    }
}

void HIGGSApplicationBase::quit()
{
    MessageManager::getInstance()->stopDispatchLoop();
}

void HIGGSApplicationBase::sendUnhandledException (const std::exception* const e,
                                                  const char* const sourceFile,
                                                  const int lineNumber)
{
    if (auto* app = HIGGSApplicationBase::getInstance())
    {
        // If you hit this assertion then the __FILE__ macro is providing a
        // relative path instead of an absolute path. On Windows this will be
        // a path relative to the build directory rather than the currently
        // running application. To fix this you must compile with the /FC flag.
        jassert (File::isAbsolutePath (sourceFile));

        app->unhandledException (e, sourceFile, lineNumber);
    }
}

//==============================================================================
#if ! (HIGGS_IOS || HIGGS_ANDROID)
 #define HIGGS_HANDLE_MULTIPLE_INSTANCES 1
#endif

#if HIGGS_HANDLE_MULTIPLE_INSTANCES
struct HIGGSApplicationBase::MultipleInstanceHandler final : public ActionListener
{
    MultipleInstanceHandler (const String& appName)
        : appLock ("higgsAppLock_" + appName)
    {
    }

    bool sendCommandLineToPreexistingInstance()
    {
        if (appLock.enter (0))
            return false;

        if (auto* app = HIGGSApplicationBase::getInstance())
        {
            MessageManager::broadcastMessage (app->getApplicationName() + "/" + app->getCommandLineParameters());
            return true;
        }

        jassertfalse;
        return false;
    }

    void actionListenerCallback (const String& message) override
    {
        if (auto* app = HIGGSApplicationBase::getInstance())
        {
            auto appName = app->getApplicationName();

            if (message.startsWith (appName + "/"))
                app->anotherInstanceStarted (message.substring (appName.length() + 1));
        }
    }

private:
    InterProcessLock appLock;

    HIGGS_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MultipleInstanceHandler)
};

bool HIGGSApplicationBase::sendCommandLineToPreexistingInstance()
{
    jassert (multipleInstanceHandler == nullptr); // this must only be called once!

    multipleInstanceHandler.reset (new MultipleInstanceHandler (getApplicationName()));
    return multipleInstanceHandler->sendCommandLineToPreexistingInstance();
}

#else
struct HIGGSApplicationBase::MultipleInstanceHandler {};
#endif

HIGGSApplicationBase::HIGGSApplicationBase()
{
    jassert (isStandaloneApp() && appInstance == nullptr);
    appInstance = this;
}

HIGGSApplicationBase::~HIGGSApplicationBase()
{
    jassert (appInstance == this);
    appInstance = nullptr;
}

//==============================================================================
#if HIGGS_ANDROID

StringArray HIGGSApplicationBase::getCommandLineParameterArray() { return {}; }
String HIGGSApplicationBase::getCommandLineParameters()          { return {}; }

#else

#if HIGGS_WINDOWS && ! defined (_CONSOLE)

String HIGGS_CALLTYPE HIGGSApplicationBase::getCommandLineParameters()
{
    return CharacterFunctions::findEndOfToken (CharPointer_UTF16 (GetCommandLineW()),
                                               CharPointer_UTF16 (L" "),
                                               CharPointer_UTF16 (L"\"")).findEndOfWhitespace();
}

StringArray HIGGS_CALLTYPE HIGGSApplicationBase::getCommandLineParameterArray()
{
    StringArray s;
    int argc = 0;

    if (auto argv = CommandLineToArgvW (GetCommandLineW(), &argc))
    {
        s = StringArray (argv + 1, argc - 1);
        LocalFree (argv);
    }

    return s;
}

#else

#if HIGGS_IOS && HIGGS_MODULE_AVAILABLE_higgs_gui_basics
 extern int higgs_iOSMain (int argc, const char* argv[], void* classPtr);
#endif

#if HIGGS_MAC
 extern void initialiseNSApplication();
#endif

#if (HIGGS_LINUX || HIGGS_BSD) && HIGGS_MODULE_AVAILABLE_higgs_gui_extra && (! defined (HIGGS_WEB_BROWSER) || HIGGS_WEB_BROWSER)
 extern "C" int higgs_gtkWebkitMain (int argc, const char* const* argv);
#endif

#if HIGGS_WINDOWS
 const char* const* higgs_argv = nullptr;
 int higgs_argc = 0;
#else
 extern const char* const* higgs_argv;  // declared in higgs_core
 extern int higgs_argc;
#endif

String HIGGSApplicationBase::getCommandLineParameters()
{
    String argString;

    for (const auto& arg : getCommandLineParameterArray())
    {
        const auto withQuotes = arg.containsChar (' ') && ! arg.isQuotedString()
                              ? arg.quoted ('"')
                              : arg;
        argString << withQuotes << ' ';
    }

    return argString.trim();
}

StringArray HIGGSApplicationBase::getCommandLineParameterArray()
{
    StringArray result;

    for (int i = 1; i < higgs_argc; ++i)
        result.add (CharPointer_UTF8 (higgs_argv[i]));

    return result;
}

int HIGGSApplicationBase::main (int argc, const char* argv[])
{
    HIGGS_AUTORELEASEPOOL
    {
        higgs_argc = argc;
        higgs_argv = argv;

       #if HIGGS_MAC
        initialiseNSApplication();
       #endif

       #if (HIGGS_LINUX || HIGGS_BSD) && HIGGS_MODULE_AVAILABLE_higgs_gui_extra && (! defined (HIGGS_WEB_BROWSER) || HIGGS_WEB_BROWSER)
        if (argc >= 2 && String (argv[1]) == "--higgs-gtkwebkitfork-child")
            return higgs_gtkWebkitMain (argc, argv);
       #endif

       #if HIGGS_IOS && HIGGS_MODULE_AVAILABLE_higgs_gui_basics
        return higgs_iOSMain (argc, argv, iOSCustomDelegate);
       #else

        return HIGGSApplicationBase::main();
       #endif
    }
}

#endif

//==============================================================================
int HIGGSApplicationBase::main()
{
    ScopedHiggsInitialiser_GUI libraryInitialiser;
    jassert (createInstance != nullptr);

    const std::unique_ptr<HIGGSApplicationBase> app (createInstance());
    jassert (app != nullptr);

    if (! app->initialiseApp())
        return app->shutdownApp();

    HIGGS_TRY
    {
        // loop until a quit message is received
        MessageManager::getInstance()->runDispatchLoop();
    }
    HIGGS_CATCH_EXCEPTION

    return app->shutdownApp();
}

#endif

//==============================================================================
bool HIGGSApplicationBase::initialiseApp()
{
   #if HIGGS_HANDLE_MULTIPLE_INSTANCES
    if ((! moreThanOneInstanceAllowed()) && sendCommandLineToPreexistingInstance())
    {
        DBG ("Another instance is running - quitting...");
        return false;
    }
   #endif

   #if HIGGS_WINDOWS && (! defined (_CONSOLE))
    if (isStandaloneApp() && AttachConsole (ATTACH_PARENT_PROCESS) != 0)
    {
        // If we've launched a GUI app from cmd.exe or PowerShell, we need this to enable printf etc.
        // However, only reassign stdout, stderr, stdin if they have not been already opened by
        // a redirect or similar.
        FILE* ignore;

        if (_fileno (stdout) < 0) freopen_s (&ignore, "CONOUT$", "w", stdout);
        if (_fileno (stderr) < 0) freopen_s (&ignore, "CONOUT$", "w", stderr);
        if (_fileno (stdin)  < 0) freopen_s (&ignore, "CONIN$",  "r", stdin);
    }
   #endif

    // let the app do its setting-up
    initialise (getCommandLineParameters());

    stillInitialising = false;

    if (MessageManager::getInstance()->hasStopMessageBeenSent())
        return false;

   #if HIGGS_HANDLE_MULTIPLE_INSTANCES
    if (auto* mih = multipleInstanceHandler.get())
        MessageManager::getInstance()->registerBroadcastListener (mih);
   #endif

    return true;
}

int HIGGSApplicationBase::shutdownApp()
{
    jassert (HIGGSApplicationBase::getInstance() == this);

   #if HIGGS_HANDLE_MULTIPLE_INSTANCES
    if (auto* mih = multipleInstanceHandler.get())
        MessageManager::getInstance()->deregisterBroadcastListener (mih);
   #endif

    HIGGS_TRY
    {
        // give the app a chance to clean up
        shutdown();
    }
    HIGGS_CATCH_EXCEPTION

    multipleInstanceHandler.reset();
    return getApplicationReturnValue();
}

} // namespace higgs
