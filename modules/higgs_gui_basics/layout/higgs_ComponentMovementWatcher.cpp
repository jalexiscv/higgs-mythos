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

ComponentMovementWatcher::ComponentMovementWatcher (Component* const comp)
    : component (comp),
      wasShowing (comp->isShowing())
{
    jassert (component != nullptr); // can't use this with a null pointer

    component->addComponentListener (this);
    registerWithParentComps();
}

ComponentMovementWatcher::~ComponentMovementWatcher()
{
    if (component != nullptr)
        component->removeComponentListener (this);

    unregister();
}

//==============================================================================
void ComponentMovementWatcher::componentParentHierarchyChanged (Component&)
{
    if (component != nullptr && ! reentrant)
    {
        const ScopedValueSetter<bool> setter (reentrant, true);

        auto* peer = component->getPeer();
        auto peerID = peer != nullptr ? peer->getUniqueID() : 0;

        if (peerID != lastPeerID)
        {
            componentPeerChanged();

            if (component == nullptr)
                return;

            lastPeerID = peerID;
        }

        unregister();
        registerWithParentComps();

        componentMovedOrResized (*component, true, true);

        if (component != nullptr)
            componentVisibilityChanged (*component);
    }
}

void ComponentMovementWatcher::componentMovedOrResized (Component&, bool wasMoved, bool wasResized)
{
    if (component != nullptr)
    {
        if (wasMoved)
        {
            Point<int> newPos;
            auto* top = component->getTopLevelComponent();

            if (top != component)
                newPos = top->getLocalPoint (component, Point<int>());
            else
                newPos = top->getPosition();

            wasMoved = lastBounds.getPosition() != newPos;
            lastBounds.setPosition (newPos);
        }

        wasResized = (lastBounds.getWidth() != component->getWidth() || lastBounds.getHeight() != component->getHeight());
        lastBounds.setSize (component->getWidth(), component->getHeight());

        if (wasMoved || wasResized)
            componentMovedOrResized (wasMoved, wasResized);
    }
}

void ComponentMovementWatcher::componentBeingDeleted (Component& comp)
{
    registeredParentComps.removeFirstMatchingValue (&comp);

    if (component == &comp)
        unregister();
}

void ComponentMovementWatcher::componentVisibilityChanged (Component&)
{
    if (component != nullptr)
    {
        const bool isShowingNow = component->isShowing();

        if (wasShowing != isShowingNow)
        {
            wasShowing = isShowingNow;
            componentVisibilityChanged();
        }
    }
}

void ComponentMovementWatcher::registerWithParentComps()
{
    for (auto* p = component->getParentComponent(); p != nullptr; p = p->getParentComponent())
    {
        p->addComponentListener (this);
        registeredParentComps.add (p);
    }
}

void ComponentMovementWatcher::unregister()
{
    for (auto* c : registeredParentComps)
        c->removeComponentListener (this);

    registeredParentComps.clear();
}

} // namespace higgs
