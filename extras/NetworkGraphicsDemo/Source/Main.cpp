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

#include <HiggsHeader.h>

namespace
{
    String getBroadcastIPAddress()
    {
        return IPAddress::getLocalAddress().toString().upToLastOccurrenceOf (".", false, false) + ".255";
    }

    static const int masterPortNumber = 9001;  // the UDP port the master sends on / the clients receive.
    static const int clientPortNumber = 9002;  // the UDP port the clients send on / the master receives.

    static const String canvasStateOSCAddress = "/higgs/nfd/canvasState";
    static const String newClientOSCAddress   = "/higgs/nfd/newClient";
    static const String userInputOSCAddress   = "/higgs/nfd/userInput";
}

#include "SharedCanvas.h"
#include "ClientComponent.h"
#include "Demos.h"
#include "MasterComponent.h"


//==============================================================================
class NetworkGraphicsDemoApplication final : public HIGGSApplication
{
public:
    NetworkGraphicsDemoApplication()  : properties (getPropertyFileOptions())
    {}

    const String getApplicationName() override           { return ProjectInfo::projectName; }
    const String getApplicationVersion() override        { return ProjectInfo::versionString; }
    bool moreThanOneInstanceAllowed() override           { return true; }
    void anotherInstanceStarted (const String&) override {}

    //==============================================================================
    void initialise (const String& commandLine) override
    {
       #if ! HIGGS_IOS && ! HIGGS_ANDROID
        // Run as the master if we have a command-line flag "master" or if the exe itself
        // has been renamed to include the word "master"..
        bool isMaster = commandLine.containsIgnoreCase ("master")
                          || File::getSpecialLocation (File::currentApplicationFile)
                                .getFileName().containsIgnoreCase ("master");

        if (isMaster)
            mainWindows.add (new MainWindow (properties));
       #endif

        mainWindows.add (new MainWindow (properties, 0));

        Desktop::getInstance().setScreenSaverEnabled (false);
    }

    void shutdown() override
    {
        mainWindows.clear();
        properties.saveIfNeeded();
    }

    void systemRequestedQuit() override
    {
        quit();
    }

    //==============================================================================
    struct MainWindow final : public DocumentWindow
    {
        explicit MainWindow (PropertiesFile& props)
            : DocumentWindow ("HIGGS Networked Graphics Demo - Master", Colours::white, DocumentWindow::allButtons)
        {
            setUsingNativeTitleBar (true);
            setContentOwned (new MasterContentComponent (props), true);
            setBounds (100, 50, getWidth(), getHeight());
            setResizable (true, false);
            setVisible (true);

            glContext.attachTo (*this);
        }

        MainWindow (PropertiesFile& props, int windowIndex)
            : DocumentWindow ("HIGGS Networked Graphics Demo", Colours::black, DocumentWindow::allButtons)
        {
            setUsingNativeTitleBar (true);
            setContentOwned (new ClientCanvasComponent (props, windowIndex), true);
            setBounds (500, 100, getWidth(), getHeight());
            setResizable (true, false);
            setVisible (true);

           #if ! HIGGS_IOS
            glContext.attachTo (*this);
           #endif

           #if HIGGS_IOS || HIGGS_ANDROID
            setFullScreen (true);
           #endif
        }

        ~MainWindow() override
        {
            glContext.detach();
        }

        void closeButtonPressed() override
        {
            HIGGSApplication::getInstance()->systemRequestedQuit();
        }

        OpenGLContext glContext;

        HIGGS_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainWindow)
    };

    static PropertiesFile::Options getPropertyFileOptions()
    {
        PropertiesFile::Options o;
        o.applicationName = "HIGGS Network Graphics Demo";
        o.filenameSuffix = ".settings";
        o.folderName = "HIGGS Network Graphics Demo";
        o.osxLibrarySubFolder = "Application Support/HIGGS Network Graphics Demo";
        o.millisecondsBeforeSaving = 2000;
        return o;
    }

    PropertiesFile properties;
    OwnedArray<MainWindow> mainWindows;
};


//==============================================================================
// This macro generates the main() routine that launches the app.
START_HIGGS_APPLICATION (NetworkGraphicsDemoApplication)
