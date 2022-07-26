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

/**
    Calls a function every time the native scale factor of a component's peer changes.

    This is used in the VST and VST3 wrappers to ensure that the editor's scale is kept in sync with
    the scale of its containing component.

    @tags{GUI}
*/
class NativeScaleFactorNotifier : private ComponentMovementWatcher,
                                  private ComponentPeer::ScaleFactorListener
{
public:
    /** Constructs an instance.

        While the instance is alive, it will listen for changes to the scale factor of the
        comp's peer, and will call onScaleChanged whenever this scale factor changes.

        @param comp             The component to observe
        @param onScaleChanged   A function that will be called when the backing scale factor changes
    */
    NativeScaleFactorNotifier (Component* comp, std::function<void (float)> onScaleChanged);
    ~NativeScaleFactorNotifier() override;

private:
    void nativeScaleFactorChanged (double newScaleFactor) override;
    void componentPeerChanged() override;

    using ComponentMovementWatcher::componentVisibilityChanged;
    void componentVisibilityChanged() override {}

    using ComponentMovementWatcher::componentMovedOrResized;
    void componentMovedOrResized (bool, bool) override {}

    ComponentPeer* peer = nullptr;
    std::function<void (float)> scaleChanged;

    HIGGS_DECLARE_NON_COPYABLE (NativeScaleFactorNotifier)
    HIGGS_DECLARE_NON_MOVEABLE (NativeScaleFactorNotifier)
};

} // namespace higgs
