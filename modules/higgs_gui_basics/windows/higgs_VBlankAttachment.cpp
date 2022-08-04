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

VBlankAttachment::VBlankAttachment (Component* c, std::function<void()> callbackIn)
    : owner (c),
      callback ([cb = std::move (callbackIn)] (auto) { cb(); })
{
    jassert (owner != nullptr && callback);

    updateOwner();
    updatePeer();
}

VBlankAttachment::VBlankAttachment (Component* c, std::function<void (double)> callbackIn)
    : owner (c),
      callback (std::move (callbackIn))
{
    jassert (owner != nullptr && callback);

    updateOwner();
    updatePeer();
}

VBlankAttachment::VBlankAttachment (VBlankAttachment&& other)
    : VBlankAttachment (other.owner, std::move (other.callback))
{
    other.cleanup();
}

VBlankAttachment& VBlankAttachment::operator= (VBlankAttachment&& other)
{
    cleanup();

    owner = other.owner;
    callback = std::move (other.callback);
    updateOwner();
    updatePeer();

    other.cleanup();

    return *this;
}

VBlankAttachment::~VBlankAttachment()
{
    cleanup();
}

void VBlankAttachment::onVBlank (double timestampSec)
{
    NullCheckedInvocation::invoke (callback, timestampSec);
}

void VBlankAttachment::componentParentHierarchyChanged (Component&)
{
    updatePeer();
}

void VBlankAttachment::updateOwner()
{
    if (auto previousLastOwner = std::exchange (lastOwner, owner); previousLastOwner != owner)
    {
        if (previousLastOwner != nullptr)
            previousLastOwner->removeComponentListener (this);

        if (owner != nullptr)
            owner->addComponentListener (this);
    }
}

void VBlankAttachment::updatePeer()
{
    if (owner != nullptr)
    {
        if (auto* peer = owner->getPeer())
        {
            peer->addVBlankListener (this);

            if (lastPeer != peer && ComponentPeer::isValidPeer (lastPeer))
                lastPeer->removeVBlankListener (this);

            lastPeer = peer;
        }
    }
    else if (auto peer = std::exchange (lastPeer, nullptr); ComponentPeer::isValidPeer (peer))
    {
        peer->removeVBlankListener (this);
    }
}

void VBlankAttachment::cleanup()
{
    owner = nullptr;
    updateOwner();
    updatePeer();
}

} // namespace higgs
