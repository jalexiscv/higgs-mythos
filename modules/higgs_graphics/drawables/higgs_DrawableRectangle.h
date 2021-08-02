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
    A Drawable object which draws a rectangle.

    For details on how to change the fill and stroke, see the DrawableShape class.

    @see Drawable, DrawableShape

    @tags{GUI}
*/
class HIGGS_API  DrawableRectangle  : public DrawableShape
{
public:
    //==============================================================================
    DrawableRectangle() = default;

    //==============================================================================
    /** Sets the rectangle's bounds. */
    void setRectangle (Parallelogram<float> newBounds);

    /** Returns the rectangle's bounds. */
    Parallelogram<float> getRectangle() const noexcept              { return bounds; }

    /** Returns the corner size to be used. */
    Point<float> getCornerSize() const noexcept                     { return cornerSize; }

    /** Sets a new corner size for the rectangle */
    void setCornerSize (Point<float> newSize);

    //==============================================================================
    /** @internal */
    std::unique_ptr<Drawable> createCopy() const override;

private:
    Parallelogram<float> bounds;
    Point<float> cornerSize;

    void rebuildPath();

    HIGGS_LEAK_DETECTOR (DrawableRectangle)
};

} // namespace higgs
