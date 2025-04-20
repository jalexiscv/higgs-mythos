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

//==============================================================================
struct MythosStudioApplication::MainMenuModel final : public MenuBarModel
{
    MainMenuModel()
    {
        setApplicationCommandManagerToWatch (&getCommandManager());
    }

    StringArray getMenuBarNames() override
    {
        return getApp().getMenuNames();
    }

    PopupMenu getMenuForIndex (int /*topLevelMenuIndex*/, const String& menuName) override
    {
        return getApp().createMenu (menuName);
    }

    void menuItemSelected (int menuItemID, int /*topLevelMenuIndex*/) override
    {
        getApp().handleMainMenuCommand (menuItemID);
    }
};

//==============================================================================
void MythosStudioApplication::initialise (const String& commandLine)
{
    initialiseLogger ("IDE_Log_");
    Logger::writeToLog (SystemStats::getOperatingSystemName());
    Logger::writeToLog ("CPU: " + String (SystemStats::getCpuSpeedInMegahertz())
                          + "MHz  Cores: " + String (SystemStats::getNumCpus())
                          + "  " + String (SystemStats::getMemorySizeInMegabytes()) + "MB");

    isRunningCommandLine = commandLine.isNotEmpty()
                            && ! commandLine.startsWith ("-NSDocumentRevisionsDebugMode");

    settings = std::make_unique<StoredSettings>();

    if (isRunningCommandLine)
    {
        auto appReturnCode = performCommandLine (ArgumentList ("MythosStudio", commandLine));

        if (appReturnCode != commandLineNotPerformed)
        {
            setApplicationReturnValue (appReturnCode);
            quit();
            return;
        }

        isRunningCommandLine = false;
    }

    if (sendCommandLineToPreexistingInstance())
    {
        DBG ("Another instance is running - quitting...");
        quit();
        return;
    }

    doBasicApplicationSetup();

    // do further initialisation in a moment when the message loop has started
    triggerAsyncUpdate();
}

bool MythosStudioApplication::initialiseLogger (const char* filePrefix)
{
    if (logger == nullptr)
    {
       #if HIGGS_LINUX || HIGGS_BSD
        String folder = "~/.config/MythosStudio/Logs";
       #else
        String folder = "com.higgs.mythosstudio";
       #endif

        logger.reset (FileLogger::createDateStampedLogger (folder, filePrefix, ".txt",
                                                           getApplicationName() + " " + getApplicationVersion()
                                                               + "  ---  Build date: " __DATE__));
        Logger::setCurrentLogger (logger.get());
    }

    return logger != nullptr;
}

void MythosStudioApplication::initialiseWindows (const String& commandLine)
{
    const String commandLineWithoutNSDebug (commandLine.replace ("-NSDocumentRevisionsDebugMode YES", StringRef()));

    if (commandLineWithoutNSDebug.trim().isNotEmpty() && ! commandLineWithoutNSDebug.trim().startsWithChar ('-'))
        anotherInstanceStarted (commandLine);
    else if (mainWindowList.windows.size() == 0)
        mainWindowList.reopenLastProjects();

    mainWindowList.createWindowIfNoneAreOpen();
}

void MythosStudioApplication::handleAsyncUpdate()
{
    rescanHIGGSPathModules();
    rescanUserPathModules();

    menuModel.reset (new MainMenuModel());

   #if HIGGS_MAC
    rebuildAppleMenu();
    appleMenuRebuildListener = std::make_unique<AppleMenuRebuildListener>();
   #endif

    settings->appearance.refreshPresetSchemeList();
    setColourScheme (getGlobalProperties().getIntValue ("COLOUR SCHEME"), false);
    setEditorColourScheme (getGlobalProperties().getIntValue ("EDITOR COLOUR SCHEME"), false);
    updateEditorColourSchemeIfNeeded();

    ImageCache::setCacheTimeout (30 * 1000);
    tooltipWindow = std::make_unique<TooltipWindow> (nullptr, 1200);

    if (isAutomaticVersionCheckingEnabled())
        LatestVersionCheckerAndUpdater::getInstance()->checkForNewVersion (true);

    initialiseWindows (getCommandLineParameters());
}

void MythosStudioApplication::doBasicApplicationSetup()
{
    LookAndFeel::setDefaultLookAndFeel (&lookAndFeel);
    initCommandManager();
    icons = std::make_unique<Icons>();
}

static void deleteTemporaryFiles()
{
    auto tempDirectory = File::getSpecialLocation (File::SpecialLocationType::tempDirectory).getChildFile ("PIPs");

    if (tempDirectory.exists())
        tempDirectory.deleteRecursively();
}

void MythosStudioApplication::shutdown()
{
    utf8Window.reset();
    svgPathWindow.reset();
    aboutWindow.reset();
    pathsWindow.reset();
    editorColourSchemeWindow.reset();
    pipCreatorWindow.reset();

    mainWindowList.forceCloseAllWindows();
    openDocumentManager.clear();

   #if HIGGS_MAC
    MenuBarModel::setMacMainMenu (nullptr);
   #endif

    menuModel.reset();
    commandManager.reset();
    settings.reset();

    if (! isRunningCommandLine)
        LookAndFeel::setDefaultLookAndFeel (nullptr);

    // clean up after ourselves and delete any temp project files that may have
    // been created from PIPs
    deleteTemporaryFiles();

    if (! isRunningCommandLine)
        Logger::writeToLog ("Shutdown");

    deleteLogger();
}

struct AsyncQuitRetrier final : private Timer
{
    AsyncQuitRetrier()   { startTimer (500); }

    void timerCallback() override
    {
        stopTimer();
        delete this;

        if (auto* app = HIGGSApplicationBase::getInstance())
            app->systemRequestedQuit();
    }

    HIGGS_DECLARE_NON_COPYABLE (AsyncQuitRetrier)
};

void MythosStudioApplication::systemRequestedQuit()
{
    if (ModalComponentManager::getInstance()->cancelAllModalComponents())
    {
        new AsyncQuitRetrier();
    }
    else
    {
        closeAllMainWindows ([] (bool closedSuccessfully)
        {
            if (closedSuccessfully)
                MythosStudioApplication::quit();
        });
    }
}

//==============================================================================
String MythosStudioApplication::getVersionDescription() const
{
    String s;

    const Time buildDate (Time::getCompilationDate());

    s << "MythosStudio " << ProjectInfo::versionString
      << newLine
      << "Build date: " << buildDate.getDayOfMonth()
      << " " << Time::getMonthName (buildDate.getMonth(), true)
      << " " << buildDate.getYear();

    return s;
}

