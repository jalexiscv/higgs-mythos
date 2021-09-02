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
    A component effect that adds a coloured blur around the component's contents.

    (This will only work on non-opaque components).

    @see Component::setComponentEffect, DropShadowEffect

    @tags{Graphics}
*/
class HIGGS_API  GlowEffect  : public ImageEffectFilter
{
public:
    //==============================================================================
    /** Creates a default 'glow' effect.
        To customise its appearance, use the setGlowProperties() method.
    */
    GlowEffect();

    /** Destructor. */
    ~GlowEffect() override;

    //==============================================================================
    /** Sets the glow's radius and colour.

        The radius is how large the blur should be, and the colour is
        used to render it (for a less intense glow, lower the colour's
        opacity).
    */
    void setGlowProperties (float newRadius,
                            Colour newColour,
                            Point<int> offset = {});


    //==============================================================================
    /** @internal */
    void applyEffect (Image&, Graphics&, float scaleFactor, float alpha) override;

private:
    //==============================================================================
    float radius = 2.0f;
    Colour colour { Colours::white };
    Point<int> offset;

    HIGGS_LEAK_DETECTOR (GlowEffect)
};

} // namespace higgs
