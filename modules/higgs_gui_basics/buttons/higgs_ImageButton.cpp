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

ImageButton::ImageButton (const String& text_)
    : Button (text_),
      scaleImageToFit (true),
      preserveProportions (true),
      alphaThreshold (0)
{
}

void ImageButton::setImages (const bool resizeButtonNowToFitThisImage,
                             const bool rescaleImagesWhenButtonSizeChanges,
                             const bool preserveImageProportions,
                             const Image& normalImage_,
                             const float imageOpacityWhenNormal,
                             Colour overlayColourWhenNormal,
                             const Image& overImage_,
                             const float imageOpacityWhenOver,
                             Colour overlayColourWhenOver,
                             const Image& downImage_,
                             const float imageOpacityWhenDown,
                             Colour overlayColourWhenDown,
                             const float hitTestAlphaThreshold)
{
    normalImage = normalImage_;
    overImage = overImage_;
    downImage = downImage_;

    if (resizeButtonNowToFitThisImage && normalImage.isValid())
    {
        imageBounds.setSize (normalImage.getWidth(),
                             normalImage.getHeight());

        setSize (imageBounds.getWidth(), imageBounds.getHeight());
    }

    scaleImageToFit = rescaleImagesWhenButtonSizeChanges;
    preserveProportions = preserveImageProportions;

    normalOpacity = imageOpacityWhenNormal;
    normalOverlay = overlayColourWhenNormal;
    overOpacity   = imageOpacityWhenOver;
    overOverlay   = overlayColourWhenOver;
    downOpacity   = imageOpacityWhenDown;
    downOverlay   = overlayColourWhenDown;

    alphaThreshold = (uint8) jlimit (0, 0xff, roundToInt (255.0f * hitTestAlphaThreshold));

    repaint();
}

Image ImageButton::getCurrentImage() const
{
    if (isDown() || getToggleState())
        return getDownImage();

    if (isOver())
        return getOverImage();

    return getNormalImage();
}

Image ImageButton::getNormalImage() const
{
    return normalImage;
}

Image ImageButton::getOverImage() const
{
    return overImage.isValid() ? overImage
                               : normalImage;
}

Image ImageButton::getDownImage() const
{
    return downImage.isValid() ? downImage
                               : getOverImage();
}

void ImageButton::paintButton (Graphics& g,
                               bool shouldDrawButtonAsHighlighted,
                               bool shouldDrawButtonAsDown)
{
    g.setImageResamplingQuality (resamplingQuality);

    if (! isEnabled())
    {
        shouldDrawButtonAsHighlighted = false;
        shouldDrawButtonAsDown = false;
    }

    Image im (getCurrentImage());

    if (im.isValid())
    {
        const int iw = im.getWidth();
        const int ih = im.getHeight();
        int w = getWidth();
        int h = getHeight();
        int x = (w - iw) / 2;
        int y = (h - ih) / 2;

        if (scaleImageToFit)
        {
            if (preserveProportions)
            {
                int newW, newH;
                const float imRatio = (float) ih / (float) iw;
                const float destRatio = (float) h / (float) w;

                if (imRatio > destRatio)
                {
                    newW = roundToInt ((float) h / imRatio);
                    newH = h;
                }
                else
                {
                    newW = w;
                    newH = roundToInt ((float) w * imRatio);
                }

                x = (w - newW) / 2;
                y = (h - newH) / 2;
                w = newW;
                h = newH;
            }
            else
            {
                x = 0;
                y = 0;
            }
        }

        if (! scaleImageToFit)
        {
            w = iw;
            h = ih;
        }

        imageBounds.setBounds (x, y, w, h);

        const bool useDownImage = shouldDrawButtonAsDown || getToggleState();

        getLookAndFeel().drawImageButton (g, &im, x, y, w, h,
                                          useDownImage ? downOverlay
                                                       : (shouldDrawButtonAsHighlighted ? overOverlay
                                                                            : normalOverlay),
                                          useDownImage ? downOpacity
                                                       : (shouldDrawButtonAsHighlighted ? overOpacity
                                                                            : normalOpacity),
                                          *this);
    }
}

bool ImageButton::hitTest (int x, int y)
{
    if (! Component::hitTest (x, y)) // handle setInterceptsMouseClicks
        return false;

    if (alphaThreshold == 0)
        return true;

    Image im (getCurrentImage());

    return im.isNull() || ((! imageBounds.isEmpty())
                            && alphaThreshold < im.getPixelAt (((x - imageBounds.getX()) * im.getWidth()) / imageBounds.getWidth(),
                                                               ((y - imageBounds.getY()) * im.getHeight()) / imageBounds.getHeight()).getAlpha());
}

void ImageButton::setImageResamplingQuality (Graphics::ResamplingQuality newQuality)
{
    if (std::exchange (resamplingQuality, newQuality) != newQuality)
        repaint();
}

} // namespace higgs
