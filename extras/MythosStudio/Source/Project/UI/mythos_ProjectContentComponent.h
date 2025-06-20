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

#include "../../CodeEditor/mythos_OpenDocumentManager.h"
#include "mythos_HeaderComponent.h"
#include "mythos_ProjectMessagesComponent.h"
#include "mythos_ContentViewComponent.h"

class Sidebar;
struct WizardHolder;

//==============================================================================
class ProjectContentComponent final : public Component,
                                      public ApplicationCommandTarget,
                                      private ChangeListener,
                                      private OpenDocumentManager::DocumentCloseListener
{
public:
    //==============================================================================
    ProjectContentComponent();
    ~ProjectContentComponent() override;

    Project* getProject() const noexcept    { return project; }
    void setProject (Project*);

    void saveOpenDocumentList();
    void reloadLastOpenDocuments();

    bool showEditorForFile (const File&, bool grabFocus);
    bool hasFileInRecentList (const File&) const;
    File getCurrentFile() const;

    bool showDocument (OpenDocumentManager::Document*, bool grabFocus);
    void hideDocument (OpenDocumentManager::Document*);
    OpenDocumentManager::Document* getCurrentDocument() const    { return currentDocument; }
    void closeDocument();
    void saveDocumentAsync();
    void saveAsAsync();

    void hideEditor();
    void setScrollableEditorComponent (std::unique_ptr<Component> component);
    void setEditorDocument (std::unique_ptr<Component> component, OpenDocumentManager::Document* doc);
    Component* getEditorComponent();

    Component& getSidebarComponent();

    bool goToPreviousFile();
    bool goToNextFile();
    bool canGoToCounterpart() const;
    bool goToCounterpart();

    void saveProjectAsync();
    void closeProject();
    void openInSelectedIDE (bool saveFirst);
    void showNewExporterMenu();

    void showFilesPanel()        { showProjectPanel (0); }
    void showModulesPanel()      { showProjectPanel (1); }
    void showExportersPanel()    { showProjectPanel (2); }

    void showProjectSettings();
    void showCurrentExporterSettings();
    void showExporterSettings (const String& exporterName);
    void showModule (const String& moduleID);

    void deleteSelectedTreeItems();

    void refreshProjectTreeFileStatuses();
    void updateMissingFileStatuses();

    void showBubbleMessage (Rectangle<int>, const String&);

    StringArray getExportersWhichCanLaunch() const;

    static void getSelectedProjectItemsBeingDragged (const DragAndDropTarget::SourceDetails&,
                                                     OwnedArray<Project::Item>& selectedNodes);

    //==============================================================================
    ApplicationCommandTarget* getNextCommandTarget() override;
    void getAllCommands (Array<CommandID>&) override;
    void getCommandInfo (CommandID, ApplicationCommandInfo&) override;
    bool perform (const InvocationInfo&) override;

    bool isSaveCommand (CommandID);

    void paint (Graphics&) override;
    void resized() override;
    void childBoundsChanged (Component*) override;
    void lookAndFeelChanged() override;

    ProjectMessagesComponent& getProjectMessagesComponent()  { return projectMessagesComponent; }

    static String getProjectTabName()    { return "Project"; }

private:
    //==============================================================================
    bool documentAboutToClose (OpenDocumentManager::Document*) override;
    void changeListenerCallback (ChangeBroadcaster*) override;
    void showTranslationTool();

    //==============================================================================
    void showProjectPanel (int index);
    bool canSelectedProjectBeLaunch();

    //==============================================================================
    Project* project = nullptr;
    OpenDocumentManager::Document* currentDocument = nullptr;
    RecentDocumentList recentDocumentList;

    HeaderComponent headerComponent { this };
    std::unique_ptr<Sidebar> sidebar;
    ProjectMessagesComponent projectMessagesComponent;
    ContentViewComponent contentViewComponent;

    std::unique_ptr<ResizableEdgeComponent> resizerBar;
    ComponentBoundsConstrainer sidebarSizeConstrainer;
    std::unique_ptr<Component> translationTool;
    BubbleMessageComponent bubbleMessage;

    bool isForeground = false;
    int lastViewedTab = 0;

    std::unique_ptr<WizardHolder> wizardHolder;
    ScopedMessageBox messageBox;

    //==============================================================================
    HIGGS_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ProjectContentComponent)
};
