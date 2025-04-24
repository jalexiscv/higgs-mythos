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

#include "../Utility/PIPs/mythos_PIPGenerator.h"
#include "../Project/mythos_Project.h"
#include "../CodeEditor/mythos_OpenDocumentManager.h"

class ProjectContentComponent;

//==============================================================================
/**
    The big top-level window where everything happens.
*/
class MainWindow final : public DocumentWindow,
                         public ApplicationCommandTarget,
                         public FileDragAndDropTarget,
                         public DragAndDropContainer,
                         private Value::Listener,
                         private ChangeListener
{
public:
    //==============================================================================
    MainWindow();
    ~MainWindow() override;

    enum class OpenInIDE { no, yes };

    //==============================================================================
    void closeButtonPressed() override;

    //==============================================================================
    bool canOpenFile (const File& file) const;
    void openFile (const File& file, std::function<void (bool)> callback);

    void setProject (std::unique_ptr<Project> newProject);
    Project* getProject() const  { return currentProject.get(); }

    void makeVisible();
    void restoreWindowPosition();
    void updateTitleBarIcon();
    void closeCurrentProject (OpenDocumentManager::SaveIfNeeded askToSave, std::function<void (bool)> callback);
    void moveProject (File newProjectFile, OpenInIDE openInIDE);

    void showStartPage();

    bool isInterestedInFileDrag (const StringArray& files) override;
    void filesDropped (const StringArray& filenames, int mouseX, int mouseY) override;

    void activeWindowStatusChanged() override;

    ProjectContentComponent* getProjectContentComponent() const;

    //==============================================================================
    ApplicationCommandTarget* getNextCommandTarget() override;
    void getAllCommands (Array <CommandID>& commands) override;
    void getCommandInfo (CommandID commandID, ApplicationCommandInfo& result) override;
    bool perform (const InvocationInfo& info) override;

    bool shouldDropFilesWhenDraggedExternally (const DragAndDropTarget::SourceDetails& sourceDetails,
                                               StringArray& files, bool& canMoveFiles) override;
private:
    void valueChanged (Value&) override;
    void changeListenerCallback (ChangeBroadcaster* source) override;

    static const char* getProjectWindowPosName()   { return "projectWindowPos"; }
    void createProjectContentCompIfNeeded();

    void openPIP (const File&, std::function<void (bool)> callback);
    void setupTemporaryPIPProject (PIPGenerator&);

    void initialiseProjectWindow();

    std::unique_ptr<Project> currentProject;
    Value projectNameValue;

    std::unique_ptr<Component> blurOverlayComponent;

    ScopedMessageBox messageBox;

    HIGGS_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainWindow)
};

//==============================================================================
class MainWindowList
{
public:
    MainWindowList();

    void forceCloseAllWindows();
    void askAllWindowsToClose (std::function<void (bool)> callback);
    void closeWindow (MainWindow*);

    void goToSiblingWindow (MainWindow*, int delta);

    void createWindowIfNoneAreOpen();
    void openDocument (OpenDocumentManager::Document*, bool grabFocus);
    void openFile (const File& file, std::function<void (bool)> callback, bool openInBackground = false);

    MainWindow* createNewMainWindow();
    MainWindow* getFrontmostWindow (bool createIfNotFound = true);
    MainWindow* getOrCreateEmptyWindow();
    MainWindow* getMainWindowForFile (const File&);

    Project* getFrontmostProject();

    void reopenLastProjects();
    void saveCurrentlyOpenProjectList();

    void checkWindowBounds (MainWindow&);

    void sendLookAndFeelChange();

    OwnedArray<MainWindow> windows;

private:
    bool isInReopenLastProjects = false;

    HIGGS_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainWindowList)
    HIGGS_DECLARE_WEAK_REFERENCEABLE (MainWindowList)
};
