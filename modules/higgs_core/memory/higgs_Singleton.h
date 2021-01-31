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
/**
    Used by the HIGGS_DECLARE_SINGLETON macros to manage a static pointer
    to a singleton instance.

    You generally won't use this directly, but see the macros HIGGS_DECLARE_SINGLETON,
    HIGGS_DECLARE_SINGLETON_SINGLETHREADED, HIGGS_DECLARE_SINGLETON_SINGLETHREADED_MINIMAL,
    and HIGGS_IMPLEMENT_SINGLETON for how it is intended to be used.

    @tags{Core}
*/
template <typename Type, typename MutexType, bool onlyCreateOncePerRun>
struct SingletonHolder  : private MutexType // (inherited so we can use the empty-base-class optimisation)
{
    SingletonHolder() = default;

    ~SingletonHolder()
    {
        /* The static singleton holder is being deleted before the object that it holds
           has been deleted. This could mean that you've forgotten to call clearSingletonInstance()
           in the class's destructor, or have failed to delete it before your app shuts down.
           If you're having trouble cleaning up your singletons, perhaps consider using the
           SharedResourcePointer class instead.
        */
        jassert (instance.load() == nullptr);
    }

    /** Returns the current instance, or creates a new instance if there isn't one. */
    Type* get()
    {
        if (auto* ptr = instance.load())
            return ptr;

        typename MutexType::ScopedLockType sl (*this);

        if (auto* ptr = instance.load())
            return ptr;

        auto once = onlyCreateOncePerRun; // (local copy avoids VS compiler warning about this being constant)

        if (once)
        {
            static bool createdOnceAlready = false;

            if (createdOnceAlready)
            {
                // This means that the doNotRecreateAfterDeletion flag was set
                // and you tried to create the singleton more than once.
                jassertfalse;
                return nullptr;
            }

            createdOnceAlready = true;
        }

        static bool alreadyInside = false;

        if (alreadyInside)
        {
            // This means that your object's constructor has done something which has
            // ended up causing a recursive loop of singleton creation.
            jassertfalse;
            return nullptr;
        }

        const ScopedValueSetter<bool> scope (alreadyInside, true);
        return getWithoutChecking();
    }

    /** Returns the current instance, or creates a new instance if there isn't one, but doesn't do
        any locking, or checking for recursion or error conditions.
    */
    Type* getWithoutChecking()
    {
        if (auto* p = instance.load())
            return p;

        auto* newObject = new Type(); // (create into a local so that instance is still null during construction)
        instance.store (newObject);
        return newObject;
    }

    /** Deletes and resets the current instance, if there is one. */
    void deleteInstance()
    {
        typename MutexType::ScopedLockType sl (*this);
        delete instance.exchange (nullptr);
    }

    /** Called by the class's destructor to clear the pointer if it is currently set to the given object. */
    void clear (Type* expectedObject) noexcept
    {
        instance.compare_exchange_strong (expectedObject, nullptr);
    }

    // This must be atomic, otherwise a late call to get() may attempt to read instance while it is
    // being modified by the very first call to get().
    std::atomic<Type*> instance { nullptr };
};

/** @cond */
#define HIGGS_PRIVATE_DECLARE_SINGLETON(Classname, mutex, doNotRecreate, inlineToken, getter) \
    static inlineToken higgs::SingletonHolder<Classname, mutex, doNotRecreate> singletonHolder; \
    friend higgs::SingletonHolder<Classname, mutex, doNotRecreate>; \
    static Classname* HIGGS_CALLTYPE getInstance()                           { return singletonHolder.getter(); } \
    static Classname* HIGGS_CALLTYPE getInstanceWithoutCreating() noexcept   { return singletonHolder.instance; } \
    static void HIGGS_CALLTYPE deleteInstance() noexcept                     { singletonHolder.deleteInstance(); } \
    void clearSingletonInstance() noexcept                                  { singletonHolder.clear (this); }
/** @endcond */

//==============================================================================
/**
    Macro to generate the appropriate methods and boilerplate for a singleton class.

    To use this, add the line HIGGS_DECLARE_SINGLETON (MyClass, doNotRecreateAfterDeletion)
    to the class's definition.

    Then put a macro HIGGS_IMPLEMENT_SINGLETON (MyClass) along with the class's
    implementation code.

    It's also a very good idea to also add the call clearSingletonInstance() in your class's
    destructor, in case it is deleted by other means than deleteInstance()

    Clients can then call the static method MyClass::getInstance() to get a pointer
    to the singleton, or MyClass::getInstanceWithoutCreating() which will return nullptr if
    no instance currently exists.

    e.g. @code

        struct MySingleton
        {
            MySingleton() {}

            ~MySingleton()
            {
                // this ensures that no dangling pointers are left when the
                // singleton is deleted.
                clearSingletonInstance();
            }

            HIGGS_DECLARE_SINGLETON (MySingleton, false)
        };

        // ...and this goes in a suitable .cpp file:
        HIGGS_IMPLEMENT_SINGLETON (MySingleton)


        // example of usage:
        auto* m = MySingleton::getInstance(); // creates the singleton if there isn't already one.

        ...

        MySingleton::deleteInstance(); // safely deletes the singleton (if it's been created)

    @endcode

    If doNotRecreateAfterDeletion = true, it won't allow the object to be created more
    than once during the process's lifetime - i.e. after you've created and deleted the
    object, getInstance() will refuse to create another one. This can be useful to stop
    objects being accidentally re-created during your app's shutdown code.

    If you know that your object will only be created and deleted by a single thread, you
    can use the slightly more efficient HIGGS_DECLARE_SINGLETON_SINGLETHREADED macro instead
    of this one.

    @see HIGGS_IMPLEMENT_SINGLETON, HIGGS_DECLARE_SINGLETON_SINGLETHREADED
*/
#define HIGGS_DECLARE_SINGLETON(Classname, doNotRecreateAfterDeletion) \
    HIGGS_PRIVATE_DECLARE_SINGLETON (Classname, higgs::CriticalSection, doNotRecreateAfterDeletion, , get)

