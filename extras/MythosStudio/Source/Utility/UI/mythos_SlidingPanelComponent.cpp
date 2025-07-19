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

#include "../../Application/mythos_Headers.h"
#include "mythos_SlidingPanelComponent.h"

//==============================================================================
struct SlidingPanelComponent::DotButton final : public Button
{
    DotButton (SlidingPanelComponent& sp, int pageIndex)
        : Button (String()), owner (sp), index (pageIndex) {}

    void paintButton (Graphics& g, bool /*isMouseOverButton*/, bool /*isButtonDown*/) override
    {
        g.setColour (findColour (defaultButtonBackgroundColourId));
        const auto r = getLocalBounds().reduced (getWidth() / 4).toFloat();

        if (index == owner.getCurrentTabIndex())
            g.fillEllipse (r);
        else
            g.drawEllipse (r, 1.0f);
    }

    void clicked() override
    {
        owner.goToTab (index);
    }

    using Button::clicked;

    SlidingPanelComponent& owner;
    int index;
};

//==============================================================================
SlidingPanelComponent::SlidingPanelComponent()
    : currentIndex (0), dotSize (20)
{
    addAndMakeVisible (pageHolder);
}

SlidingPanelComponent::~SlidingPanelComponent()
{
}

SlidingPanelComponent::PageInfo::~PageInfo()
{
    if (shouldDelete)
        content.deleteAndZero();
}

void SlidingPanelComponent::addTab (const String& tabName,
                                    Component* const contentComponent,
                                    const bool deleteComponentWhenNotNeeded,
                                    const int insertIndex)
{
    PageInfo* page = new PageInfo();
    pages.insert (insertIndex, page);
    page->content = contentComponent;
    page->dotButton.reset (new DotButton (*this, pages.indexOf (page)));
    addAndMakeVisible (page->dotButton.get());
    page->name = tabName;
    page->shouldDelete = deleteComponentWhenNotNeeded;

    pageHolder.addAndMakeVisible (contentComponent);
    resized();
}

void SlidingPanelComponent::goToTab (int targetTabIndex)
{
    currentIndex = targetTabIndex;

    Desktop::getInstance().getAnimator()
        .animateComponent (&pageHolder, pageHolder.getBounds().withX (-targetTabIndex * getWidth()),
                           1.0f, 600, false, 0.0, 0.0);

    repaint();
}

void SlidingPanelComponent::resized()
{
    pageHolder.setBounds (-currentIndex * getWidth(), pageHolder.getPosition().y,
                          getNumTabs() * getWidth(), getHeight());

    Rectangle<int> content (getLocalBounds());

    Rectangle<int> dotHolder = content.removeFromBottom (20 + dotSize)
                                 .reduced ((content.getWidth() - dotSize * getNumTabs()) / 2, 10);

    for (int i = 0; i < getNumTabs(); ++i)
        pages.getUnchecked (i)->dotButton->setBounds (dotHolder.removeFromLeft (dotSize));

    for (int i = pages.size(); --i >= 0;)
        if (Component* c = pages.getUnchecked (i)->content)
            c->setBounds (content.translated (i * content.getWidth(), 0));
}
