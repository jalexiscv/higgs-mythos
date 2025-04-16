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

#include "../mythos_Headers.h"
#include "../mythos_Application.h"

#include "mythos_StartPageComponent.h"
#include "mythos_StartPageTreeHolder.h"
#include "mythos_NewProjectTemplates.h"
#include "mythos_ContentComponents.h"

//==============================================================================
struct ContentComponent final : public Component
{
    ContentComponent()
    {
        setTitle ("Content");
        setFocusContainerType (FocusContainerType::focusContainer);
    }

    void resized() override
    {
        if (content != nullptr)
            content->setBounds (getLocalBounds());
    }

    void setContent (std::unique_ptr<Component>&& newContent)
    {
        if (content.get() != newContent.get())
        {
            content = std::move (newContent);
            addAndMakeVisible (content.get());
            resized();
        }
    }

private:
    std::unique_ptr<Component> content;

    //==============================================================================
    HIGGS_LEAK_DETECTOR (ContentComponent)
};

//==============================================================================
static File findExampleFile (int dirIndex, int index)
{
    auto dir = MythosStudioApplication::getSortedExampleDirectories()[dirIndex];
    return MythosStudioApplication::getSortedExampleFilesInDirectory (dir)[index];
}

static std::unique_ptr<Component> createExampleProjectsTab (ContentComponent& content, std::function<void (const File&)> cb)
{
    StringArray exampleCategories;
    std::vector<StringArray> examples;

    for (auto& dir : MythosStudioApplication::getSortedExampleDirectories())
    {
        exampleCategories.add (dir.getFileName());

        StringArray ex;
        for (auto& f : MythosStudioApplication::getSortedExampleFilesInDirectory (dir))
            ex.add (f.getFileNameWithoutExtension());

        examples.push_back (ex);
    }

    if (exampleCategories.isEmpty())
        return nullptr;

    auto selectedCallback = [&, cb] (int category, int index) mutable
    {
        content.setContent (std::make_unique<ExampleComponent> (findExampleFile (category, index), cb));
    };

    return std::make_unique<StartPageTreeHolder> ("Examples",
                                                  exampleCategories,
                                                  examples,
                                                  std::move (selectedCallback),
                                                  StartPageTreeHolder::Open::no);
}

//==============================================================================
static StringArray getAllTemplateCategoryStrings()
{
    StringArray categories;

    for (auto& t : NewProjectTemplates::getAllTemplates())
        categories.addIfNotAlreadyThere (NewProjectTemplates::getProjectCategoryString (t.category));

    return categories;
}

static std::vector<NewProjectTemplates::ProjectTemplate> getTemplatesInCategory (const String& category)
{
    std::vector<NewProjectTemplates::ProjectTemplate> templates;

    for (auto& t : NewProjectTemplates::getAllTemplates())
        if (NewProjectTemplates::getProjectCategoryString (t.category) == category)
            templates.push_back (t);

    return templates;
}

static StringArray getAllTemplateNamesForCategory (const String& category)
{
    StringArray types;

    for (auto& t : getTemplatesInCategory (category))
        types.add (t.displayName);

    return types;
}

static std::unique_ptr<Component> createProjectTemplatesTab (ContentComponent& content,
                                                             std::function<void (std::unique_ptr<Project>&&)>&& cb)
{
    auto categories = getAllTemplateCategoryStrings();

    std::vector<StringArray> templateNames;

    for (auto& c : categories)
        templateNames.push_back (getAllTemplateNamesForCategory (c));

    auto selectedCallback = [&, cb] (int category, int index)
    {
        auto categoryString = getAllTemplateCategoryStrings()[category];
        auto templates = getTemplatesInCategory (categoryString);

        content.setContent (std::make_unique<TemplateComponent> (templates[(size_t) index], std::move (cb)));
    };

    auto holder = std::make_unique<StartPageTreeHolder> ("Templates",
                                                         categories,
                                                         templateNames,
                                                         std::move (selectedCallback),
                                                         StartPageTreeHolder::Open::yes);
    holder->setSelectedItem (categories[0], 1);

    HIGGS_BEGIN_IGNORE_WARNINGS_GCC_LIKE ("-Wredundant-move")
    return std::move (holder);
    HIGGS_END_IGNORE_WARNINGS_GCC_LIKE
}

