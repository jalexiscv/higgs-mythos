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

DrawableImage::DrawableImage (const Image& imageToUse)
{
    setImageInternal (imageToUse);
}

DrawableImage::DrawableImage (const Image& imageIn,
                              Rectangle<int> destinationBoundsIn,
                              Rectangle<int> sourceBoundsIn)
    : image (imageIn),
      destinationBounds (destinationBoundsIn),
      sourceBounds (sourceBoundsIn)
{
}

std::unique_ptr<Drawable> DrawableImage::createCopy() const
{
    return std::make_unique<DrawableImage> (*this);
}

//==============================================================================
void DrawableImage::setImage (const Image& imageToUse)
{
    if (setImageInternal (imageToUse))
        callListeners();
}

void DrawableImage::setOpacity (const float newOpacity)
{
    opacity = newOpacity;
}

void DrawableImage::setOverlayColour (Colour newOverlayColour)
{
    overlayColour = newOverlayColour;
}

void DrawableImage::setBoundingBox (Rectangle<float> newBounds)
{
    setBoundingBox (Parallelogram<float> (newBounds));
}

void DrawableImage::setBoundingBox (Parallelogram<float> newBounds)
{
    if (bounds != newBounds)
    {
        bounds = newBounds;

        if (image.isValid())
        {
            auto tr = bounds.topLeft + (bounds.topRight - bounds.topLeft) / (float) image.getWidth();
            auto bl = bounds.topLeft + (bounds.bottomLeft - bounds.topLeft) / (float) image.getHeight();

            auto t = AffineTransform::fromTargetPoints (
                bounds.topLeft.x, bounds.topLeft.y, tr.x, tr.y, bl.x, bl.y);

            if (t.isSingularity())
                t = {};

            setDrawableTransform (t);
        }
    }
}

//==============================================================================
void DrawableImage::paint (Graphics& g) const
{
    const Graphics::ScopedSaveState ss (g);

    if (! image.isValid())
        return;

    g.setImageResamplingQuality (resamplingQuality);

    const auto dst = destinationBounds.isEmpty() ? image.getBounds() : destinationBounds;
    const auto src = sourceBounds.isEmpty() ? image.getBounds() : sourceBounds;

    if (opacity > 0.0f && ! overlayColour.isOpaque())
    {
        g.setOpacity (opacity);

        g.drawImage (image,
                     dst.getX(),
                     dst.getY(),
                     dst.getWidth(),
                     dst.getHeight(),
                     src.getX(),
                     src.getY(),
                     src.getWidth(),
                     src.getHeight(),
                     false);
    }

    if (! overlayColour.isTransparent())
    {
        g.setColour (overlayColour.withMultipliedAlpha (opacity));

        g.drawImage (image,
                     dst.getX(),
                     dst.getY(),
                     dst.getWidth(),
                     dst.getHeight(),
                     src.getX(),
                     src.getY(),
                     src.getWidth(),
                     src.getHeight(),
                     true);
    }
}

Rectangle<float> DrawableImage::getDrawableBoundsUntransformed() const
{
    return image.getBounds().toFloat();
}

bool DrawableImage::hitTest (Point<float> p) const
{
    const auto pInt = p.roundToInt();
    return image.isValid() && image.getPixelAt (pInt.x, pInt.y).getAlpha() >= 127;
}

Path DrawableImage::getOutlineAsPath() const
{
    return {}; // not applicable for images
}

bool DrawableImage::isImage() const
{
    return true;
}

void DrawableImage::setImageResamplingQuality (Graphics::ResamplingQuality newQuality)
{
    resamplingQuality = newQuality;
}

//==============================================================================
bool DrawableImage::setImageInternal (const Image& imageToUse)
{
    if (image != imageToUse)
    {
        image = imageToUse;
        setBoundingBox (image.getBounds().toFloat());
        return true;
    }

    return false;
}

} // namespace higgs
