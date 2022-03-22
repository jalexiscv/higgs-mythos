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
    A button that contains a filled shape.

    @see Button, ImageButton, TextButton, ArrowButton

    @tags{GUI}
*/
class HIGGS_API  ShapeButton  : public Button
{
public:
    //==============================================================================
    /** Creates a ShapeButton.

        @param name             a name to give the component - see Component::setName()
        @param normalColour     the colour to fill the shape with when the mouse isn't over
        @param overColour       the colour to use when the mouse is over the shape
        @param downColour       the colour to use when the button is in the pressed-down state
    */
    ShapeButton (const String& name,
                 Colour normalColour,
                 Colour overColour,
                 Colour downColour);

    /** Destructor. */
    ~ShapeButton() override;

    //==============================================================================
    /** Sets the shape to use.

        @param newShape                 the shape to use
        @param resizeNowToFitThisShape  if true, the button will be resized to fit the shape's bounds
        @param maintainShapeProportions if true, the shape's proportions will be kept fixed when
                                        the button is resized
        @param hasDropShadow            if true, the button will be given a drop-shadow effect
    */
    void setShape (const Path& newShape,
                   bool resizeNowToFitThisShape,
                   bool maintainShapeProportions,
                   bool hasDropShadow);

    /** Set the colours to use for drawing the shape.

        @param normalColour     the colour to fill the shape with when the mouse isn't over
        @param overColour       the colour to use when the mouse is over the shape
        @param downColour       the colour to use when the button is in the pressed-down state
    */
    void setColours (Colour normalColour,
                     Colour overColour,
                     Colour downColour);

    /** Sets the colours to use for drawing the shape when the button's toggle state is 'on'. To enable this behaviour, use the
        shouldUseOnColours() method.

        @param normalColourOn   the colour to fill the shape with when the mouse isn't over and the button's toggle state is 'on'
        @param overColourOn     the colour to use when the mouse is over the shape and the button's toggle state is 'on'
        @param downColourOn     the colour to use when the button is in the pressed-down state and the button's toggle state is 'on'
     */
    void setOnColours (Colour normalColourOn,
                       Colour overColourOn,
                       Colour downColourOn);

    /** Set whether the button should use the 'on' set of colours when its toggle state is 'on'.
        By default these will be the same as the normal colours but the setOnColours method can be
        used to provide a different set of colours.
    */
    void shouldUseOnColours (bool shouldUse);

    /** Sets up an outline to draw around the shape.

        @param outlineColour        the colour to use
        @param outlineStrokeWidth   the thickness of line to draw
    */
    void setOutline (Colour outlineColour, float outlineStrokeWidth);

    /** This lets you specify a border to be left around the edge of the button when
        drawing the shape.
    */
    void setBorderSize (BorderSize<int> border);

    /** @internal */
    void paintButton (Graphics&, bool, bool) override;

private:
    //==============================================================================
    Colour normalColour,   overColour,   downColour,
           normalColourOn, overColourOn, downColourOn, outlineColour;
    bool useOnColours;
    DropShadowEffect shadow;
    Path shape;
    BorderSize<int> border;
    bool maintainShapeProportions;
    float outlineWidth;

    HIGGS_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ShapeButton)
};

} // namespace higgs
