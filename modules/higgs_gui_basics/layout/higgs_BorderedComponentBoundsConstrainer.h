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
    A ComponentBoundsConstrainer that can be used to add a constant border onto another
    ComponentBoundsConstrainer.

    This is useful when trying to constrain the size of a resizable window or
    other component that wraps a constrained component, such as a plugin
    editor.

    @see ResizableCornerComponent, ResizableBorderComponent, ResizableWindow,
         ComponentBoundsConstrainer

    @tags{GUI}
*/
class HIGGS_API  BorderedComponentBoundsConstrainer : public ComponentBoundsConstrainer
{
public:
    /** Default constructor. */
    BorderedComponentBoundsConstrainer() = default;

    /** Returns a pointer to another constrainer that will be used as the
        base for any resizing operations.
    */
    virtual ComponentBoundsConstrainer* getWrappedConstrainer() const = 0;

    /** Returns the border that should be applied to the constrained bounds. */
    virtual BorderSize<int> getAdditionalBorder() const = 0;

    /** @internal */
    void checkBounds (Rectangle<int>& bounds,
                      const Rectangle<int>& previousBounds,
                      const Rectangle<int>& limits,
                      bool isStretchingTop,
                      bool isStretchingLeft,
                      bool isStretchingBottom,
                      bool isStretchingRight) override;

    HIGGS_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (BorderedComponentBoundsConstrainer)
};

} // namespace higgs