void MythosStudioApplication::anotherInstanceStarted (const String& commandLine)
{
    if (! commandLine.trim().startsWithChar ('-'))
    {
        ArgumentList list ({}, commandLine);

        for (auto& arg : list.arguments)
            openFile (arg.resolveAsFile(), nullptr);
    }
}

MythosStudioApplication& MythosStudioApplication::getApp()
{
    MythosStudioApplication* const app = dynamic_cast<MythosStudioApplication*> (HIGGSApplication::getInstance());
    jassert (app != nullptr);
    return *app;
}

ApplicationCommandManager& MythosStudioApplication::getCommandManager()
{
    auto* cm = MythosStudioApplication::getApp().commandManager.get();
    jassert (cm != nullptr);
    return *cm;
}


//==============================================================================
enum
{
    recentProjectsBaseID = 100,
    openWindowsBaseID = 300,
    activeDocumentsBaseID = 400,
    showPathsID = 1999,
    examplesBaseID = 2000
};

MenuBarModel* MythosStudioApplication::getMenuModel()
{
    return menuModel.get();
}

StringArray MythosStudioApplication::getMenuNames()
{
    return { "File", "Edit", "View", "Window", "Document", "Tools", "Help" };
}

PopupMenu MythosStudioApplication::createMenu (const String& menuName)
{
    if (menuName == "File")
        return createFileMenu();

    if (menuName == "Edit")
        return createEditMenu();

    if (menuName == "View")
        return createViewMenu();

    if (menuName == "Window")
        return createWindowMenu();

    if (menuName == "Document")
        return createDocumentMenu();

    if (menuName == "Tools")
        return createToolsMenu();

    if (menuName == "Help")
        return createHelpMenu();

    jassertfalse; // names have changed?
    return {};
}

PopupMenu MythosStudioApplication::createFileMenu()
{
    PopupMenu menu;
    menu.addCommandItem (commandManager.get(), CommandIDs::newProject);
    menu.addCommandItem (commandManager.get(), CommandIDs::newProjectFromClipboard);
    menu.addCommandItem (commandManager.get(), CommandIDs::newPIP);
    menu.addSeparator();
    menu.addCommandItem (commandManager.get(), CommandIDs::open);

    {
        PopupMenu recentFiles;

        settings->recentFiles.createPopupMenuItems (recentFiles, recentProjectsBaseID, true, true);

        if (recentFiles.getNumItems() > 0)
        {
            recentFiles.addSeparator();
            recentFiles.addCommandItem (commandManager.get(), CommandIDs::clearRecentFiles);
        }

        menu.addSubMenu ("Open Recent", recentFiles);
    }

    menu.addSubMenu ("Open Example", createExamplesPopupMenu());

    menu.addSeparator();
    menu.addCommandItem (commandManager.get(), CommandIDs::closeDocument);
    menu.addCommandItem (commandManager.get(), CommandIDs::saveDocument);
    menu.addCommandItem (commandManager.get(), CommandIDs::saveDocumentAs);
    menu.addCommandItem (commandManager.get(), CommandIDs::saveAll);
    menu.addSeparator();
    menu.addCommandItem (commandManager.get(), CommandIDs::closeProject);
    menu.addCommandItem (commandManager.get(), CommandIDs::saveProject);
    menu.addSeparator();
    menu.addCommandItem (commandManager.get(), CommandIDs::openInIDE);
    menu.addCommandItem (commandManager.get(), CommandIDs::saveAndOpenInIDE);

   #if ! HIGGS_MAC
    menu.addCommandItem (commandManager.get(), CommandIDs::showAboutWindow);
    menu.addCommandItem (commandManager.get(), CommandIDs::checkForNewVersion);
    menu.addCommandItem (commandManager.get(), CommandIDs::enableNewVersionCheck);
    menu.addCommandItem (commandManager.get(), CommandIDs::showGlobalPathsWindow);
    menu.addSeparator();
    menu.addCommandItem (commandManager.get(), StandardApplicationCommandIDs::quit);
   #endif

    return menu;
}

PopupMenu MythosStudioApplication::createEditMenu()
{
    PopupMenu menu;
    menu.addCommandItem (commandManager.get(), StandardApplicationCommandIDs::undo);
    menu.addCommandItem (commandManager.get(), StandardApplicationCommandIDs::redo);
    menu.addSeparator();
    menu.addCommandItem (commandManager.get(), StandardApplicationCommandIDs::cut);
    menu.addCommandItem (commandManager.get(), StandardApplicationCommandIDs::copy);
    menu.addCommandItem (commandManager.get(), StandardApplicationCommandIDs::paste);
    menu.addCommandItem (commandManager.get(), StandardApplicationCommandIDs::del);
    menu.addCommandItem (commandManager.get(), StandardApplicationCommandIDs::selectAll);
    menu.addCommandItem (commandManager.get(), StandardApplicationCommandIDs::deselectAll);
    menu.addSeparator();
    menu.addCommandItem (commandManager.get(), CommandIDs::showFindPanel);
    menu.addCommandItem (commandManager.get(), CommandIDs::findSelection);
    menu.addCommandItem (commandManager.get(), CommandIDs::findNext);
    menu.addCommandItem (commandManager.get(), CommandIDs::findPrevious);
    return menu;
}

PopupMenu MythosStudioApplication::createViewMenu()
{
    PopupMenu menu;
    menu.addCommandItem (commandManager.get(), CommandIDs::showProjectSettings);
    menu.addCommandItem (commandManager.get(), CommandIDs::showFileExplorerPanel);
    menu.addCommandItem (commandManager.get(), CommandIDs::showModulesPanel);
    menu.addCommandItem (commandManager.get(), CommandIDs::showExportersPanel);
    menu.addCommandItem (commandManager.get(), CommandIDs::showExporterSettings);

    menu.addSeparator();
    createColourSchemeItems (menu);

    return menu;
}

