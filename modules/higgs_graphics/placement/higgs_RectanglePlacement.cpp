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

bool RectanglePlacement::operator== (const RectanglePlacement& other) const noexcept
{
    return flags == other.flags;
}

bool RectanglePlacement::operator!= (const RectanglePlacement& other) const noexcept
{
    return flags != other.flags;
}

void RectanglePlacement::applyTo (double& x, double& y, double& w, double& h,
                                  const double dx, const double dy, const double dw, const double dh) const noexcept
{
    if (approximatelyEqual (w, 0.0) || approximatelyEqual (h, 0.0))
        return;

    if ((flags & stretchToFit) != 0)
    {
        x = dx;
        y = dy;
        w = dw;
        h = dh;
    }
    else
    {
        double scale = (flags & fillDestination) != 0 ? jmax (dw / w, dh / h)
                                                      : jmin (dw / w, dh / h);

        if ((flags & onlyReduceInSize) != 0)
            scale = jmin (scale, 1.0);

        if ((flags & onlyIncreaseInSize) != 0)
            scale = jmax (scale, 1.0);

        w *= scale;
        h *= scale;

        if ((flags & xLeft) != 0)
            x = dx;
        else if ((flags & xRight) != 0)
            x = dx + dw - w;
        else
            x = dx + (dw - w) * 0.5;

        if ((flags & yTop) != 0)
            y = dy;
        else if ((flags & yBottom) != 0)
            y = dy + dh - h;
        else
            y = dy + (dh - h) * 0.5;
    }
}

AffineTransform RectanglePlacement::getTransformToFit (const Rectangle<float>& source, const Rectangle<float>& destination) const noexcept
{
    if (source.isEmpty())
        return AffineTransform();

    float newX = destination.getX();
    float newY = destination.getY();

    float scaleX = destination.getWidth() / source.getWidth();
    float scaleY = destination.getHeight() / source.getHeight();

    if ((flags & stretchToFit) == 0)
    {
        scaleX = (flags & fillDestination) != 0 ? jmax (scaleX, scaleY)
                                                : jmin (scaleX, scaleY);

        if ((flags & onlyReduceInSize) != 0)
            scaleX = jmin (scaleX, 1.0f);

        if ((flags & onlyIncreaseInSize) != 0)
            scaleX = jmax (scaleX, 1.0f);

        scaleY = scaleX;

        if ((flags & xRight) != 0)
            newX += destination.getWidth() - source.getWidth() * scaleX;             // right
        else if ((flags & xLeft) == 0)
            newX += (destination.getWidth() - source.getWidth() * scaleX) / 2.0f;    // centre

        if ((flags & yBottom) != 0)
            newY += destination.getHeight() - source.getHeight() * scaleX;             // bottom
        else if ((flags & yTop) == 0)
            newY += (destination.getHeight() - source.getHeight() * scaleX) / 2.0f;    // centre
    }

    return AffineTransform::translation (-source.getX(), -source.getY())
                .scaled (scaleX, scaleY)
                .translated (newX, newY);
}

} // namespace higgs
