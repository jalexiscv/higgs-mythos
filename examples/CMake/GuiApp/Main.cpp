#include "MainComponent.h"

//==============================================================================
class GuiAppApplication final : public higgs::HIGGSApplication
{
public:
    //==============================================================================
    GuiAppApplication() {}

    // We inject these as compile definitions from the CMakeLists.txt
    // If you've enabled the higgs header with `higgs_generate_higgs_header(<thisTarget>)`
    // you could `#include <HiggsHeader.h>` and use `ProjectInfo::projectName` etc. instead.
    const higgs::String getApplicationName() override       { return HIGGS_APPLICATION_NAME_STRING; }
    const higgs::String getApplicationVersion() override    { return HIGGS_APPLICATION_VERSION_STRING; }
    bool moreThanOneInstanceAllowed() override             { return true; }

    //==============================================================================
    void initialise (const higgs::String& commandLine) override
    {
        // This method is where you should put your application's initialisation code..
        higgs::ignoreUnused (commandLine);

        mainWindow.reset (new MainWindow (getApplicationName()));
    }

    void shutdown() override
    {
        // Add your application's shutdown code here..

        mainWindow = nullptr; // (deletes our window)
    }

    //==============================================================================
    void systemRequestedQuit() override
    {
        // This is called when the app is being asked to quit: you can ignore this
        // request and let the app carry on running, or call quit() to allow the app to close.
        quit();
    }

    void anotherInstanceStarted (const higgs::String& commandLine) override
    {
        // When another instance of the app is launched while this one is running,
        // this method is invoked, and the commandLine parameter tells you what
        // the other instance's command-line arguments were.
        higgs::ignoreUnused (commandLine);
    }

    //==============================================================================
    /*
        This class implements the desktop window that contains an instance of
        our MainComponent class.
    */
    class MainWindow final : public higgs::DocumentWindow
    {
    public:
        explicit MainWindow (higgs::String name)
            : DocumentWindow (name,
                              higgs::Desktop::getInstance().getDefaultLookAndFeel()
                                                          .findColour (backgroundColourId),
                              allButtons)
        {
            setUsingNativeTitleBar (true);
            setContentOwned (new MainComponent(), true);

           #if HIGGS_IOS || HIGGS_ANDROID
            setFullScreen (true);
           #else
            setResizable (true, true);
            centreWithSize (getWidth(), getHeight());
           #endif

            setVisible (true);
        }

        void closeButtonPressed() override
        {
            // This is called when the user tries to close this window. Here, we'll just
            // ask the app to quit when this happens, but you can change this to do
            // whatever you need.
            getInstance()->systemRequestedQuit();
        }

        /* Note: Be careful if you override any DocumentWindow methods - the base
           class uses a lot of them, so by overriding you might break its functionality.
           It's best to do all your work in your content component instead, but if
           you really have to override any DocumentWindow methods, make sure your
           subclass also calls the superclass's method.
        */

    private:
        HIGGS_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainWindow)
    };

private:
    std::unique_ptr<MainWindow> mainWindow;
};

//==============================================================================
// This macro generates the main() routine that launches the app.
START_HIGGS_APPLICATION (GuiAppApplication)