void MythosStudioApplication::createColourSchemeItems (PopupMenu& menu)
{
    {
        PopupMenu colourSchemeMenu;

        colourSchemeMenu.addItem (PopupMenu::Item ("Dark")
                                    .setTicked (selectedColourSchemeIndex == 0)
                                    .setAction ([this] { setColourScheme (0, true); updateEditorColourSchemeIfNeeded(); }));

        colourSchemeMenu.addItem (PopupMenu::Item ("Grey")
                                    .setTicked (selectedColourSchemeIndex == 1)
                                    .setAction ([this] { setColourScheme (1, true); updateEditorColourSchemeIfNeeded(); }));

        colourSchemeMenu.addItem (PopupMenu::Item ("Light")
                                    .setTicked (selectedColourSchemeIndex == 2)
                                    .setAction ([this] { setColourScheme (2, true); updateEditorColourSchemeIfNeeded(); }));

        menu.addSubMenu ("Colour Scheme", colourSchemeMenu);
    }

    {
        PopupMenu editorColourSchemeMenu;

        auto& appearanceSettings = getAppSettings().appearance;

        appearanceSettings.refreshPresetSchemeList();
        auto schemes = appearanceSettings.getPresetSchemes();

        auto i = 0;

        for (auto& s : schemes)
        {
            editorColourSchemeMenu.addItem (PopupMenu::Item (s)
                                               .setEnabled (editorColourSchemeWindow == nullptr)
                                               .setTicked (selectedEditorColourSchemeIndex == i)
                                               .setAction ([this, i] { setEditorColourScheme (i, true); }));
            ++i;
        }

        editorColourSchemeMenu.addSeparator();
        editorColourSchemeMenu.addItem (PopupMenu::Item ("Create...")
                                           .setEnabled (editorColourSchemeWindow == nullptr)
                                           .setAction ([this] { showEditorColourSchemeWindow(); }));

        menu.addSubMenu ("Editor Colour Scheme", editorColourSchemeMenu);
    }
}

PopupMenu MythosStudioApplication::createWindowMenu()
{
    PopupMenu menu;
    menu.addCommandItem (commandManager.get(), CommandIDs::goToPreviousWindow);
    menu.addCommandItem (commandManager.get(), CommandIDs::goToNextWindow);
    menu.addCommandItem (commandManager.get(), CommandIDs::closeWindow);
    menu.addSeparator();

    int counter = 0;

    for (auto* window : mainWindowList.windows)
    {
        if (window != nullptr)
        {
            if (auto* project = window->getProject())
                menu.addItem (openWindowsBaseID + counter++, project->getProjectNameString());
        }
    }

    menu.addSeparator();
    menu.addCommandItem (commandManager.get(), CommandIDs::closeAllWindows);
    return menu;
}

PopupMenu MythosStudioApplication::createDocumentMenu()
{
    PopupMenu menu;
    menu.addCommandItem (commandManager.get(), CommandIDs::goToPreviousDoc);
    menu.addCommandItem (commandManager.get(), CommandIDs::goToNextDoc);
    menu.addCommandItem (commandManager.get(), CommandIDs::goToCounterpart);
    menu.addSeparator();

    auto numDocs = jmin (50, openDocumentManager.getNumOpenDocuments());

    for (int i = 0; i < numDocs; ++i)
    {
        OpenDocumentManager::Document* doc = openDocumentManager.getOpenDocument (i);
        menu.addItem (activeDocumentsBaseID + i, doc->getName());
    }

    menu.addSeparator();
    menu.addCommandItem (commandManager.get(), CommandIDs::closeAllDocuments);
    return menu;
}

PopupMenu MythosStudioApplication::createToolsMenu()
{
    PopupMenu menu;
    menu.addCommandItem (commandManager.get(), CommandIDs::showUTF8Tool);
    menu.addCommandItem (commandManager.get(), CommandIDs::showSVGPathTool);
    menu.addCommandItem (commandManager.get(), CommandIDs::showTranslationTool);
    return menu;
}

PopupMenu MythosStudioApplication::createHelpMenu()
{
    PopupMenu menu;
    menu.addCommandItem (commandManager.get(), CommandIDs::showForum);
    menu.addSeparator();
    menu.addCommandItem (commandManager.get(), CommandIDs::showAPIModules);
    menu.addCommandItem (commandManager.get(), CommandIDs::showAPIClasses);
    menu.addCommandItem (commandManager.get(), CommandIDs::showTutorials);
    return menu;
}

PopupMenu MythosStudioApplication::createExtraAppleMenuItems()
{
    PopupMenu menu;
    menu.addCommandItem (commandManager.get(), CommandIDs::showAboutWindow);
    menu.addCommandItem (commandManager.get(), CommandIDs::checkForNewVersion);
    menu.addCommandItem (commandManager.get(), CommandIDs::enableNewVersionCheck);
    menu.addSeparator();
    menu.addCommandItem (commandManager.get(), CommandIDs::showGlobalPathsWindow);
    return menu;
}

PopupMenu MythosStudioApplication::createExamplesPopupMenu() noexcept
{
    PopupMenu menu;
    numExamples = 0;
    for (auto& dir : getSortedExampleDirectories())
    {
        PopupMenu m;
        for (auto& f : getSortedExampleFilesInDirectory (dir))
        {
            m.addItem (examplesBaseID + numExamples, f.getFileNameWithoutExtension());
            ++numExamples;
        }

        menu.addSubMenu (dir.getFileName(), m);
    }

    if (numExamples == 0)
    {
        menu.addItem (showPathsID, "Set path to HIGGS...");
    }
    else
    {
        menu.addSeparator();
        menu.addCommandItem (commandManager.get(), CommandIDs::launchDemoRunner);
    }

    return menu;
}

#if HIGGS_MAC
 void MythosStudioApplication::rebuildAppleMenu()
 {
     auto extraAppleMenuItems = createExtraAppleMenuItems();

     // workaround broken "Open Recent" submenu: not passing the
     // submenu's title here avoids the defect in HiggsMainMenuHandler::addMenuItem
     MenuBarModel::setMacMainMenu (menuModel.get(), &extraAppleMenuItems); //, "Open Recent");
 }
#endif

//==============================================================================
File MythosStudioApplication::getHIGGSExamplesDirectoryPathFromGlobal() noexcept
{
    auto globalPath = File::createFileWithoutCheckingPath (getAppSettings().getStoredPath (Ids::higgsPath, TargetOS::getThisOS()).get().toString()
                                                                           .replace ("~", File::getSpecialLocation (File::userHomeDirectory).getFullPathName()));

    if (globalPath.exists())
        return File (globalPath).getChildFile ("examples");

    return {};
}

