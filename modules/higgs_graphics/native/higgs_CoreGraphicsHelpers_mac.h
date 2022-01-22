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
namespace
{
    template <class RectType>
    Rectangle<int> convertToRectInt (RectType r) noexcept
    {
        return { (int) r.origin.x,
                 (int) r.origin.y,
                 (int) r.size.width,
                 (int) r.size.height };
    }

    template <class RectType>
    Rectangle<float> convertToRectFloat (RectType r) noexcept
    {
        return { (float) r.origin.x,
                 (float) r.origin.y,
                 (float) r.size.width,
                 (float) r.size.height };
    }

    template <class RectType>
    CGRect convertToCGRect (RectType r) noexcept
    {
        return CGRectMake ((CGFloat) r.getX(), (CGFloat) r.getY(), (CGFloat) r.getWidth(), (CGFloat) r.getHeight());
    }

    template <class PointType>
    Point<float> convertToPointFloat (PointType p) noexcept
    {
        return { (float) p.x, (float) p.y };
    }

    template <typename PointType>
    CGPoint convertToCGPoint (PointType p) noexcept
    {
        return CGPointMake ((CGFloat) p.x, (CGFloat) p.y);
    }

    template <class PointType>
    Point<int> roundToIntPoint (PointType p) noexcept
    {
        return { roundToInt (p.x), roundToInt (p.y) };
    }

   #if HIGGS_MAC
    inline CGFloat getMainScreenHeight() noexcept
    {
        if ([[NSScreen screens] count] == 0)
            return 0.0f;

        return [[[NSScreen screens] objectAtIndex: 0] frame].size.height;
    }

    inline NSRect flippedScreenRect (NSRect r) noexcept
    {
        r.origin.y = getMainScreenHeight() - (r.origin.y + r.size.height);
        return r;
    }

    inline NSPoint flippedScreenPoint (NSPoint p) noexcept
    {
        p.y = getMainScreenHeight() - p.y;
        return p;
    }
   #endif
}

CGImageRef higgs_createCoreGraphicsImage (const Image&, CGColorSpaceRef);
CGContextRef higgs_getImageContext (const Image&);

#if HIGGS_IOS
 Image higgs_createImageFromUIImage (UIImage*);
#endif

#if HIGGS_MAC
 NSImage* imageToNSImage (const ScaledImage& image);
#endif

} // namespace higgs
