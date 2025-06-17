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

#include "mythos_HeaderComponent.h"

#include "../../Application/mythos_Application.h"

#include "../../ProjectSaving/mythos_ProjectExporter.h"
#include "../../Project/UI/mythos_ProjectContentComponent.h"

//==============================================================================
HeaderComponent::HeaderComponent (ProjectContentComponent* pcc)
    : projectContentComponent (pcc)
{
    setTitle ("Header");
    setFocusContainerType (FocusContainerType::focusContainer);

    addAndMakeVisible (configLabel);
    addAndMakeVisible (exporterBox);

    exporterBox.onChange = [this] { updateExporterButton(); };

    higgsIcon.setImage (ImageCache::getFromMemory (BinaryData::higgs_icon_png, BinaryData::higgs_icon_pngSize), RectanglePlacement::centred);
    addAndMakeVisible (higgsIcon);

    projectNameLabel.setText ({}, dontSendNotification);
    addAndMakeVisible (projectNameLabel);

    initialiseButtons();
}

//==============================================================================
void HeaderComponent::resized()
{
    auto bounds = getLocalBounds();
    configLabel.setFont (FontOptions { (float) bounds.getHeight() / 3.0f });

    {
        auto headerBounds = bounds.removeFromLeft (tabsWidth);

        const int buttonSize = 25;
        auto buttonBounds = headerBounds.removeFromRight (buttonSize);

        projectSettingsButton.setBounds (buttonBounds.removeFromBottom (buttonSize).reduced (2));

        higgsIcon.setBounds (headerBounds.removeFromLeft (headerBounds.getHeight()).reduced (2));

        headerBounds.removeFromRight (5);
        projectNameLabel.setBounds (headerBounds);
    }

    {
        auto exporterWidth = jmin (400, bounds.getWidth() / 2);
        Rectangle<int> exporterBounds (0, 0, exporterWidth, bounds.getHeight());

        exporterBounds.setCentre (bounds.getCentre());

        saveAndOpenInIDEButton.setBounds (exporterBounds.removeFromRight (exporterBounds.getHeight()).reduced (2));

        exporterBounds.removeFromRight (5);
        exporterBox.setBounds (exporterBounds.removeFromBottom (roundToInt ((float) exporterBounds.getHeight() / 1.8f)));
        configLabel.setBounds (exporterBounds);
    }
}

void HeaderComponent::paint (Graphics& g)
{
    g.fillAll (findColour (backgroundColourId));
}

//==============================================================================
void HeaderComponent::setCurrentProject (Project* newProject)
{
    stopTimer();
    repaint();

    projectNameLabel.setText ({}, dontSendNotification);

    project = newProject;

    if (project != nullptr)
    {
        exportersTree = project->getExporters();
        exportersTree.addListener (this);
        updateExporters();

        projectNameValue.referTo (project->getProjectValue (Ids::name));
        projectNameValue.addListener (this);
        updateName();
    }
}

//==============================================================================
void HeaderComponent::updateExporters()
{
    auto selectedExporter = getSelectedExporter();

    exporterBox.clear();

    int i = 0;

    for (Project::ExporterIterator exporter (*project); exporter.next(); ++i)
    {
        const auto exporterName = exporter->getUniqueName();
        const auto id = i + 1;
        exporterBox.addItem (exporterName, id);

        if (selectedExporter != nullptr && exporterName == selectedExporter->getUniqueName())
            exporterBox.setSelectedId (id);
    }

    const auto preferredExporterIndex = std::invoke ([&]
    {
        std::vector<ProjectExporter::ExporterTypeInfo> infos;
        ProjectExporter::getCurrentPlatformExporterTypeInfos (infos);

        for (const auto& info : infos)
        {
            int index = 0;

            for (Project::ExporterIterator exporter (*project); exporter.next(); ++index)
            {
                if (exporter->getUniqueName().contains (info.displayName))
                    return index;
            }
        }

        if (exporterBox.getSelectedItemIndex() == -1)
        {
            int index = 0;

            for (Project::ExporterIterator exporter (*project); exporter.next(); ++index)
            {
                if (exporter->canLaunchProject())
                    return index;
            }
        }

        return -1;
    });

    if (exporterBox.getSelectedItemIndex() == -1)
        exporterBox.setSelectedItemIndex (preferredExporterIndex != -1 ? preferredExporterIndex : 0);

    updateExporterButton();
}

std::unique_ptr<ProjectExporter> HeaderComponent::getSelectedExporter() const
{
    if (project != nullptr)
    {
        int i = 0;
        auto selectedIndex = exporterBox.getSelectedItemIndex();

        for (Project::ExporterIterator exporter (*project); exporter.next();)
            if (i++ == selectedIndex)
                return std::move (exporter.exporter);
    }

    return nullptr;
}

bool HeaderComponent::canCurrentExporterLaunchProject() const
{
    if (project != nullptr)
    {
        if (auto selectedExporter = getSelectedExporter())
        {
            for (Project::ExporterIterator exporter (*project); exporter.next();)
                if (exporter->canLaunchProject() && exporter->getUniqueName() == selectedExporter->getUniqueName())
                    return true;
        }
    }

    return false;
}

//==============================================================================
void HeaderComponent::sidebarTabsWidthChanged (int newWidth)
{
    tabsWidth = newWidth;
    resized();
}

void HeaderComponent::valueChanged (Value&)
{
    updateName();
}

void HeaderComponent::timerCallback()
{
    repaint();
}

//==============================================================================
void HeaderComponent::initialiseButtons()
{
    addAndMakeVisible (projectSettingsButton);
    projectSettingsButton.onClick = [this] { projectContentComponent->showProjectSettings(); };

    addAndMakeVisible (saveAndOpenInIDEButton);
    saveAndOpenInIDEButton.setBackgroundColour (Colours::white);
    saveAndOpenInIDEButton.setIconInset (7);
    saveAndOpenInIDEButton.onClick = [this]
    {
        if (project == nullptr)
            return;

        if (! project->isSaveAndExportDisabled())
        {
            projectContentComponent->openInSelectedIDE (true);
            return;
        }

        auto setWarningVisible = [this] (const Identifier& identifier)
        {
            auto child = project->getProjectMessages().getChildWithName (ProjectMessages::Ids::warning)
                                                      .getChildWithName (identifier);

            if (child.isValid())
                child.setProperty (ProjectMessages::Ids::isVisible, true, nullptr);
        };

        if (project->isFileModificationCheckPending())
            setWarningVisible (ProjectMessages::Ids::mythosFileModified);
    };

    updateExporterButton();
}

void HeaderComponent::updateName()
{
    if (project != nullptr)
        projectNameLabel.setText (project->getDocumentTitle(), dontSendNotification);
}

void HeaderComponent::updateExporterButton()
{
    if (auto selectedExporter = getSelectedExporter())
    {
        auto selectedName = selectedExporter->getUniqueName();

        for (auto info : ProjectExporter::getExporterTypeInfos())
        {
            if (selectedName.contains (info.displayName))
            {
                saveAndOpenInIDEButton.setImage (info.icon);
                saveAndOpenInIDEButton.repaint();
                saveAndOpenInIDEButton.setEnabled (canCurrentExporterLaunchProject());
            }
        }
    }
}
