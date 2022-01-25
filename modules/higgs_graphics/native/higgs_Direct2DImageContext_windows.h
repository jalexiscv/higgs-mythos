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

class Direct2DImageContext : public Direct2DGraphicsContext
{
public:
    Direct2DImageContext (ComSmartPtr<ID2D1DeviceContext1>,
                          ComSmartPtr<ID2D1Bitmap1>,
                          const RectangleList<int>&);

    ~Direct2DImageContext() override;

    ComSmartPtr<ID2D1DeviceContext1> getDeviceContext() const;

private:
    struct ImagePimpl;
    std::unique_ptr<ImagePimpl> pimpl;

    Pimpl* getPimpl() const noexcept override;
    void clearTargetBuffer() override;

    HIGGS_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Direct2DImageContext)
};

} // namespace higgs