Array<File> MythosStudioApplication::getSortedExampleDirectories() noexcept
{
    Array<File> exampleDirectories;

    auto examplesPath = getHIGGSExamplesDirectoryPathFromGlobal();

    if (! isValidHIGGSExamplesDirectory (examplesPath))
        return {};

    for (const auto& iter : RangedDirectoryIterator (examplesPath, false, "*", File::findDirectories))
    {
        auto exampleDirectory = iter.getFile();

        if (exampleDirectory.getNumberOfChildFiles (File::findFiles | File::ignoreHiddenFiles) > 0
            && exampleDirectory.getFileName() != "DemoRunner"
            && exampleDirectory.getFileName() != "Assets"
            && exampleDirectory.getFileName() != "CMake")
        {
            exampleDirectories.add (exampleDirectory);
        }
    }

    exampleDirectories.sort();

    return exampleDirectories;
}

Array<File> MythosStudioApplication::getSortedExampleFilesInDirectory (const File& directory) noexcept
{
    Array<File> exampleFiles;

    for (const auto& iter : RangedDirectoryIterator (directory, false, "*.h", File::findFiles))
        exampleFiles.add (iter.getFile());

    exampleFiles.sort();

    return exampleFiles;
}

void MythosStudioApplication::findAndLaunchExample (int selectedIndex)
{
    File example;

    for (auto& dir : getSortedExampleDirectories())
    {
        auto exampleFiles = getSortedExampleFilesInDirectory (dir);

        if (selectedIndex < exampleFiles.size())
        {
            example = exampleFiles.getUnchecked (selectedIndex);
            break;
        }

        selectedIndex -= exampleFiles.size();
    }

    // example doesn't exist?
    jassert (example != File());

    openFile (example, nullptr);
}

//==============================================================================
static String getPlatformSpecificFileExtension()
{
   #if HIGGS_MAC
    return ".app";
   #elif HIGGS_WINDOWS
    return ".exe";
   #elif HIGGS_LINUX || HIGGS_BSD
    return {};
   #else
    jassertfalse;
    return {};
   #endif
}

static File getPlatformSpecificProjectFolder()
{
    auto examplesDir = MythosStudioApplication::getHIGGSExamplesDirectoryPathFromGlobal();

    if (examplesDir == File())
        return {};

    auto buildsFolder = examplesDir.getChildFile ("DemoRunner").getChildFile ("Builds");

   #if HIGGS_MAC
    return buildsFolder.getChildFile ("MacOSX");
   #elif HIGGS_WINDOWS
    return buildsFolder.getChildFile ("VisualStudio2022");
   #elif HIGGS_LINUX || HIGGS_BSD
    return buildsFolder.getChildFile ("LinuxMakefile");
   #else
    jassertfalse;
    return {};
   #endif
}

static File tryToFindDemoRunnerExecutableInBuilds()
{
    auto projectFolder = getPlatformSpecificProjectFolder();

    if (projectFolder == File())
        return {};

   #if HIGGS_MAC
    projectFolder = projectFolder.getChildFile ("build");
    auto demoRunnerExecutable = projectFolder.getChildFile ("Release").getChildFile ("DemoRunner.app");

    if (demoRunnerExecutable.exists())
        return demoRunnerExecutable;

    demoRunnerExecutable = projectFolder.getChildFile ("Debug").getChildFile ("DemoRunner.app");

    if (demoRunnerExecutable.exists())
        return demoRunnerExecutable;
   #elif HIGGS_WINDOWS
    projectFolder = projectFolder.getChildFile ("x64");
    auto demoRunnerExecutable = projectFolder.getChildFile ("Release").getChildFile ("App").getChildFile ("DemoRunner.exe");

    if (demoRunnerExecutable.existsAsFile())
        return demoRunnerExecutable;

    demoRunnerExecutable = projectFolder.getChildFile ("Debug").getChildFile ("App").getChildFile ("DemoRunner.exe");

    if (demoRunnerExecutable.existsAsFile())
        return demoRunnerExecutable;
   #elif HIGGS_LINUX || HIGGS_BSD
    projectFolder = projectFolder.getChildFile ("build");
    auto demoRunnerExecutable = projectFolder.getChildFile ("DemoRunner");

    if (demoRunnerExecutable.existsAsFile())
        return demoRunnerExecutable;
   #endif

    return {};
}

static File tryToFindPrebuiltDemoRunnerExecutable()
{
    auto prebuiltFile = File (getAppSettings().getStoredPath (Ids::higgsPath, TargetOS::getThisOS()).get().toString())
                               .getChildFile ("DemoRunner" + getPlatformSpecificFileExtension());

   #if HIGGS_MAC
    if (prebuiltFile.exists())
   #else
    if (prebuiltFile.existsAsFile())
   #endif
        return prebuiltFile;

    return {};
}

void MythosStudioApplication::checkIfGlobalHIGGSPathHasChanged()
{
    auto globalHIGGSPath = File (getAppSettings().getStoredPath (Ids::higgsPath, TargetOS::getThisOS()).get());

    if (lastHIGGSPath != globalHIGGSPath)
    {
        hasScannedForDemoRunnerProject = false;
        hasScannedForDemoRunnerExecutable = false;

        lastHIGGSPath = globalHIGGSPath;
    }
}

File MythosStudioApplication::tryToFindDemoRunnerExecutable()
{
    checkIfGlobalHIGGSPathHasChanged();

    if (hasScannedForDemoRunnerExecutable)
        return lastDemoRunnerExectuableFile;

    hasScannedForDemoRunnerExecutable = true;

    auto demoRunnerExecutable = tryToFindDemoRunnerExecutableInBuilds();

    if (demoRunnerExecutable == File())
        demoRunnerExecutable = tryToFindPrebuiltDemoRunnerExecutable();

    lastDemoRunnerExectuableFile = demoRunnerExecutable;

    return demoRunnerExecutable;
}

File MythosStudioApplication::tryToFindDemoRunnerProject()
{
    checkIfGlobalHIGGSPathHasChanged();

    if (hasScannedForDemoRunnerProject)
        return lastDemoRunnerProjectFile;

    hasScannedForDemoRunnerProject = true;

    auto projectFolder = getPlatformSpecificProjectFolder();

    if (projectFolder == File())
    {
        lastDemoRunnerProjectFile = File();
        return {};
    }

   #if HIGGS_MAC
    auto demoRunnerProjectFile = projectFolder.getChildFile ("DemoRunner.xcodeproj");
   #elif HIGGS_WINDOWS
    auto demoRunnerProjectFile = projectFolder.getChildFile ("DemoRunner.sln");
   #elif HIGGS_LINUX || HIGGS_BSD
    auto demoRunnerProjectFile = projectFolder.getChildFile ("Makefile");
   #endif

   #if HIGGS_MAC
    if (! demoRunnerProjectFile.exists())
   #else
    if (! demoRunnerProjectFile.existsAsFile())
   #endif
        demoRunnerProjectFile = File();

    lastDemoRunnerProjectFile = demoRunnerProjectFile;

    return demoRunnerProjectFile;
}

