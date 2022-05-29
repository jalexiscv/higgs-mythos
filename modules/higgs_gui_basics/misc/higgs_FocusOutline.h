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
    Adds a focus outline to a component.

    This object creates and manages a component that sits on top of a target
    component. It will track the position of the target component and will be
    brought to the front with the tracked component.

    Use the Component::setHasFocusOutline() method to indicate that a component
    should have a focus outline drawn around it, and when it receives keyboard
    focus one of these objects will be created using the
    LookAndFeel::createFocusOutlineForComponent() method. You can override this
    method in your own LookAndFeel classes to draw a custom outline if required.

    @tags{GUI}
*/
class HIGGS_API  FocusOutline  : private ComponentListener
{
public:
    //==============================================================================
    /** Defines the focus outline window properties.

        Pass an instance of one of these to the FocusOutline constructor to control
        the bounds for the outline window and how it is drawn.
    */
    struct HIGGS_API  OutlineWindowProperties
    {
        virtual ~OutlineWindowProperties() = default;

        /** Return the bounds for the outline window in screen coordinates. */
        virtual Rectangle<int> getOutlineBounds (Component& focusedComponent) = 0;

        /** This method will be called to draw the focus outline. */
        virtual void drawOutline (Graphics&, int width, int height) = 0;
    };

    //==============================================================================
    /** Creates a FocusOutline.

        Call setOwner to attach it to a component.
    */
    FocusOutline (std::unique_ptr<OutlineWindowProperties> props);

    /** Destructor. */
    ~FocusOutline() override;

    /** Attaches the outline to a component. */
    void setOwner (Component* componentToFollow);

private:
    //==============================================================================
    void componentMovedOrResized (Component&, bool, bool) override;
    void componentBroughtToFront (Component&) override;
    void componentParentHierarchyChanged (Component&) override;
    void componentVisibilityChanged (Component&) override;

    void updateOutlineWindow();
    void updateParent();

    //==============================================================================
    std::unique_ptr<OutlineWindowProperties> properties;

    WeakReference<Component> owner;
    std::unique_ptr<Component> outlineWindow;
    WeakReference<Component> lastParentComp;

    bool reentrant = false;

    //==============================================================================
    HIGGS_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FocusOutline)
};

} // namespace higgs
