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

#include "../../Application/mythos_Headers.h"
#include "../../Utility/UI/mythos_IconButton.h"

class Project;
class ProjectContentComponent;
class ProjectExporter;

//==============================================================================
class HeaderComponent final : public Component,
                              private ValueTree::Listener,
                              private Value::Listener,
                              private Timer
{
public:
    HeaderComponent (ProjectContentComponent* projectContentComponent);

    //==============================================================================
    void resized() override;
    void paint (Graphics&) override;

    //==============================================================================
    void setCurrentProject (Project*);

    void updateExporters();
    std::unique_ptr<ProjectExporter> getSelectedExporter() const;
    bool canCurrentExporterLaunchProject() const;

    void sidebarTabsWidthChanged (int newWidth);

private:
    //==============================================================================
    void valueChanged (Value&) override;
    void timerCallback() override;

    //==============================================================================
    void valueTreeChildAdded (ValueTree& parentTree, ValueTree&) override        { updateIfNeeded (parentTree); }
    void valueTreeChildRemoved (ValueTree& parentTree, ValueTree&, int) override { updateIfNeeded (parentTree); }
    void valueTreeChildOrderChanged (ValueTree& parentTree, int, int) override   { updateIfNeeded (parentTree); }

    void updateIfNeeded (ValueTree tree)
    {
        if (tree == exportersTree)
            updateExporters();
    }

    //==============================================================================
    void initialiseButtons();

    void updateName();
    void updateExporterButton();

    //==============================================================================
    int tabsWidth = 200;

    ProjectContentComponent* projectContentComponent = nullptr;
    Project* project = nullptr;
    ValueTree exportersTree;

    Value projectNameValue;

    ComboBox exporterBox;
    Label configLabel  { "Config Label", "Selected exporter" }, projectNameLabel;

    ImageComponent higgsIcon;

    IconButton projectSettingsButton { "Project Settings", getIcons().settings },
               saveAndOpenInIDEButton { "Save and Open in IDE", Image() };

    HIGGS_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (HeaderComponent)
};
