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

#pragma once

#include "mythos_MainWindow.h"
#include "../Project/Modules/mythos_Modules.h"
#include "mythos_AutoUpdater.h"
#include "../CodeEditor/mythos_SourceCodeEditor.h"
#include "../Utility/UI/mythos_MythosStudioLookAndFeel.h"

//==============================================================================
class MythosStudioApplication final : public HIGGSApplication,
                                  private AsyncUpdater
{
public:
    MythosStudioApplication() = default;

    static MythosStudioApplication& getApp();
    static ApplicationCommandManager& getCommandManager();

    //==============================================================================
    void initialise (const String& commandLine) override;
    void shutdown() override;
    void systemRequestedQuit() override;
    void deleteLogger();

    const String getApplicationName() override       { return "MythosStudio"; }
    const String getApplicationVersion() override    { return ProjectInfo::versionString; }

    String getVersionDescription() const;
    bool moreThanOneInstanceAllowed() override       { return true; } // this is handled manually in initialise()

    void anotherInstanceStarted (const String& commandLine) override;

    //==============================================================================
    MenuBarModel* getMenuModel();

    void getAllCommands (Array<CommandID>&) override;
    void getCommandInfo (CommandID commandID, ApplicationCommandInfo&) override;
    bool perform (const InvocationInfo&) override;

    //==============================================================================
    void openFile (const File&, std::function<void (bool)>);
    void showPathsWindow (bool highlightHIGGSPath = false);
    PropertiesFile::Options getPropertyFileOptionsFor (const String& filename, bool isProjectSettings);
    void selectEditorColourSchemeWithName (const String& schemeName);

    //==============================================================================
    void rescanHIGGSPathModules();
    void rescanUserPathModules();

    AvailableModulesList& getHIGGSPathModulesList()     { return higgsPathModulesList; }
    AvailableModulesList& getUserPathsModulesList()    { return userPathsModulesList; }

    bool isAutomaticVersionCheckingEnabled() const;
    void setAutomaticVersionCheckingEnabled (bool shouldBeEnabled);

    bool shouldPromptUserAboutIncorrectHIGGSPath() const;
    void setShouldPromptUserAboutIncorrectHIGGSPath (bool shouldPrompt);

    static File getHIGGSExamplesDirectoryPathFromGlobal() noexcept;
    static Array<File> getSortedExampleDirectories() noexcept;
    static Array<File> getSortedExampleFilesInDirectory (const File&) noexcept;

    //==============================================================================
    MythosStudioLookAndFeel lookAndFeel;

    std::unique_ptr<StoredSettings> settings;
    std::unique_ptr<Icons> icons;

    struct MainMenuModel;
    std::unique_ptr<MainMenuModel> menuModel;

    MainWindowList mainWindowList;
    OpenDocumentManager openDocumentManager;
    std::unique_ptr<ApplicationCommandManager> commandManager;

    bool isRunningCommandLine = false;

private:
    //==============================================================================
    void handleAsyncUpdate() override;
    void doBasicApplicationSetup();

    void initCommandManager();
    bool initialiseLogger (const char* filePrefix);
    void initialiseWindows (const String& commandLine);

    void createNewProject();
    void createNewProjectFromClipboard();
    void createNewPIP();
    void askUserToOpenFile();
    void saveAllDocuments();
    void closeAllDocuments (OpenDocumentManager::SaveIfNeeded askUserToSave);
    void closeAllMainWindows (std::function<void (bool)>);
    void closeAllMainWindowsAndQuitIfNeeded();
    void clearRecentFiles();

    StringArray getMenuNames();
    PopupMenu createMenu (const String& menuName);
    PopupMenu createFileMenu();
    PopupMenu createEditMenu();
    PopupMenu createViewMenu();
    void createColourSchemeItems (PopupMenu&);
    PopupMenu createWindowMenu();
    PopupMenu createDocumentMenu();
    PopupMenu createToolsMenu();
    PopupMenu createHelpMenu();
    PopupMenu createExtraAppleMenuItems();
    void handleMainMenuCommand (int menuItemID);
    PopupMenu createExamplesPopupMenu() noexcept;

    void findAndLaunchExample (int);

    void checkIfGlobalHIGGSPathHasChanged();
    File tryToFindDemoRunnerExecutable();
    File tryToFindDemoRunnerProject();
    void launchDemoRunner();

    void setColourScheme (int index, bool saveSetting);
    void setEditorColourScheme (int index, bool saveSetting);
    void updateEditorColourSchemeIfNeeded();

    void showUTF8ToolWindow();
    void showSVGPathDataToolWindow();
    void showAboutWindow();
    void showEditorColourSchemeWindow();
    void showPIPCreatorWindow();

    void launchForumBrowser();
    void launchModulesBrowser();
    void launchClassesBrowser();
    void launchTutorialsBrowser();

    //==============================================================================
   #if HIGGS_MAC
    class AppleMenuRebuildListener final : private MenuBarModel::Listener
    {
    public:
        AppleMenuRebuildListener()
        {
            if (auto* model = MythosStudioApplication::getApp().getMenuModel())
                model->addListener (this);
        }

        ~AppleMenuRebuildListener() override
        {
            if (auto* model = MythosStudioApplication::getApp().getMenuModel())
                model->removeListener (this);
        }

    private:
        void menuBarItemsChanged (MenuBarModel*) override  {}

        void menuCommandInvoked (MenuBarModel*,
                                 const ApplicationCommandTarget::InvocationInfo& info) override
        {
            if (info.commandID == CommandIDs::enableNewVersionCheck)
                Timer::callAfterDelay (50, [] { MythosStudioApplication::getApp().rebuildAppleMenu(); });
        }
    };

    void rebuildAppleMenu();

    std::unique_ptr<AppleMenuRebuildListener> appleMenuRebuildListener;
   #endif

    //==============================================================================
    std::unique_ptr<TooltipWindow> tooltipWindow;
    AvailableModulesList higgsPathModulesList, userPathsModulesList;

    std::unique_ptr<Component> utf8Window, svgPathWindow, aboutWindow, pathsWindow,
                               editorColourSchemeWindow, pipCreatorWindow;

    std::unique_ptr<FileLogger> logger;

    int numExamples = 0;
    std::unique_ptr<AlertWindow> demoRunnerAlert;
    bool hasScannedForDemoRunnerExecutable = false, hasScannedForDemoRunnerProject = false;
    File lastHIGGSPath, lastDemoRunnerExectuableFile, lastDemoRunnerProjectFile;

    int selectedColourSchemeIndex = 0, selectedEditorColourSchemeIndex = 0;

    std::unique_ptr<FileChooser> chooser;
    ScopedMessageBox messageBox;

    //==============================================================================
    HIGGS_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MythosStudioApplication)
    HIGGS_DECLARE_WEAK_REFERENCEABLE (MythosStudioApplication)
};
