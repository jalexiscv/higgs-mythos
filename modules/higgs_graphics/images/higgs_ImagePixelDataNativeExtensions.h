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

/*  Allows access to ImagePixelData implementation details by LowLevelGraphicsContext instances.
    The internal templating is mainly to facilitate returning a type with dynamic implementation by value.
*/
class ImagePixelDataNativeExtensions
{
public:
    template <typename Impl>
    explicit ImagePixelDataNativeExtensions (Impl x)
        : impl (std::make_unique<Concrete<Impl>> (std::move (x))) {}

    /*  For subsection images, this returns the top-left pixel inside the root image */
    Point<int> getTopLeft() const { return impl->getTopLeft(); }

   #if HIGGS_WINDOWS
    Span<const Direct2DPixelDataPage> getPages (ComSmartPtr<ID2D1Device1> x) const { return impl->getPages (x); }
   #endif

   #if HIGGS_MAC || HIGGS_IOS
    CGContextRef getCGContext() const { return impl->getCGContext(); }
    CFUniquePtr<CGImageRef> getCGImage (CGColorSpaceRef x) const { return impl->getCGImage (x); }
   #endif

private:
    struct Base
    {
        virtual ~Base() = default;
        virtual Point<int> getTopLeft() const = 0;

       #if HIGGS_WINDOWS
        virtual Span<const Direct2DPixelDataPage> getPages (ComSmartPtr<ID2D1Device1>) const = 0;
       #endif

       #if HIGGS_MAC || HIGGS_IOS
        virtual CGContextRef getCGContext() const = 0;
        virtual CFUniquePtr<CGImageRef> getCGImage (CGColorSpaceRef x) const = 0;
       #endif
    };

    template <typename Impl>
    class Concrete : public Base
    {
    public:
        explicit Concrete (Impl x)
            : impl (std::move (x)) {}

        Point<int> getTopLeft() const override { return impl.getTopLeft(); }

       #if HIGGS_WINDOWS
        Span<const Direct2DPixelDataPage> getPages (ComSmartPtr<ID2D1Device1> x) const override { return impl.getPages (x); }
       #endif

       #if HIGGS_MAC || HIGGS_IOS
        CGContextRef getCGContext() const override { return impl.getCGContext(); }
        CFUniquePtr<CGImageRef> getCGImage (CGColorSpaceRef x) const override { return impl.getCGImage (x); }
       #endif

    private:
        Impl impl;
    };

    std::unique_ptr<Base> impl;
};

} // namespace higgs
