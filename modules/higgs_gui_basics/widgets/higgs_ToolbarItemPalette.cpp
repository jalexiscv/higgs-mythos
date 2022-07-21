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

namespace higgs
{

ToolbarItemPalette::ToolbarItemPalette (ToolbarItemFactory& tbf, Toolbar& bar)
    : factory (tbf), toolbar (bar)
{
    auto* itemHolder = new Component();
    viewport.setViewedComponent (itemHolder);

    Array<int> allIds;
    factory.getAllToolbarItemIds (allIds);

    for (auto& i : allIds)
        addComponent (i, -1);

    addAndMakeVisible (viewport);
}

ToolbarItemPalette::~ToolbarItemPalette()
{
}

//==============================================================================
void ToolbarItemPalette::addComponent (const int itemId, const int index)
{
    if (auto* tc = Toolbar::createItem (factory, itemId))
    {
        items.insert (index, tc);
        viewport.getViewedComponent()->addAndMakeVisible (tc, index);
        tc->setEditingMode (ToolbarItemComponent::editableOnPalette);
    }
    else
    {
        jassertfalse;
    }
}

void ToolbarItemPalette::replaceComponent (ToolbarItemComponent& comp)
{
    auto index = items.indexOf (&comp);
    jassert (index >= 0);
    items.removeObject (&comp, false);

    addComponent (comp.getItemId(), index);
    resized();
}

void ToolbarItemPalette::resized()
{
    viewport.setBoundsInset (BorderSize<int> (1));

    auto* itemHolder = viewport.getViewedComponent();

    const int indent = 8;
    const int preferredWidth = viewport.getWidth() - viewport.getScrollBarThickness() - indent;
    const int height = toolbar.getThickness();
    auto x = indent;
    auto y = indent;
    int maxX = 0;

    for (auto* tc : items)
    {
        tc->setStyle (toolbar.getStyle());

        int preferredSize = 1, minSize = 1, maxSize = 1;

        if (tc->getToolbarItemSizes (height, false, preferredSize, minSize, maxSize))
        {
            if (x + preferredSize > preferredWidth && x > indent)
            {
                x = indent;
                y += height;
            }

            tc->setBounds (x, y, preferredSize, height);

            x += preferredSize + 8;
            maxX = jmax (maxX, x);
        }
    }

    itemHolder->setSize (maxX, y + height + 8);
}

//==============================================================================
std::unique_ptr<AccessibilityHandler> ToolbarItemPalette::createAccessibilityHandler()
{
    return std::make_unique<AccessibilityHandler> (*this, AccessibilityRole::group);
}

} // namespace higgs
