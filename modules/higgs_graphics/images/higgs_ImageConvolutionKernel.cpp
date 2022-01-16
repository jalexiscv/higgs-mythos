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

ImageConvolutionKernel::ImageConvolutionKernel (int sizeToUse)
    : values ((size_t) (sizeToUse * sizeToUse)),
      size (sizeToUse)
{
    clear();
}

ImageConvolutionKernel::~ImageConvolutionKernel() = default;

//==============================================================================
float ImageConvolutionKernel::getKernelValue (const int x, const int y) const noexcept
{
    if (isPositiveAndBelow (x, size) && isPositiveAndBelow (y, size))
        return values [x + y * size];

    jassertfalse;
    return 0;
}

void ImageConvolutionKernel::setKernelValue (const int x, const int y, const float value) noexcept
{
    if (isPositiveAndBelow (x, size) && isPositiveAndBelow (y, size))
    {
        values [x + y * size] = value;
    }
    else
    {
        jassertfalse;
    }
}

void ImageConvolutionKernel::clear()
{
    for (int i = size * size; --i >= 0;)
        values[i] = 0;
}

void ImageConvolutionKernel::setOverallSum (const float desiredTotalSum)
{
    double currentTotal = 0.0;

    for (int i = size * size; --i >= 0;)
        currentTotal += values[i];

    rescaleAllValues ((float) (desiredTotalSum / currentTotal));
}

void ImageConvolutionKernel::rescaleAllValues (const float multiplier)
{
    for (int i = size * size; --i >= 0;)
        values[i] *= multiplier;
}

//==============================================================================
void ImageConvolutionKernel::createGaussianBlur (const float radius)
{
    const double radiusFactor = -1.0 / (radius * radius * 2);
    const int centre = size >> 1;

    for (int y = size; --y >= 0;)
    {
        for (int x = size; --x >= 0;)
        {
            auto cx = x - centre;
            auto cy = y - centre;

            values [x + y * size] = (float) std::exp (radiusFactor * (cx * cx + cy * cy));
        }
    }

    setOverallSum (1.0f);
}

//==============================================================================
void ImageConvolutionKernel::applyToImage (Image& destImage,
                                           const Image& sourceImage,
                                           const Rectangle<int>& destinationArea) const
{
    if (sourceImage == destImage)
    {
        destImage.duplicateIfShared();
    }
    else
    {
        if (sourceImage.getWidth() != destImage.getWidth()
             || sourceImage.getHeight() != destImage.getHeight()
             || sourceImage.getFormat() != destImage.getFormat())
        {
            jassertfalse;
            return;
        }
    }

    auto area = destinationArea.getIntersection (destImage.getBounds());

    if (area.isEmpty())
        return;

    auto right = area.getRight();
    auto bottom = area.getBottom();

    const Image::BitmapData destData (destImage, area.getX(), area.getY(), area.getWidth(), area.getHeight(),
                                      Image::BitmapData::writeOnly);
    uint8* line = destData.data;

    const Image::BitmapData srcData (sourceImage, Image::BitmapData::readOnly);

    const auto applyKernel = [&] (auto stride)
    {
        constexpr auto pixelStride = stride.value;

        for (int y = area.getY(); y < bottom; ++y)
        {
            uint8* dest = line;
            line += destData.lineStride;

            for (int x = area.getX(); x < right; ++x)
            {
                float sum[pixelStride]{};

                for (int yy = 0; yy < size; ++yy)
                {
                    const int sy = y + yy - (size >> 1);

                    if (sy >= srcData.height)
                        break;

                    if (sy < 0)
                        continue;

                    int sx = x - (size >> 1);
                    const uint8* src = srcData.getPixelPointer (jmax (0, sx), sy);

                    for (int xx = 0; xx < size; ++xx, ++sx)
                    {
                        if (sx >= srcData.width)
                            break;

                        if (sx < 0)
                            continue;

                        const auto kernelMult = values[xx + yy * size];

                        for (auto& s : sum)
                            s += kernelMult * *src++;
                    }
                }

                for (const auto& s : sum)
                    *dest++ = (uint8) jmin (0xff, roundToInt (s));
            }
        }
    };

    switch (destData.pixelStride)
    {
        case 4:
            return applyKernel (std::integral_constant<size_t, 4>{});
        case 3:
            return applyKernel (std::integral_constant<size_t, 3>{});
        case 1:
            return applyKernel (std::integral_constant<size_t, 1>{});
    }
}

} // namespace higgs
