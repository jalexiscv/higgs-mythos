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
    Defines a drop-shadow effect.

    @tags{Graphics}
*/
struct HIGGS_API  DropShadow
{
    /** Creates a default drop-shadow effect. */
    DropShadow() = default;

    /** Creates a drop-shadow object with the given parameters. */
    DropShadow (Colour shadowColour, int radius, Point<int> offset) noexcept;

    /** Renders a drop-shadow based on the alpha-channel of the given image. */
    void drawForImage (Graphics& g, const Image& srcImage) const;

    /** Renders a drop-shadow based on the shape of a path. */
    void drawForPath (Graphics& g, const Path& path) const;

    /** Renders a drop-shadow for a rectangle.
        Note that for speed, this approximates the shadow using gradients.
    */
    void drawForRectangle (Graphics& g, const Rectangle<int>& area) const;

    /** The colour with which to render the shadow.
        In most cases you'll probably want to leave this as black with an alpha
        value of around 0.5
    */
    Colour colour { 0x90000000 };

    /** The approximate spread of the shadow. */
    int radius { 4 };

    /** The offset of the shadow. */
    Point<int> offset;
};

//==============================================================================
/**
    An effect filter that adds a drop-shadow behind the image's content.

    (This will only work on images/components that aren't opaque, of course).

    When added to a component, this effect will draw a soft-edged
    shadow based on what gets drawn inside it. The shadow will also
    be applied to the component's children.

    For speed, this doesn't use a proper gaussian blur, but cheats by
    using a simple bilinear filter. If you need a really high-quality
    shadow, check out ImageConvolutionKernel::createGaussianBlur()

    @see Component::setComponentEffect

    @tags{Graphics}
*/
class HIGGS_API  DropShadowEffect  : public ImageEffectFilter
{
public:
    //==============================================================================
    /** Creates a default drop-shadow effect.
        To customise the shadow's appearance, use the setShadowProperties() method.
    */
    DropShadowEffect();

    /** Destructor. */
    ~DropShadowEffect() override;

    //==============================================================================
    /** Sets up parameters affecting the shadow's appearance. */
    void setShadowProperties (const DropShadow& newShadow);

    //==============================================================================
    /** @internal */
    void applyEffect (Image& sourceImage, Graphics& destContext, float scaleFactor, float alpha) override;


private:
    //==============================================================================
    DropShadow shadow;

    HIGGS_LEAK_DETECTOR (DropShadowEffect)
};

} // namespace higgs
