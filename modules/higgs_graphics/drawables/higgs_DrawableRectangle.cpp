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

std::unique_ptr<Drawable> DrawableRectangle::createCopy() const
{
    return std::make_unique<DrawableRectangle> (*this);
}

//==============================================================================
void DrawableRectangle::setRectangle (Parallelogram<float> newBounds)
{
    if (std::exchange (bounds, newBounds) != newBounds)
        rebuildPath();
}

void DrawableRectangle::setCornerSize (Point<float> newSize)
{
    if (std::exchange (cornerSize, newSize) != newSize)
        rebuildPath();
}

void DrawableRectangle::rebuildPath()
{
    auto w = bounds.getWidth();
    auto h = bounds.getHeight();

    Path newPath;

    if (cornerSize.x > 0 && cornerSize.y > 0)
        newPath.addRoundedRectangle (0, 0, w, h, cornerSize.x, cornerSize.y);
    else
        newPath.addRectangle (0, 0, w, h);

    newPath.applyTransform (AffineTransform::fromTargetPoints (Point<float>(),
                                                               bounds.topLeft,
                                                               Point<float> (w, 0),
                                                               bounds.topRight,
                                                               Point<float> (0, h),
                                                               bounds.bottomLeft));

    if (path != newPath)
    {
        path.swapWithPath (newPath);
        pathChanged();
    }
}

} // namespace higgs
