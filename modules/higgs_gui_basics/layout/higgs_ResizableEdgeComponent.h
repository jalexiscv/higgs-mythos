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
    A component that resizes its parent component when dragged.

    This component forms a bar along one edge of a component, allowing it to
    be dragged by that edges to resize it.

    To use it, just add it to your component, positioning it along the appropriate
    edge. Make sure you reposition the resizer component each time the parent's size
    changes, to keep it in the correct position.

    @see ResizableBorderComponent, ResizableCornerComponent

    @tags{GUI}
*/
class HIGGS_API  ResizableEdgeComponent  : public Component
{
public:
    //==============================================================================
    enum Edge
    {
        leftEdge,   /**< Indicates a vertical bar that can be dragged left/right to move the component's left-hand edge. */
        rightEdge,  /**< Indicates a vertical bar that can be dragged left/right to move the component's right-hand edge. */
        topEdge,    /**< Indicates a horizontal bar that can be dragged up/down to move the top of the component. */
        bottomEdge  /**< Indicates a horizontal bar that can be dragged up/down to move the bottom of the component. */
    };

    /** Creates a resizer bar.

        Pass in the target component which you want to be resized when this one is
        dragged. The target component will usually be this component's parent, but this
        isn't mandatory.

        Remember that when the target component is resized, it'll need to move and
        resize this component to keep it in place, as this won't happen automatically.

        If the constrainer parameter is not a nullptr, then this object will be used to
        enforce limits on the size and position that the component can be stretched to.
        Make sure that the constrainer isn't deleted while still in use by this object.

        @see ComponentBoundsConstrainer
    */
    ResizableEdgeComponent (Component* componentToResize,
                            ComponentBoundsConstrainer* constrainer,
                            Edge edgeToResize);

    /** Destructor. */
    ~ResizableEdgeComponent() override;

    bool isVertical() const noexcept;

protected:
    //==============================================================================
    /** @internal */
    void paint (Graphics&) override;
    /** @internal */
    void mouseDown (const MouseEvent&) override;
    /** @internal */
    void mouseDrag (const MouseEvent&) override;
    /** @internal */
    void mouseUp (const MouseEvent&) override;

private:
    WeakReference<Component> component;
    ComponentBoundsConstrainer* constrainer;
    Rectangle<int> originalBounds;
    const Edge edge;

    HIGGS_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ResizableEdgeComponent)
};

} // namespace higgs
