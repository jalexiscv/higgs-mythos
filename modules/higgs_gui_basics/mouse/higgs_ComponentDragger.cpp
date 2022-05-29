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

//==============================================================================
void ComponentDragger::startDraggingComponent (Component* const componentToDrag, const MouseEvent& e)
{
    jassert (componentToDrag != nullptr);
    jassert (e.mods.isAnyMouseButtonDown()); // The event has to be a drag event!

    if (componentToDrag != nullptr)
        mouseDownWithinTarget = e.getEventRelativeTo (componentToDrag).getMouseDownPosition();
}

void ComponentDragger::dragComponent (Component* const componentToDrag, const MouseEvent& e,
                                      ComponentBoundsConstrainer* const constrainer)
{
    jassert (componentToDrag != nullptr);
    jassert (e.mods.isAnyMouseButtonDown()); // The event has to be a drag event!

    if (componentToDrag != nullptr)
    {
        const auto bounds = componentToDrag->getBounds();

        const auto setBounds = [&] (auto b)
        {
            if (constrainer != nullptr)
                constrainer->setBoundsForComponent (componentToDrag, b, false, false, false, false);
            else
                componentToDrag->setBounds (b);
        };

        if (auto* peer = componentToDrag->isOnDesktop() ? componentToDrag->getPeer() : nullptr)
        {
            // If the component is a window, multiple mouse events can get queued while it's in the same position,
            // so their coordinates become wrong after the first one moves the window, so in that case, we'll use
            // the current mouse position instead of the one that the event contains...

            const auto globalMouseDown = componentToDrag->localPointToGlobal (mouseDownWithinTarget.toFloat());
            const auto peerSpaceMouseDown = peer->globalToLocal (detail::ScalingHelpers::scaledScreenPosToUnscaled (globalMouseDown));
            const auto [multimonitor, logical] = detail::ComponentHelpers::getTopLeftForPeer (*peer, e.source.getScreenPosition(), peerSpaceMouseDown);
            const auto scope = peer->setMultimonitorPositionOverride (multimonitor.roundToInt());
            setBounds (bounds.withPosition (logical.roundToInt()));
        }
        else
        {
            setBounds (bounds + (e.getEventRelativeTo (componentToDrag).getPosition() - mouseDownWithinTarget));
        }
    }
}

} // namespace higgs