void MythosStudioApplication::launchDemoRunner()
{
    auto demoRunnerFile = tryToFindDemoRunnerExecutable();

    if (demoRunnerFile != File() && demoRunnerFile.startAsProcess())
        return;

    demoRunnerFile = tryToFindDemoRunnerProject();

    if (demoRunnerFile != File())
    {
        auto& lf = Desktop::getInstance().getDefaultLookAndFeel();

       #if HIGGS_LINUX || HIGGS_BSD
        const auto opt = MessageBoxOptions{}.withIconType (MessageBoxIconType::WarningIcon)
                                            .withTitle ("Open Project")
                                            .withMessage ("Couldn't find a compiled version of the Demo Runner."
                                                          " Please compile the Demo Runner project in the HIGGS examples directory.")
                                            .withButton ("OK", 0)
                                            .withAssociatedComponent (mainWindowList.getFrontmostWindow (false));
        demoRunnerAlert = lf.createAlertWindowForOptions (opt);
        demoRunnerAlert->enterModalState (true, ModalCallbackFunction::create ([this] (int)
                                                {
                                                    demoRunnerAlert.reset (nullptr);
                                                }), false);

       #else
        const auto opt = MessageBoxOptions{}.withIconType (MessageBoxIconType::QuestionIcon)
                                            .withTitle ("Open Project")
                                            .withMessage ("Couldn't find a compiled version of the Demo Runner."
                                                          " Do you want to open the project?")
                                            .withButton ("Open project", 1)
                                            .withButton ("Cancel", 0)
                                            .withAssociatedComponent (mainWindowList.getFrontmostWindow (false));
        demoRunnerAlert = lf.createAlertWindowForOptions (opt);
        demoRunnerAlert->enterModalState (true, ModalCallbackFunction::create ([this, demoRunnerFile] (int retVal)
                                                {
                                                    demoRunnerAlert.reset (nullptr);

                                                    if (retVal == 1)
                                                        demoRunnerFile.startAsProcess();
                                                }), false);
       #endif
    }
}

//==============================================================================
void MythosStudioApplication::handleMainMenuCommand (int menuItemID)
{
    if (menuItemID >= recentProjectsBaseID && menuItemID < (recentProjectsBaseID + 100))
    {
        // open a file from the "recent files" menu
        openFile (settings->recentFiles.getFile (menuItemID - recentProjectsBaseID), nullptr);
    }
    else if (menuItemID >= openWindowsBaseID && menuItemID < (openWindowsBaseID + 100))
    {
        if (auto* window = mainWindowList.windows.getUnchecked (menuItemID - openWindowsBaseID))
            window->toFront (true);
    }
    else if (menuItemID >= activeDocumentsBaseID && menuItemID < (activeDocumentsBaseID + 200))
    {
        if (auto* doc = openDocumentManager.getOpenDocument (menuItemID - activeDocumentsBaseID))
            mainWindowList.openDocument (doc, true);
        else
            jassertfalse;
    }
    else if (menuItemID == showPathsID)
    {
        showPathsWindow (true);
    }
    else if (menuItemID >= examplesBaseID && menuItemID < (examplesBaseID + numExamples))
    {
        findAndLaunchExample (menuItemID - examplesBaseID);
    }
}

//==============================================================================
void MythosStudioApplication::getAllCommands (Array <CommandID>& commands)
{
    HIGGSApplication::getAllCommands (commands);

    const CommandID ids[] = { CommandIDs::newProject,
                              CommandIDs::newProjectFromClipboard,
                              CommandIDs::newPIP,
                              CommandIDs::open,
                              CommandIDs::launchDemoRunner,
                              CommandIDs::closeAllWindows,
                              CommandIDs::closeAllDocuments,
                              CommandIDs::clearRecentFiles,
                              CommandIDs::saveAll,
                              CommandIDs::showGlobalPathsWindow,
                              CommandIDs::showUTF8Tool,
                              CommandIDs::showSVGPathTool,
                              CommandIDs::showAboutWindow,
                              CommandIDs::checkForNewVersion,
                              CommandIDs::enableNewVersionCheck,
                              CommandIDs::showForum,
                              CommandIDs::showAPIModules,
                              CommandIDs::showAPIClasses,
                              CommandIDs::showTutorials };

    commands.addArray (ids, numElementsInArray (ids));
}

