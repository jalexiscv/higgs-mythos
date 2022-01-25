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

/*  A single bitmap that represents a subsection of a virtual bitmap. */
struct Direct2DPixelDataPage
{
    /*  The bounds of the stored bitmap inside the virtual bitmap. */
    Rectangle<int> getBounds() const
    {
        if (bitmap == nullptr)
            return {};

        const auto size = bitmap->GetPixelSize();
        return Rectangle { (int) size.width, (int) size.height }.withPosition (topLeft);
    }

    /*  The stored subsection bitmap. */
    ComSmartPtr<ID2D1Bitmap1> bitmap;

    /*  The top-left position of this virtual bitmap inside the virtual bitmap. */
    Point<int> topLeft;
};

} // namespace higgs
