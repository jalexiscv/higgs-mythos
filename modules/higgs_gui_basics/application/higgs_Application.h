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
    An instance of this class is used to specify initialisation and shutdown
    code for the application.

    Any application that wants to run an event loop must declare a subclass of
    HIGGSApplicationBase or HIGGSApplication, and implement its various pure virtual
    methods.

    It then needs to use the START_HIGGS_APPLICATION macro somewhere in a CPP file
    to declare an instance of this class and generate suitable platform-specific
    boilerplate code to launch the app.

    Note that this class is derived from HIGGSApplicationBase, which contains most
    of the useful methods and functionality. This derived class is here simply as
    a convenient way to also inherit from an ApplicationCommandTarget, and to implement
    default versions of some of the pure virtual base class methods. But you can derive
    your app object directly from HIGGSApplicationBase if you want to, and by doing so
    can avoid having a dependency on the higgs_gui_basics module.

    e.g. @code
        class MyHIGGSApp  : public HIGGSApplication
        {
        public:
            MyHIGGSApp()  {}
            ~MyHIGGSApp() {}

            void initialise (const String& commandLine) override
            {
                myMainWindow.reset (new MyApplicationWindow());
                myMainWindow->setBounds (100, 100, 400, 500);
                myMainWindow->setVisible (true);
            }

            void shutdown() override
            {
                myMainWindow = nullptr;
            }

            const String getApplicationName() override
            {
                return "Super HIGGS-o-matic";
            }

            const String getApplicationVersion() override
            {
                return "1.0";
            }

        private:
            std::unique_ptr<MyApplicationWindow> myMainWindow;
        };

        // this generates boilerplate code to launch our app class:
        START_HIGGS_APPLICATION (MyHIGGSApp)
    @endcode

    @see HIGGSApplicationBase, START_HIGGS_APPLICATION

    @tags{GUI}
*/
class HIGGS_API  HIGGSApplication  : public HIGGSApplicationBase,
                                   public ApplicationCommandTarget
{
public:
    //==============================================================================
    /** Constructs a HIGGS app object.

        If subclasses implement a constructor or destructor, they shouldn't call any
        HIGGS code in there - put your startup/shutdown code in initialise() and
        shutdown() instead.
    */
    HIGGSApplication();

    /** Destructor.

        If subclasses implement a constructor or destructor, they shouldn't call any
        HIGGS code in there - put your startup/shutdown code in initialise() and
        shutdown() instead.
    */
    ~HIGGSApplication() override;

    //==============================================================================
    /** Returns the global instance of the application object being run. */
    static HIGGSApplication* HIGGS_CALLTYPE getInstance() noexcept;

    //==============================================================================
   #if DOXYGEN
    /** Returns the application's name. */
    virtual const String getApplicationName() = 0;

    /** Returns the application's version number. */
    virtual const String getApplicationVersion() = 0;
   #endif

    /** Checks whether multiple instances of the app are allowed.

        If your application class returns true for this, more than one instance is
        permitted to run (except on OSX where the OS automatically stops you launching
        a second instance of an app without explicitly starting it from the command-line).

        If it's false, the second instance won't start, but you will still get a
        callback to anotherInstanceStarted() to tell you about this - which
        gives you a chance to react to what the user was trying to do.
    */
    bool moreThanOneInstanceAllowed() override;

    /** Indicates that the user has tried to start up another instance of the app.
        This will get called only if moreThanOneInstanceAllowed() is false.
    */
    void anotherInstanceStarted (const String& commandLine) override;

    /** Called when the operating system is trying to close the application.

        The default implementation of this method is to call quit(), but it may
        be overloaded to ignore the request or do some other special behaviour
        instead. For example, you might want to offer the user the chance to save
        their changes before quitting, and give them the chance to cancel.

        If you want to send a quit signal to your app, this is the correct method
        to call, because it means that requests that come from the system get handled
        in the same way as those from your own application code. So e.g. you'd
        call this method from a "quit" item on a menu bar.
    */
    void systemRequestedQuit() override;

    /** This method is called when the application is being put into background mode
        by the operating system.
    */
    void suspended() override;

    /** This method is called when the application is being woken from background mode
        by the operating system.
    */
    void resumed() override;

    /** If any unhandled exceptions make it through to the message dispatch loop, this
        callback will be triggered, in case you want to log them or do some other
        type of error-handling.

        If the type of exception is derived from the std::exception class, the pointer
        passed-in will be valid. If the exception is of unknown type, this pointer
        will be null.
    */
    void unhandledException (const std::exception* e,
                             const String& sourceFilename,
                             int lineNumber) override;

    //==============================================================================
    /** @internal */
    ApplicationCommandTarget* getNextCommandTarget() override;
    /** @internal */
    void getCommandInfo (CommandID, ApplicationCommandInfo&) override;
    /** @internal */
    void getAllCommands (Array<CommandID>&) override;
    /** @internal */
    bool perform (const InvocationInfo&) override;

private:
    bool initialiseApp() override;

    HIGGS_DECLARE_NON_COPYABLE (HIGGSApplication)
};

} // namespace higgs
