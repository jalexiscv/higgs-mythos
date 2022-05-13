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

ResizableCornerComponent::ResizableCornerComponent (Component* componentToResize,
                                                    ComponentBoundsConstrainer* boundsConstrainer)
   : component (componentToResize),
     constrainer (boundsConstrainer)
{
    setRepaintsOnMouseActivity (true);
    setMouseCursor (MouseCursor::BottomRightCornerResizeCursor);
}

ResizableCornerComponent::~ResizableCornerComponent() = default;

//==============================================================================
void ResizableCornerComponent::paint (Graphics& g)
{
    getLookAndFeel().drawCornerResizer (g, getWidth(), getHeight(),
                                        isMouseOverOrDragging(),
                                        isMouseButtonDown());
}

void ResizableCornerComponent::mouseDown (const MouseEvent& e)
{
    if (component == nullptr)
    {
        jassertfalse; // You've deleted the component that this resizer is supposed to be controlling!
        return;
    }

    originalBounds = component->getBounds();

    using Zone = ResizableBorderComponent::Zone;
    const Zone zone { Zone::bottom | Zone::right };

    if (auto* peer = component->getPeer())
        if (&peer->getComponent() == component)
            peer->startHostManagedResize (peer->globalToLocal (localPointToGlobal (e.getPosition())), zone);

    if (constrainer != nullptr)
        constrainer->resizeStart();
}

void ResizableCornerComponent::mouseDrag (const MouseEvent& e)
{
    if (component == nullptr)
    {
        jassertfalse; // You've deleted the component that this resizer is supposed to be controlling!
        return;
    }

    auto r = originalBounds.withSize (originalBounds.getWidth() + e.getDistanceFromDragStartX(),
                                      originalBounds.getHeight() + e.getDistanceFromDragStartY());

    if (constrainer != nullptr)
        constrainer->setBoundsForComponent (component, r, false, false, true, true);
    else if (auto pos = component->getPositioner())
        pos->applyNewBounds (r);
    else
        component->setBounds (r);
}

void ResizableCornerComponent::mouseUp (const MouseEvent&)
{
    if (constrainer != nullptr)
        constrainer->resizeEnd();
}

bool ResizableCornerComponent::hitTest (int x, int y)
{
    if (getWidth() <= 0)
        return false;

    auto yAtX = getHeight() - (getHeight() * x / getWidth());
    return y >= yAtX - getHeight() / 4;
}

} // namespace higgs