//==============================================================================
struct ProjectTemplatesAndExamples final : public TabbedComponent
{
    ProjectTemplatesAndExamples (ContentComponent& c,
                                 std::function<void (std::unique_ptr<Project>&&)>&& newProjectCb,
                                 std::function<void (const File&)>&& exampleCb)
        : TabbedComponent (TabbedButtonBar::Orientation::TabsAtTop),
          content (c),
          exampleSelectedCallback (std::move (exampleCb))
    {
        setTitle ("Templates and Examples");
        setFocusContainerType (FocusContainerType::focusContainer);

        addTab ("New Project",
                Colours::transparentBlack,
                createProjectTemplatesTab (content, std::move (newProjectCb)).release(),
                true);

        refreshExamplesTab();
    }

    void refreshExamplesTab()
    {
        auto wasOpen = (getCurrentTabIndex() == 1);

        removeTab (1);

        auto exampleTabs = createExampleProjectsTab (content, exampleSelectedCallback);

        addTab ("Open Example",
                Colours::transparentBlack,
                exampleTabs == nullptr ? new SetHIGGSPathComponent (*this) : exampleTabs.release(),
                true);

        if (wasOpen)
            setCurrentTabIndex (1);
    }

private:
    //==============================================================================
    struct SetHIGGSPathComponent final : public Component,
                                        private ChangeListener
    {
        explicit SetHIGGSPathComponent (ProjectTemplatesAndExamples& o)
            : owner (o)
        {
            getGlobalProperties().addChangeListener (this);

            setPathButton.setButtonText ("Set path to HIGGS...");
            setPathButton.onClick = [] { MythosStudioApplication::getApp().showPathsWindow (true); };

            addAndMakeVisible (setPathButton);
        }

        ~SetHIGGSPathComponent() override
        {
            getGlobalProperties().removeChangeListener (this);
        }

        void paint (Graphics& g) override
        {
            g.fillAll (findColour (secondaryBackgroundColourId));
        }

        void resized() override
        {
            auto bounds = getLocalBounds().reduced (5);
            bounds.removeFromTop (25);

            setPathButton.setBounds (bounds.removeFromTop (25));
        }

    private:
        void changeListenerCallback (ChangeBroadcaster*) override
        {
            if (isValidHIGGSExamplesDirectory (MythosStudioApplication::getHIGGSExamplesDirectoryPathFromGlobal()))
                owner.refreshExamplesTab();
        }

        ProjectTemplatesAndExamples& owner;
        TextButton setPathButton;
    };

    ContentComponent& content;
    std::function<void (const File&)> exampleSelectedCallback;
};

//==============================================================================
StartPageComponent::StartPageComponent (std::function<void (std::unique_ptr<Project>&&)>&& newProjectCb,
                                        std::function<void (const File&)>&& exampleCb)
    : content (std::make_unique<ContentComponent>()),
      tabs (std::make_unique<ProjectTemplatesAndExamples> (*content, std::move (newProjectCb), std::move (exampleCb)))
{
    tabs->setOutline (0);
    addAndMakeVisible (*tabs);

    addAndMakeVisible (openExistingButton);
    openExistingButton.setCommandToTrigger (&MythosStudioApplication::getCommandManager(), CommandIDs::open, true);

    addAndMakeVisible (*content);

    setSize (900, 600);
}

void StartPageComponent::paint (Graphics& g)
{
    g.fillAll (findColour (backgroundColourId));
}

void StartPageComponent::resized()
{
    auto bounds = getLocalBounds().reduced (10);

    auto tabBounds = bounds.removeFromLeft (bounds.getWidth() / 3);

    openExistingButton.setBounds (tabBounds.removeFromBottom (30).reduced (10, 0));
    tabBounds.removeFromBottom (5);

    tabs->setBounds (tabBounds);
    bounds.removeFromLeft (10);

    content->setBounds (bounds);
}
