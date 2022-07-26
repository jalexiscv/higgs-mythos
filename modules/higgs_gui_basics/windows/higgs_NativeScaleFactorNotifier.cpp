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

static void removeScaleFactorListenerFromAllPeers (ComponentPeer::ScaleFactorListener& listener)
{
     for (int i = 0; i < ComponentPeer::getNumPeers(); ++i)
         ComponentPeer::getPeer (i)->removeScaleFactorListener (&listener);
}

NativeScaleFactorNotifier::NativeScaleFactorNotifier (Component* comp, std::function<void (float)> onScaleChanged)
    : ComponentMovementWatcher (comp),
      scaleChanged (std::move (onScaleChanged))
{
    componentPeerChanged();
}

NativeScaleFactorNotifier::~NativeScaleFactorNotifier()
{
    removeScaleFactorListenerFromAllPeers (*this);
}

void NativeScaleFactorNotifier::nativeScaleFactorChanged (double newScaleFactor)
{
    NullCheckedInvocation::invoke (scaleChanged, (float) newScaleFactor);
}

void NativeScaleFactorNotifier::componentPeerChanged()
{
    removeScaleFactorListenerFromAllPeers (*this);

    if (auto* x = getComponent())
        peer = x->getPeer();

    if (auto* x = peer)
    {
        x->addScaleFactorListener (this);
        nativeScaleFactorChanged (x->getPlatformScaleFactor());
    }
}

} // namespace higgs