void MythosStudioApplication::getCommandInfo (CommandID commandID, ApplicationCommandInfo& result)
{
    switch (commandID)
    {
    case CommandIDs::newProject:
        result.setInfo ("New Project...", "Creates a new HIGGS project", CommandCategories::general, 0);
        result.defaultKeypresses.add (KeyPress ('n', ModifierKeys::commandModifier, 0));
        break;

    case CommandIDs::newProjectFromClipboard:
        result.setInfo ("New Project From Clipboard...", "Creates a new HIGGS project from the clipboard contents", CommandCategories::general, 0);
        result.defaultKeypresses.add (KeyPress ('n', ModifierKeys::commandModifier | ModifierKeys::shiftModifier, 0));
        break;

    case CommandIDs::newPIP:
        result.setInfo ("New PIP...", "Opens the PIP Creator utility for creating a new PIP", CommandCategories::general, 0);
        result.defaultKeypresses.add (KeyPress ('p', ModifierKeys::commandModifier | ModifierKeys::shiftModifier, 0));
        break;

    case CommandIDs::launchDemoRunner:
        result.setInfo ("Launch Demo Runner", "Launches the HIGGS demo runner application, or the project if it can't be found", CommandCategories::general, 0);
        result.setActive (tryToFindDemoRunnerExecutable() != File() || tryToFindDemoRunnerProject() != File());
        break;

    case CommandIDs::open:
        result.setInfo ("Open...", "Opens a HIGGS project", CommandCategories::general, 0);
        result.defaultKeypresses.add (KeyPress ('o', ModifierKeys::commandModifier, 0));
        break;

    case CommandIDs::showGlobalPathsWindow:
        result.setInfo ("Global Paths...",
                        "Shows the window to change the stored global paths.",
                        CommandCategories::general, 0);
        break;

    case CommandIDs::closeAllWindows:
        result.setInfo ("Close All Windows", "Closes all open windows", CommandCategories::general, 0);
        result.setActive (mainWindowList.windows.size() > 0);
        break;

    case CommandIDs::closeAllDocuments:
        result.setInfo ("Close All Documents", "Closes all open documents", CommandCategories::general, 0);
        result.setActive (openDocumentManager.getNumOpenDocuments() > 0);
        break;

    case CommandIDs::clearRecentFiles:
        result.setInfo ("Clear Recent Files", "Clears all recent files from the menu", CommandCategories::general, 0);
        result.setActive (settings->recentFiles.getNumFiles() > 0);
        break;

    case CommandIDs::saveAll:
        result.setInfo ("Save All", "Saves all open documents", CommandCategories::general, 0);
        result.defaultKeypresses.add (KeyPress ('s', ModifierKeys::commandModifier | ModifierKeys::altModifier, 0));
        break;

    case CommandIDs::showUTF8Tool:
        result.setInfo ("UTF-8 String-Literal Helper", "Shows the UTF-8 string literal utility", CommandCategories::general, 0);
        break;

    case CommandIDs::showSVGPathTool:
        result.setInfo ("SVG Path Converter", "Shows the SVG->Path data conversion utility", CommandCategories::general, 0);
        break;

    case CommandIDs::showAboutWindow:
        result.setInfo ("About MythosStudio", "Shows the MythosStudio's 'About' page.", CommandCategories::general, 0);
        break;

    case CommandIDs::checkForNewVersion:
        result.setInfo ("Check for New Version...", "Checks the web server for a new version of HIGGS", CommandCategories::general, 0);
        break;

    case CommandIDs::enableNewVersionCheck:
        result.setInfo ("Automatically Check for New Versions",
                        "Enables automatic background checking for new versions of HIGGS.",
                        CommandCategories::general,
                        (isAutomaticVersionCheckingEnabled() ? ApplicationCommandInfo::isTicked : 0));
        break;

    case CommandIDs::showForum:
        result.setInfo ("HIGGS Community Forum", "Shows the HIGGS community forum in a browser", CommandCategories::general, 0);
        break;

    case CommandIDs::showAPIModules:
        result.setInfo ("API Modules", "Shows the API modules documentation in a browser", CommandCategories::general, 0);
        break;

    case CommandIDs::showAPIClasses:
        result.setInfo ("API Classes", "Shows the API classes documentation in a browser", CommandCategories::general, 0);
        break;

    case CommandIDs::showTutorials:
        result.setInfo ("HIGGS Tutorials", "Shows the HIGGS tutorials in a browser", CommandCategories::general, 0);
        break;

    default:
        HIGGSApplication::getCommandInfo (commandID, result);
        break;
    }
}

bool MythosStudioApplication::perform (const InvocationInfo& info)
{
    switch (info.commandID)
    {
        case CommandIDs::newProject:                createNewProject(); break;
        case CommandIDs::newProjectFromClipboard:   createNewProjectFromClipboard(); break;
        case CommandIDs::newPIP:                    createNewPIP(); break;
        case CommandIDs::open:                      askUserToOpenFile(); break;
        case CommandIDs::launchDemoRunner:          launchDemoRunner(); break;
        case CommandIDs::saveAll:                   saveAllDocuments(); break;
        case CommandIDs::closeAllWindows:           closeAllMainWindowsAndQuitIfNeeded(); break;
        case CommandIDs::closeAllDocuments:         closeAllDocuments (OpenDocumentManager::SaveIfNeeded::yes); break;
        case CommandIDs::clearRecentFiles:          clearRecentFiles(); break;
        case CommandIDs::showUTF8Tool:              showUTF8ToolWindow(); break;
        case CommandIDs::showSVGPathTool:           showSVGPathDataToolWindow(); break;
        case CommandIDs::showGlobalPathsWindow:     showPathsWindow (false); break;
        case CommandIDs::showAboutWindow:           showAboutWindow(); break;
        case CommandIDs::checkForNewVersion:        LatestVersionCheckerAndUpdater::getInstance()->checkForNewVersion (false); break;
        case CommandIDs::enableNewVersionCheck:     setAutomaticVersionCheckingEnabled (! isAutomaticVersionCheckingEnabled()); break;
        case CommandIDs::showForum:                 launchForumBrowser(); break;
        case CommandIDs::showAPIModules:            launchModulesBrowser(); break;
        case CommandIDs::showAPIClasses:            launchClassesBrowser(); break;
        case CommandIDs::showTutorials:             launchTutorialsBrowser(); break;
        default:                                    return HIGGSApplication::perform (info);
    }

    return true;
}

//==============================================================================
void MythosStudioApplication::createNewProject()
{
    auto* mw = mainWindowList.getOrCreateEmptyWindow();
    jassert (mw != nullptr);

    mw->showStartPage();

    mainWindowList.checkWindowBounds (*mw);
}

void MythosStudioApplication::createNewProjectFromClipboard()
{
    auto tempFile = File::getSpecialLocation (File::SpecialLocationType::tempDirectory).getChildFile ("PIPs").getChildFile ("Clipboard")
                                                                                       .getChildFile ("PIPFile_" + String (std::abs (Random::getSystemRandom().nextInt())) + ".h")
                                                                                       .getNonexistentSibling();

    if (tempFile.existsAsFile())
        tempFile.deleteFile();

    tempFile.create();
    tempFile.appendText (SystemClipboard::getTextFromClipboard());

    auto cleanup = [parent = WeakReference { this }, tempFile] (String errorString)
    {
        if (parent == nullptr || errorString.isEmpty())
            return;

        auto options = MessageBoxOptions::makeOptionsOk (MessageBoxIconType::WarningIcon, "Error", errorString);
        parent->messageBox = AlertWindow::showScopedAsync (options, nullptr);
        tempFile.deleteFile();
    };

    if (! isPIPFile (tempFile))
    {
        cleanup ("Clipboard does not contain a valid PIP.");
        return;
    }

    openFile (tempFile, [parent = WeakReference { this }, cleanup] (bool openedSuccessfully)
    {
        if (parent == nullptr)
            return;

        if (! openedSuccessfully)
        {
            cleanup ("Couldn't create project from clipboard contents.");
            parent->mainWindowList.closeWindow (parent->mainWindowList.windows.getLast());
        }
    });
}

