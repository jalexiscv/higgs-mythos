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

struct Direct2DImageContext::ImagePimpl : public Direct2DGraphicsContext::Pimpl
{
public:
    ImagePimpl (Direct2DImageContext& ownerIn,
                ComSmartPtr<ID2D1DeviceContext1> contextIn,
                ComSmartPtr<ID2D1Bitmap1> bitmapIn,
                const RectangleList<int>& paintAreasIn)
        : Pimpl (ownerIn),
          context (std::move (contextIn)),
          bitmap (std::move (bitmapIn)),
          paintAreas (paintAreasIn)
    {
    }

    Rectangle<int> getFrameSize() const override
    {
        if (bitmap == nullptr)
            return {};

        const auto size = bitmap->GetSize();
        return { (int) size.width, (int) size.height };
    }

    ComSmartPtr<ID2D1DeviceContext1> getDeviceContext() const override
    {
        return context;
    }

    ComSmartPtr<ID2D1Image> getDeviceContextTarget() const override
    {
        return bitmap;
    }

    RectangleList<int> getPaintAreas() const override
    {
        return paintAreas;
    }

private:
    ComSmartPtr<ID2D1DeviceContext1> context;
    ComSmartPtr<ID2D1Bitmap1> bitmap;
    RectangleList<int> paintAreas;

    HIGGS_DECLARE_WEAK_REFERENCEABLE (ImagePimpl)
};

//==============================================================================
Direct2DImageContext::Direct2DImageContext (ComSmartPtr<ID2D1DeviceContext1> context,
                                            ComSmartPtr<ID2D1Bitmap1> bitmap,
                                            const RectangleList<int>& paintAreas)
    : pimpl (new ImagePimpl { *this, context, bitmap, paintAreas })
{
   #if HIGGS_DIRECT2D_METRICS
    metrics = Direct2DMetricsHub::getInstance()->imageContextMetrics;
   #endif
}

Direct2DImageContext::~Direct2DImageContext() = default;

ComSmartPtr<ID2D1DeviceContext1> Direct2DImageContext::getDeviceContext() const
{
    return getPimpl()->getDeviceContext();
}

Direct2DGraphicsContext::Pimpl* Direct2DImageContext::getPimpl() const noexcept
{
    return pimpl.get();
}

void Direct2DImageContext::clearTargetBuffer()
{
    // The bitmap was already cleared when it was created; do nothing here
}

} // namespace higgs
