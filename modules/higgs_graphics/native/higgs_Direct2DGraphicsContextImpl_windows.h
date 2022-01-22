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

struct Direct2DGraphicsContext::Pimpl : private DxgiAdapterListener
{
public:
    explicit Pimpl (Direct2DGraphicsContext& ownerIn);
    ~Pimpl() override;

    virtual SavedState* startFrame();
    virtual HRESULT finishFrame();

    virtual bool prepare();
    virtual void teardown();
    virtual bool checkPaintReady();

    virtual RectangleList<int> getPaintAreas() const = 0;
    virtual Rectangle<int> getFrameSize() const = 0;
    virtual ComSmartPtr<ID2D1DeviceContext1> getDeviceContext() const = 0;
    virtual ComSmartPtr<ID2D1Image> getDeviceContextTarget() const = 0;

    SavedState* getCurrentSavedState() const;
    SavedState* pushFirstSavedState (Rectangle<int> initialClipRegion);

    SavedState* pushSavedState();
    SavedState* popSavedState();

    void popAllSavedStates();

    void setDeviceContextTransform (AffineTransform transform);
    void resetDeviceContextTransform();

    DxgiAdapter::Ptr getDefaultAdapter() const
    {
        return directX->adapters.getDefaultAdapter();
    }

    auto getDirect2DFactory() const
    {
        return directX->getD2DFactory();
    }

    auto getDirectWriteFactory() const
    {
        return directWrite->getDWriteFactory();
    }

    auto getDirectWriteFactory4() const
    {
        return directWrite->getDWriteFactory4();
    }

    auto& getFontCollection() const
    {
        return directWrite->getFonts();
    }

    uint64_t getFrameId() const
    {
        return owner.getFrameId();
    }

    Direct2DMetrics::Ptr getMetrics() const
    {
        return owner.metrics;
    }

    bool fillSpriteBatch (const RectangleList<float>& list);

    virtual ComSmartPtr<IDWriteRenderingParams> getDefaultTextRenderingParams() const
    {
        ComSmartPtr<IDWriteRenderingParams> result;
        getDirectWriteFactory()->CreateRenderingParams (result.resetAndGetPointerAddress());
        return result;
    }

    DirectWriteGlyphRun glyphRun;

private:
    static void resetTransform (ID2D1DeviceContext1* context);
    static void setTransform (ID2D1DeviceContext1* context, AffineTransform newTransform);

    DxgiAdapter::Ptr findAdapter() const;

    void adapterCreated (DxgiAdapter::Ptr newAdapter) override;
    void adapterRemoved (DxgiAdapter::Ptr expiringAdapter) override;

    Direct2DGraphicsContext& owner;
    SharedResourcePointer<DirectX> directX;
    SharedResourcePointer<Direct2DFactories> directWrite;

    std::optional<Direct2DDeviceResources> deviceResources;

    std::vector<std::unique_ptr<Direct2DGraphicsContext::SavedState>> savedClientStates;

   #if HIGGS_DIRECT2D_METRICS
    int64 paintStartTicks = 0;
   #endif

    HIGGS_DECLARE_WEAK_REFERENCEABLE (Pimpl)
};

} // namespace higgs
