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
/** Initialises HIGGS's GUI classes.

    If you're embedding HIGGS into an application that uses its own event-loop rather
    than using the START_HIGGS_APPLICATION macro, call this function before making any
    HIGGS calls, to make sure things are initialised correctly.

    Note that if you're creating a HIGGS DLL for Windows, you may also need to call the
    Process::setCurrentModuleInstanceHandle() method.

    @see shutdownHiggs_GUI()
*/
HIGGS_API void HIGGS_CALLTYPE  initialiseHiggs_GUI();

/** Clears up any static data being used by HIGGS's GUI classes.

    If you're embedding HIGGS into an application that uses its own event-loop rather
    than using the START_HIGGS_APPLICATION macro, call this function in your shutdown
    code to clean up any HIGGS objects that might be lying around.

    @see initialiseHiggs_GUI()
*/
HIGGS_API void HIGGS_CALLTYPE  shutdownHiggs_GUI();


//==============================================================================
/** A utility object that helps you initialise and shutdown HIGGS correctly
    using an RAII pattern.

    When the first instance of this class is created, it calls initialiseHiggs_GUI(),
    and when the last instance is deleted, it calls shutdownHiggs_GUI(), so that you
    can easily be sure that as long as at least one instance of the class exists, the
    library will be initialised.

    This class is particularly handy to use at the beginning of a console app's
    main() function, because it'll take care of shutting down whenever you return
    from the main() call.

    Be careful with your threading though - to be safe, you should always make sure
    that these objects are created and deleted on the message thread.

    @tags{Events}
*/
class HIGGS_API  ScopedHiggsInitialiser_GUI  final
{
public:
    /** The constructor simply calls initialiseHiggs_GUI(). */
    ScopedHiggsInitialiser_GUI();

    /** The destructor simply calls shutdownHiggs_GUI(). */
    ~ScopedHiggsInitialiser_GUI();

    HIGGS_DECLARE_NON_COPYABLE (ScopedHiggsInitialiser_GUI)
    HIGGS_DECLARE_NON_MOVEABLE (ScopedHiggsInitialiser_GUI)
};


//==============================================================================
/**
    To start a HIGGS app, use this macro: START_HIGGS_APPLICATION (AppSubClass) where
    AppSubClass is the name of a class derived from HIGGSApplication or HIGGSApplicationBase.

    See the HIGGSApplication and HIGGSApplicationBase class documentation for more details.
*/
#if DOXYGEN
 #define START_HIGGS_APPLICATION(AppClass)