/**
    The same as HIGGS_DECLARE_SINGLETON, but does not require a matching
    HIGGS_IMPLEMENT_SINGLETON definition.
*/
#define HIGGS_DECLARE_SINGLETON_INLINE(Classname, doNotRecreateAfterDeletion) \
    HIGGS_PRIVATE_DECLARE_SINGLETON (Classname, higgs::CriticalSection, doNotRecreateAfterDeletion, inline, get)

//==============================================================================
/** This is a counterpart to the HIGGS_DECLARE_SINGLETON macros.

    After adding the HIGGS_DECLARE_SINGLETON to the class definition, this macro has
    to be used in the cpp file.

    This macro is not required for singletons declared with the INLINE macros, specifically
    HIGGS_DECLARE_SINGLETON_INLINE, HIGGS_DECLARE_SINGLETON_SINGLETHREADED_INLINE, and
    HIGGS_DECLARE_SINGLETON_SINGLETHREADED_MINIMAL_INLINE.
*/
#define HIGGS_IMPLEMENT_SINGLETON(Classname) \
    decltype (Classname::singletonHolder) Classname::singletonHolder;

//==============================================================================
/**
    Macro to declare member variables and methods for a singleton class.

    This is exactly the same as HIGGS_DECLARE_SINGLETON, but doesn't use a critical
    section to make access to it thread-safe. If you know that your object will
    only ever be created or deleted by a single thread, then this is a
    more efficient version to use.

    If doNotRecreateAfterDeletion = true, it won't allow the object to be created more
    than once during the process's lifetime - i.e. after you've created and deleted the
    object, getInstance() will refuse to create another one. This can be useful to stop
    objects being accidentally re-created during your app's shutdown code.

    See the documentation for HIGGS_DECLARE_SINGLETON for more information about
    how to use it. Just like HIGGS_DECLARE_SINGLETON you need to also have a
    corresponding HIGGS_IMPLEMENT_SINGLETON statement somewhere in your code.

    @see HIGGS_IMPLEMENT_SINGLETON, HIGGS_DECLARE_SINGLETON, HIGGS_DECLARE_SINGLETON_SINGLETHREADED_MINIMAL
*/
#define HIGGS_DECLARE_SINGLETON_SINGLETHREADED(Classname, doNotRecreateAfterDeletion) \
    HIGGS_PRIVATE_DECLARE_SINGLETON (Classname, higgs::DummyCriticalSection, doNotRecreateAfterDeletion, , get)

/**
    The same as HIGGS_DECLARE_SINGLETON_SINGLETHREADED, but does not require a matching
    HIGGS_IMPLEMENT_SINGLETON definition.
*/
#define HIGGS_DECLARE_SINGLETON_SINGLETHREADED_INLINE(Classname, doNotRecreateAfterDeletion) \
    HIGGS_PRIVATE_DECLARE_SINGLETON (Classname, higgs::DummyCriticalSection, doNotRecreateAfterDeletion, inline, get)

//==============================================================================
/**
    Macro to declare member variables and methods for a singleton class.

    This is like HIGGS_DECLARE_SINGLETON_SINGLETHREADED, but doesn't do any checking
    for recursion or repeated instantiation. It's intended for use as a lightweight
    version of a singleton, where you're using it in very straightforward
    circumstances and don't need the extra checking.

    See the documentation for HIGGS_DECLARE_SINGLETON for more information about
    how to use it. Just like HIGGS_DECLARE_SINGLETON you need to also have a
    corresponding HIGGS_IMPLEMENT_SINGLETON statement somewhere in your code.

    @see HIGGS_IMPLEMENT_SINGLETON, HIGGS_DECLARE_SINGLETON
*/
#define HIGGS_DECLARE_SINGLETON_SINGLETHREADED_MINIMAL(Classname) \
    HIGGS_PRIVATE_DECLARE_SINGLETON (Classname, higgs::DummyCriticalSection, false, , getWithoutChecking)

/**
    The same as HIGGS_DECLARE_SINGLETON_SINGLETHREADED_MINIMAL, but does not require a matching
    HIGGS_IMPLEMENT_SINGLETON definition.
*/
#define HIGGS_DECLARE_SINGLETON_SINGLETHREADED_MINIMAL_INLINE(Classname) \
    HIGGS_PRIVATE_DECLARE_SINGLETON (Classname, higgs::DummyCriticalSection, false, inline, getWithoutChecking)

//==============================================================================
/** @cond */
// These are ancient macros, and have now been updated with new names to match the HIGGS style guide,
// so please update your code to use the newer versions!
#define higgs_DeclareSingleton(Classname, doNotRecreate)                HIGGS_DECLARE_SINGLETON(Classname, doNotRecreate)
#define higgs_DeclareSingleton_SingleThreaded(Classname, doNotRecreate) HIGGS_DECLARE_SINGLETON_SINGLETHREADED(Classname, doNotRecreate)
#define higgs_DeclareSingleton_SingleThreaded_Minimal(Classname)        HIGGS_DECLARE_SINGLETON_SINGLETHREADED_MINIMAL(Classname)
#define higgs_ImplementSingleton(Classname)                             HIGGS_IMPLEMENT_SINGLETON(Classname)
#define higgs_ImplementSingleton_SingleThreaded(Classname)              HIGGS_IMPLEMENT_SINGLETON(Classname)
/** @endcond */

} // namespace higgs
