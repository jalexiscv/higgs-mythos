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
/** A simple implementation of the b2Draw class, used to draw a Box2D world.

    To use it, simply create an instance of this class in your paint() method,
    and call its render() method.

    @tags{Box2D}
*/
class Box2DRenderer   : public b2Draw

{
public:
    Box2DRenderer() noexcept;

    /** Renders the world.

        @param g        the context to render into
        @param world    the world to render
        @param box2DWorldLeft   the left coordinate of the area of the world to be drawn
        @param box2DWorldTop    the top coordinate of the area of the world to be drawn
        @param box2DWorldRight  the right coordinate of the area of the world to be drawn
        @param box2DWorldBottom the bottom coordinate of the area of the world to be drawn
        @param targetArea   the area within the target context onto which the source
                            world rectangle should be mapped
    */
    void render (Graphics& g,
                 b2World& world,
                 float box2DWorldLeft, float box2DWorldTop,
                 float box2DWorldRight, float box2DWorldBottom,
                 const Rectangle<float>& targetArea);

    // b2Draw methods:
    void DrawPolygon (const b2Vec2*, int32, const b2Color&) override;
    void DrawSolidPolygon (const b2Vec2*, int32, const b2Color&) override;
    void DrawCircle (const b2Vec2& center, float32 radius, const b2Color&) override;
    void DrawSolidCircle (const b2Vec2& center, float32 radius, const b2Vec2& axis, const b2Color&) override;
    void DrawSegment (const b2Vec2& p1, const b2Vec2& p2, const b2Color&) override;
    void DrawTransform (const b2Transform& xf) override;

    /** Converts a b2Color to a higgs Colour. */
    virtual Colour getColour (const b2Color&) const;
    /** Returns the thickness to use for drawing outlines. */
    virtual float getLineThickness() const;

protected:
    Graphics* graphics;

    HIGGS_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Box2DRenderer)
};

} // namespace higgs