#else
 #if HIGGS_WINDOWS && ! defined (_CONSOLE)
  #define HIGGS_MAIN_FUNCTION                                                        \
      HIGGS_BEGIN_IGNORE_WARNINGS_MSVC (28251)                                       \
      int __stdcall WinMain (struct HINSTANCE__*, struct HINSTANCE__*, char*, int)  \
      HIGGS_END_IGNORE_WARNINGS_MSVC
  #define HIGGS_MAIN_FUNCTION_ARGS
 #else
  #define HIGGS_MAIN_FUNCTION       int main (int argc, char* argv[])
  #define HIGGS_MAIN_FUNCTION_ARGS  argc, (const char**) argv
 #endif

 #if HIGGS_IOS

  #define HIGGS_CREATE_APPLICATION_DEFINE(AppClass) \
    HIGGS_BEGIN_IGNORE_WARNINGS_GCC_LIKE ("-Wmissing-prototypes") \
    higgs::HIGGSApplicationBase* higgs_CreateApplication() { return new AppClass(); } \
    void* higgs_GetIOSCustomDelegateClass()              { return nullptr; } \
    HIGGS_END_IGNORE_WARNINGS_GCC_LIKE

  #define HIGGS_CREATE_APPLICATION_DEFINE_CUSTOM_DELEGATE(AppClass, DelegateClass) \
    HIGGS_BEGIN_IGNORE_WARNINGS_GCC_LIKE ("-Wmissing-prototypes") \
    higgs::HIGGSApplicationBase* higgs_CreateApplication() { return new AppClass(); } \
    void* higgs_GetIOSCustomDelegateClass()              { return [DelegateClass class]; } \
    HIGGS_END_IGNORE_WARNINGS_GCC_LIKE

  #define HIGGS_MAIN_FUNCTION_DEFINITION \
    HIGGS_MAIN_FUNCTION \
    { \
       higgs::HIGGSApplicationBase::createInstance = &higgs_CreateApplication; \
       higgs::HIGGSApplicationBase::iOSCustomDelegate = higgs_GetIOSCustomDelegateClass(); \
       return higgs::HIGGSApplicationBase::main (HIGGS_MAIN_FUNCTION_ARGS); \
    }

 #elif HIGGS_ANDROID

  #define HIGGS_CREATE_APPLICATION_DEFINE(AppClass) \
    extern "C" higgs::HIGGSApplicationBase* higgs_CreateApplication() { return new AppClass(); }

  #define HIGGS_MAIN_FUNCTION_DEFINITION

 #else

  #define HIGGS_CREATE_APPLICATION_DEFINE(AppClass) \
    higgs::HIGGSApplicationBase* higgs_CreateApplication(); \
    higgs::HIGGSApplicationBase* higgs_CreateApplication() { return new AppClass(); }

  #define HIGGS_MAIN_FUNCTION_DEFINITION \
    HIGGS_MAIN_FUNCTION \
    { \
       higgs::HIGGSApplicationBase::createInstance = &higgs_CreateApplication; \
       return higgs::HIGGSApplicationBase::main (HIGGS_MAIN_FUNCTION_ARGS); \
    }

 #endif

 #if HiggsPlugin_Build_Standalone
  #if HIGGS_USE_CUSTOM_PLUGIN_STANDALONE_APP
    #define START_HIGGS_APPLICATION(AppClass) HIGGS_CREATE_APPLICATION_DEFINE(AppClass)
    #if HIGGS_IOS
     #define START_HIGGS_APPLICATION_WITH_CUSTOM_DELEGATE(AppClass, DelegateClass) HIGGS_CREATE_APPLICATION_DEFINE_CUSTOM_DELEGATE(AppClass, DelegateClass)
    #endif
  #else
   #define START_HIGGS_APPLICATION(AppClass) static_assert(false, "You are trying to use START_HIGGS_APPLICATION in an audio plug-in. Define HIGGS_USE_CUSTOM_PLUGIN_STANDALONE_APP=1 if you want to use a custom standalone target app.");
   #if HIGGS_IOS
    #define START_HIGGS_APPLICATION_WITH_CUSTOM_DELEGATE(AppClass, DelegateClass) static_assert(false, "You are trying to use START_HIGGS_APPLICATION in an audio plug-in. Define HIGGS_USE_CUSTOM_PLUGIN_STANDALONE_APP=1 if you want to use a custom standalone target app.");
   #endif
  #endif
 #else

  #define START_HIGGS_APPLICATION(AppClass) \
     HIGGS_BEGIN_IGNORE_WARNINGS_GCC_LIKE ("-Wmissing-prototypes") \
     HIGGS_CREATE_APPLICATION_DEFINE(AppClass) \
     HIGGS_MAIN_FUNCTION_DEFINITION \
     HIGGS_END_IGNORE_WARNINGS_GCC_LIKE

  #if HIGGS_IOS
    /**
       You can instruct HIGGS to use a custom iOS app delegate class instead of HIGGS's default
       app delegate. For HIGGS to work you must pass all messages to HIGGS's internal app delegate.
       Below is an example of minimal forwarding custom delegate. Note that you are at your own
       risk if you decide to use your own delegate and subtle, hard to debug bugs may occur.

       @interface MyCustomDelegate : NSObject <UIApplicationDelegate> { NSObject<UIApplicationDelegate>* higgsDelegate; } @end

       @implementation MyCustomDelegate

       -(id) init
       {
           self = [super init];
           higgsDelegate = reinterpret_cast<NSObject<UIApplicationDelegate>*> ([[NSClassFromString (@"HiggsAppStartupDelegate") alloc] init]);
           return self;
       }

       -(void) dealloc
       {
           [higgsDelegate release];
           [super dealloc];
       }

       - (void) forwardInvocation: (NSInvocation*) anInvocation
       {
           if (higgsDelegate != nullptr && [higgsDelegate respondsToSelector: [anInvocation selector]])
               [anInvocation invokeWithTarget: higgsDelegate];
           else
               [super forwardInvocation: anInvocation];
       }

       -(BOOL) respondsToSelector: (SEL) aSelector
       {
           if (higgsDelegate != nullptr && [higgsDelegate respondsToSelector: aSelector])
               return YES;

           return [super respondsToSelector: aSelector];
       }
       @end
   */
   #define START_HIGGS_APPLICATION_WITH_CUSTOM_DELEGATE(AppClass, DelegateClass) \
      HIGGS_CREATE_APPLICATION_DEFINE_CUSTOM_DELEGATE(AppClass, DelegateClass) \
      HIGGS_MAIN_FUNCTION_DEFINITION
  #endif
 #endif
#endif

} // namespace higgs
