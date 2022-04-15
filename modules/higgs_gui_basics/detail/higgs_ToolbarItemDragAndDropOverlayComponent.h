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

namespace higgs::detail
{

class ToolbarItemDragAndDropOverlayComponent    : public Component
{
public:
    ToolbarItemDragAndDropOverlayComponent()
    {
        setAlwaysOnTop (true);
        setRepaintsOnMouseActivity (true);
        setMouseCursor (MouseCursor::DraggingHandCursor);
    }

    void paint (Graphics& g) override
    {
        if (ToolbarItemComponent* const tc = getToolbarItemComponent())
        {
            if (isMouseOverOrDragging()
                  && tc->getEditingMode() == ToolbarItemComponent::editableOnToolbar)
            {
                g.setColour (findColour (Toolbar::editingModeOutlineColourId, true));
                g.drawRect (getLocalBounds(), jmin (2, (getWidth() - 1) / 2,
                                                       (getHeight() - 1) / 2));
            }
        }
    }

    void mouseDown (const MouseEvent& e) override
    {
        isDragging = false;

        if (ToolbarItemComponent* const tc = getToolbarItemComponent())
        {
            tc->dragOffsetX = e.x;
            tc->dragOffsetY = e.y;
        }
    }

    void mouseDrag (const MouseEvent& e) override
    {
        if (e.mouseWasDraggedSinceMouseDown() && ! isDragging)
        {
            isDragging = true;

            if (DragAndDropContainer* const dnd = DragAndDropContainer::findParentDragContainerFor (this))
            {
                dnd->startDragging (Toolbar::toolbarDragDescriptor, getParentComponent(), ScaledImage(), true, nullptr, &e.source);

                if (ToolbarItemComponent* const tc = getToolbarItemComponent())
                {
                    tc->isBeingDragged = true;

                    if (tc->getEditingMode() == ToolbarItemComponent::editableOnToolbar)
                        tc->setVisible (false);
                }
            }
        }
    }

    void mouseUp (const MouseEvent&) override
    {
        isDragging = false;

        if (ToolbarItemComponent* const tc = getToolbarItemComponent())
        {
            tc->isBeingDragged = false;

            if (Toolbar* const tb = tc->getToolbar())
                tb->updateAllItemPositions (true);
            else if (tc->getEditingMode() == ToolbarItemComponent::editableOnToolbar)
                delete tc;
        }
    }

    void parentSizeChanged() override
    {
        setBounds (0, 0, getParentWidth(), getParentHeight());
    }

private:
    //==============================================================================
    bool isDragging = false;

    ToolbarItemComponent* getToolbarItemComponent() const noexcept
    {
        return dynamic_cast<ToolbarItemComponent*> (getParentComponent());
    }

    HIGGS_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ToolbarItemDragAndDropOverlayComponent)
};

} // namespace higgs::detail