void MythosStudioApplication::createNewPIP()
{
    showPIPCreatorWindow();
}

void MythosStudioApplication::askUserToOpenFile()
{
    chooser = std::make_unique<FileChooser> ("Open File");
    auto flags = FileBrowserComponent::openMode | FileBrowserComponent::canSelectFiles;

    chooser->launchAsync (flags, [this] (const FileChooser& fc)
    {
        const auto result = fc.getResult();

        if (result != File{})
            openFile (result, nullptr);
    });
}

void MythosStudioApplication::openFile (const File& file, std::function<void (bool)> callback)
{
    mainWindowList.openFile (file, std::move (callback));
}

void MythosStudioApplication::saveAllDocuments()
{
    openDocumentManager.saveAllSyncWithoutAsking();

    for (int i = 0; i < mainWindowList.windows.size(); ++i)
        if (auto* pcc = mainWindowList.windows.getUnchecked (i)->getProjectContentComponent())
            pcc->refreshProjectTreeFileStatuses();
}

void MythosStudioApplication::closeAllDocuments (OpenDocumentManager::SaveIfNeeded askUserToSave)
{
    openDocumentManager.closeAllAsync (askUserToSave, nullptr);
}

void MythosStudioApplication::closeAllMainWindows (std::function<void (bool)> callback)
{
    mainWindowList.askAllWindowsToClose (std::move (callback));
}

void MythosStudioApplication::closeAllMainWindowsAndQuitIfNeeded()
{
    closeAllMainWindows ([parent = WeakReference<MythosStudioApplication> { this }] (bool closedSuccessfully)
    {
       #if HIGGS_MAC
        ignoreUnused (parent, closedSuccessfully);
       #else
        if (parent == nullptr)
            return;

        if (closedSuccessfully && parent->mainWindowList.windows.size() == 0)
            parent->systemRequestedQuit();
       #endif
    });
}

void MythosStudioApplication::clearRecentFiles()
{
    settings->recentFiles.clear();
    settings->recentFiles.clearRecentFilesNatively();
    settings->flush();
    menuModel->menuItemsChanged();
}

//==============================================================================
void MythosStudioApplication::showUTF8ToolWindow()
{
    if (utf8Window != nullptr)
        utf8Window->toFront (true);
    else
        new FloatingToolWindow ("UTF-8 String Literal Converter", "utf8WindowPos",
                                new UTF8Component(), utf8Window, true,
                                500, 500, 300, 300, 1000, 1000);
}

void MythosStudioApplication::showSVGPathDataToolWindow()
{
    if (svgPathWindow != nullptr)
        svgPathWindow->toFront (true);
    else
        new FloatingToolWindow ("SVG Path Converter", "svgPathWindowPos",
                                new SVGPathDataComponent(), svgPathWindow, true,
                                500, 500, 300, 300, 1000, 1000);
}

void MythosStudioApplication::showAboutWindow()
{
    if (aboutWindow != nullptr)
        aboutWindow->toFront (true);
    else
        new FloatingToolWindow ({}, {}, new AboutWindowComponent(),
                                aboutWindow, false,
                                500, 300, 500, 300, 500, 300);
}

void MythosStudioApplication::showPathsWindow (bool highlightHIGGSPath)
{
    if (pathsWindow != nullptr)
        pathsWindow->toFront (true);
    else
        new FloatingToolWindow ("Global Paths", "pathsWindowPos",
                                new GlobalPathsWindowComponent(), pathsWindow, false,
                                600, 700, 600, 700, 600, 700);

    if (highlightHIGGSPath)
        if (auto* pathsComp = dynamic_cast<GlobalPathsWindowComponent*> (pathsWindow->getChildComponent (0)))
            pathsComp->highlightHIGGSPath();
}

void MythosStudioApplication::showEditorColourSchemeWindow()
{
    if (editorColourSchemeWindow != nullptr)
        editorColourSchemeWindow->toFront (true);
    else
        new FloatingToolWindow ("Editor Colour Scheme", "editorColourSchemeWindowPos",
                                new EditorColourSchemeWindowComponent(), editorColourSchemeWindow, false,
                                500, 500, 500, 500, 500, 500);
}

void MythosStudioApplication::showPIPCreatorWindow()
{
    if (pipCreatorWindow != nullptr)
        pipCreatorWindow->toFront (true);
    else
        new FloatingToolWindow ("PIP Creator", "pipCreatorWindowPos",
                                new PIPCreatorWindowComponent(), pipCreatorWindow, false,
                                600, 750, 600, 750, 600, 750);
}

void MythosStudioApplication::launchForumBrowser()
{
    URL forumLink ("https://github.com/jalexiscv/higgs-mythos/discussions");

    if (forumLink.isWellFormed())
        forumLink.launchInDefaultBrowser();
}

void MythosStudioApplication::launchModulesBrowser()
{
    URL modulesLink ("https://github.com/jalexiscv/higgs-mythos/tree/main/docs/modules.html");

    if (modulesLink.isWellFormed())
        modulesLink.launchInDefaultBrowser();
}

void MythosStudioApplication::launchClassesBrowser()
{
    URL classesLink ("https://github.com/jalexiscv/higgs-mythos/tree/main/docs/classes.html");

    if (classesLink.isWellFormed())
        classesLink.launchInDefaultBrowser();
}

void MythosStudioApplication::launchTutorialsBrowser()
{
    URL tutorialsLink ("https://github.com/jalexiscv/higgs-mythos-tutorials");

    if (tutorialsLink.isWellFormed())
        tutorialsLink.launchInDefaultBrowser();
}

//==============================================================================
struct FileWithTime
{
    FileWithTime (const File& f) : file (f), time (f.getLastModificationTime()) {}
    FileWithTime() {}

    bool operator<  (const FileWithTime& other) const    { return time <  other.time; }
    bool operator== (const FileWithTime& other) const    { return time == other.time; }

    File file;
    Time time;
};

void MythosStudioApplication::deleteLogger()
{
    const int maxNumLogFilesToKeep = 50;

    Logger::setCurrentLogger (nullptr);

    if (logger != nullptr)
    {
        auto logFiles = logger->getLogFile().getParentDirectory().findChildFiles (File::findFiles, false);

        if (logFiles.size() > maxNumLogFilesToKeep)
        {
            Array<FileWithTime> files;

            for (auto& f : logFiles)
                files.addUsingDefaultSort (f);

            for (int i = 0; i < files.size() - maxNumLogFilesToKeep; ++i)
                files.getReference (i).file.deleteFile();
        }
    }

    logger.reset();
}

