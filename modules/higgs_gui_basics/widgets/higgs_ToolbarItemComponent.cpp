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

ToolbarItemFactory::ToolbarItemFactory() {}
ToolbarItemFactory::~ToolbarItemFactory() {}

//==============================================================================
ToolbarItemComponent::ToolbarItemComponent (const int itemId_,
                                            const String& labelText,
                                            const bool isBeingUsedAsAButton_)
    : Button (labelText),
      itemId (itemId_),
      mode (normalMode),
      toolbarStyle (Toolbar::iconsOnly),
      dragOffsetX (0),
      dragOffsetY (0),
      isActive (true),
      isBeingDragged (false),
      isBeingUsedAsAButton (isBeingUsedAsAButton_)
{
    // Your item ID can't be 0!
    jassert (itemId_ != 0);
}

ToolbarItemComponent::~ToolbarItemComponent()
{
    overlayComp.reset();
}

Toolbar* ToolbarItemComponent::getToolbar() const
{
    return dynamic_cast<Toolbar*> (getParentComponent());
}

bool ToolbarItemComponent::isToolbarVertical() const
{
    const Toolbar* const t = getToolbar();
    return t != nullptr && t->isVertical();
}

void ToolbarItemComponent::setStyle (const Toolbar::ToolbarItemStyle& newStyle)
{
    if (toolbarStyle != newStyle)
    {
        toolbarStyle = newStyle;
        repaint();
        resized();
    }
}

void ToolbarItemComponent::paintButton (Graphics& g, const bool over, const bool down)
{
    if (isBeingUsedAsAButton)
        getLookAndFeel().paintToolbarButtonBackground (g, getWidth(), getHeight(),
                                                       over, down, *this);

    if (toolbarStyle != Toolbar::iconsOnly)
    {
        auto indent = contentArea.getX();
        auto y = indent;
        auto h = getHeight() - indent * 2;

        if (toolbarStyle == Toolbar::iconsWithText)
        {
            y = contentArea.getBottom() + indent / 2;
            h -= contentArea.getHeight();
        }

        getLookAndFeel().paintToolbarButtonLabel (g, indent, y, getWidth() - indent * 2, h,
                                                  getButtonText(), *this);
    }

    if (! contentArea.isEmpty())
    {
        Graphics::ScopedSaveState ss (g);

        g.reduceClipRegion (contentArea);
        g.setOrigin (contentArea.getPosition());

        paintButtonArea (g, contentArea.getWidth(), contentArea.getHeight(), over, down);
    }
}

void ToolbarItemComponent::resized()
{
    if (toolbarStyle != Toolbar::textOnly)
    {
        const int indent = jmin (proportionOfWidth (0.08f),
                                 proportionOfHeight (0.08f));

        contentArea = Rectangle<int> (indent, indent,
                                      getWidth() - indent * 2,
                                      toolbarStyle == Toolbar::iconsWithText ? proportionOfHeight (0.55f)
                                                                             : (getHeight() - indent * 2));
    }
    else
    {
        contentArea = {};
    }

    contentAreaChanged (contentArea);
}

void ToolbarItemComponent::setEditingMode (const ToolbarEditingMode newMode)
{
    if (mode != newMode)
    {
        mode = newMode;
        repaint();

        if (mode == normalMode)
        {
            overlayComp.reset();
        }
        else if (overlayComp == nullptr)
        {
            overlayComp.reset (new detail::ToolbarItemDragAndDropOverlayComponent());
            addAndMakeVisible (overlayComp.get());
            overlayComp->parentSizeChanged();
        }

        resized();
    }
}

//==============================================================================
std::unique_ptr<AccessibilityHandler> ToolbarItemComponent::createAccessibilityHandler()
{
    const auto shouldItemBeAccessible = (itemId != ToolbarItemFactory::separatorBarId
                                      && itemId != ToolbarItemFactory::spacerId
                                      && itemId != ToolbarItemFactory::flexibleSpacerId);

    if (! shouldItemBeAccessible)
        return createIgnoredAccessibilityHandler (*this);

    return std::make_unique<detail::ButtonAccessibilityHandler> (*this, AccessibilityRole::button);
}

} // namespace higgs
