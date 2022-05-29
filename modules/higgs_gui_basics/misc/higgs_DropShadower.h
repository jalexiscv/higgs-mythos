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
/**
    Adds a drop-shadow to a component.

    This object creates and manages a set of components which sit around a
    component, creating a gaussian shadow around it. The components will track
    the position of the component and if it's brought to the front they'll also
    follow this.

    For desktop windows you don't need to use this class directly - just
    set the Component::windowHasDropShadow flag when calling
    Component::addToDesktop(), and the system will create one of these if it's
    needed (which it obviously isn't on the Mac, for example).

    @tags{GUI}
*/
class HIGGS_API  DropShadower  : private ComponentListener
{
public:
    //==============================================================================
    /** Creates a DropShadower. */
    DropShadower (const DropShadow& shadowType);

    /** Destructor. */
    ~DropShadower() override;

    /** Attaches the DropShadower to the component you want to shadow. */
    void setOwner (Component* componentToFollow);

private:
    //==============================================================================
    void componentMovedOrResized (Component&, bool, bool) override;
    void componentBroughtToFront (Component&) override;
    void componentChildrenChanged (Component&) override;
    void componentParentHierarchyChanged (Component&) override;
    void componentVisibilityChanged (Component&) override;

    void updateParent();
    void updateShadows();

    class ShadowWindow;

    WeakReference<Component> owner;
    OwnedArray<Component> shadowWindows;
    DropShadow shadow;
    bool reentrant = false;
    WeakReference<Component> lastParentComp;

    class ParentVisibilityChangedListener;
    std::unique_ptr<ParentVisibilityChangedListener> visibilityChangedListener;

    class ScaleWatcher;
    std::unique_ptr<ScaleWatcher> scaleWatcher;

    HIGGS_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DropShadower)
    HIGGS_DECLARE_WEAK_REFERENCEABLE (DropShadower)
};

} // namespace higgs