PropertiesFile::Options MythosStudioApplication::getPropertyFileOptionsFor (const String& filename, bool isProjectSettings)
{
    PropertiesFile::Options options;
    options.applicationName     = filename;
    options.filenameSuffix      = "settings";
    options.osxLibrarySubFolder = "Application Support";
   #if HIGGS_LINUX || HIGGS_BSD
    options.folderName          = "~/.config/MythosStudio";
   #else
    options.folderName          = "MythosStudio";
   #endif

    if (isProjectSettings)
        options.folderName += "/ProjectSettings";

    return options;
}

void MythosStudioApplication::initCommandManager()
{
    commandManager.reset (new ApplicationCommandManager());
    commandManager->registerAllCommandsForTarget (this);

    {
        CodeDocument doc;
        CppCodeEditorComponent ed (File(), doc);
        commandManager->registerAllCommandsForTarget (&ed);
    }
}

static void rescanModules (AvailableModulesList& list, const Array<File>& paths, bool async)
{
    if (async)
        list.scanPathsAsync (paths);
    else
        list.scanPaths (paths);
}

void MythosStudioApplication::rescanHIGGSPathModules()
{
    rescanModules (higgsPathModulesList, { getAppSettings().getStoredPath (Ids::defaultHiggsModulePath, TargetOS::getThisOS()).get().toString() }, ! isRunningCommandLine);
}

void MythosStudioApplication::rescanUserPathModules()
{
    rescanModules (userPathsModulesList, { getAppSettings().getStoredPath (Ids::defaultUserModulePath, TargetOS::getThisOS()).get().toString() }, ! isRunningCommandLine);
}

bool MythosStudioApplication::isAutomaticVersionCheckingEnabled() const
{
    return ! getGlobalProperties().getBoolValue (Ids::dontQueryForUpdate);
}

void MythosStudioApplication::setAutomaticVersionCheckingEnabled (bool enabled)
{
    getGlobalProperties().setValue (Ids::dontQueryForUpdate, ! enabled);
}

bool MythosStudioApplication::shouldPromptUserAboutIncorrectHIGGSPath() const
{
    return ! getGlobalProperties().getBoolValue (Ids::dontAskAboutHIGGSPath);
}

void MythosStudioApplication::setShouldPromptUserAboutIncorrectHIGGSPath (bool shouldPrompt)
{
    getGlobalProperties().setValue (Ids::dontAskAboutHIGGSPath, ! shouldPrompt);
}

void MythosStudioApplication::selectEditorColourSchemeWithName (const String& schemeName)
{
    auto& appearanceSettings = getAppSettings().appearance;
    auto schemes = appearanceSettings.getPresetSchemes();

    auto schemeIndex = schemes.indexOf (schemeName);

    if (schemeIndex >= 0)
        setEditorColourScheme (schemeIndex, true);
}

void MythosStudioApplication::setColourScheme (int index, bool saveSetting)
{
    switch (index)
    {
        case 0: lookAndFeel.setColourScheme (LookAndFeel_V4::getDarkColourScheme());  break;
        case 1: lookAndFeel.setColourScheme (LookAndFeel_V4::getGreyColourScheme());  break;
        case 2: lookAndFeel.setColourScheme (LookAndFeel_V4::getLightColourScheme()); break;
        default: break;
    }

    lookAndFeel.setupColours();
    mainWindowList.sendLookAndFeelChange();

    if (utf8Window != nullptr)                  utf8Window->sendLookAndFeelChange();
    if (svgPathWindow != nullptr)               svgPathWindow->sendLookAndFeelChange();
    if (aboutWindow != nullptr)                 aboutWindow->sendLookAndFeelChange();
    if (pathsWindow != nullptr)                 pathsWindow->sendLookAndFeelChange();
    if (editorColourSchemeWindow != nullptr)    editorColourSchemeWindow->sendLookAndFeelChange();
    if (pipCreatorWindow != nullptr)            pipCreatorWindow->sendLookAndFeelChange();

    auto* mcm = ModalComponentManager::getInstance();
    for (auto i = 0; i < mcm->getNumModalComponents(); ++i)
        mcm->getModalComponent (i)->sendLookAndFeelChange();

    if (saveSetting)
    {
        auto& properties = getGlobalProperties();
        properties.setValue ("COLOUR SCHEME", index);
    }

    selectedColourSchemeIndex = index;

    getCommandManager().commandStatusChanged();
}

void MythosStudioApplication::setEditorColourScheme (int index, bool saveSetting)
{
    auto& appearanceSettings = getAppSettings().appearance;
    auto schemes = appearanceSettings.getPresetSchemes();

    index = jmin (index, schemes.size() - 1);

    appearanceSettings.selectPresetScheme (index);

    if (saveSetting)
    {
        auto& properties = getGlobalProperties();
        properties.setValue ("EDITOR COLOUR SCHEME", index);
    }

    selectedEditorColourSchemeIndex = index;

    getCommandManager().commandStatusChanged();
}

static bool isEditorColourSchemeADefaultScheme (const StringArray& schemes, int editorColourSchemeIndex)
{
    auto& schemeName = schemes[editorColourSchemeIndex];
    return (schemeName == "Default (Dark)" || schemeName == "Default (Light)");
}

static int getEditorColourSchemeForGUIColourScheme (const StringArray& schemes, int guiColourSchemeIndex)
{
    auto defaultDarkEditorIndex  = schemes.indexOf ("Default (Dark)");
    auto defaultLightEditorIndex = schemes.indexOf ("Default (Light)");

    // Can't find default code editor colour schemes!
    jassert (defaultDarkEditorIndex != -1 && defaultLightEditorIndex != -1);

    return (guiColourSchemeIndex == 2 ? defaultLightEditorIndex : defaultDarkEditorIndex);
}

void MythosStudioApplication::updateEditorColourSchemeIfNeeded()
{
    auto& appearanceSettings = getAppSettings().appearance;
    auto schemes = appearanceSettings.getPresetSchemes();

    if (isEditorColourSchemeADefaultScheme (schemes, selectedEditorColourSchemeIndex))
        setEditorColourScheme (getEditorColourSchemeForGUIColourScheme (schemes, selectedColourSchemeIndex), true);
}
