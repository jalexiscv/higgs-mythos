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
/**
    A lowest-common-denominator implementation of LowLevelGraphicsContext that does all
    its rendering in memory.

    User code is not supposed to create instances of this class directly - do all your
    rendering via the Graphics class instead.

    @tags{Graphics}
*/
class HIGGS_API  LowLevelGraphicsSoftwareRenderer : public LowLevelGraphicsContext
{
public:
    //==============================================================================
    /** Creates a context to render into an image. */
    LowLevelGraphicsSoftwareRenderer (const Image& imageToRenderOnto);

    /** Creates a context to render into a clipped subsection of an image. */
    LowLevelGraphicsSoftwareRenderer (const Image& imageToRenderOnto, Point<int> origin,
                                      const RectangleList<int>& initialClip);

    /** Destructor. */
    ~LowLevelGraphicsSoftwareRenderer() override;

    std::unique_ptr<ImageType> getPreferredImageTypeForTemporaryImages() const override
    {
        return std::make_unique<SoftwareImageType>();
    }

    bool isVectorDevice() const override;
    Rectangle<int> getClipBounds() const override;
    bool isClipEmpty() const override;

    void setOrigin (Point<int> o) override;
    void addTransform (const AffineTransform& t) override;
    float getPhysicalPixelScaleFactor() const override;
    bool clipRegionIntersects (const Rectangle<int>& r) override;
    bool clipToRectangle (const Rectangle<int>& r) override;
    bool clipToRectangleList (const RectangleList<int>& r) override;
    void excludeClipRectangle (const Rectangle<int>& r) override;
    void clipToPath (const Path& path, const AffineTransform& t) override;
    void clipToImageAlpha (const Image& im, const AffineTransform& t) override;
    void saveState() override;
    void restoreState() override;
    void beginTransparencyLayer (float opacity) override;
    void endTransparencyLayer() override;
    void setFill (const FillType& fillType) override;
    void setImageBlendMode (BlendMode blendMode) override;
    void setOpacity (float newOpacity) override;
    void setInterpolationQuality (Graphics::ResamplingQuality quality) override;
    void fillRect (const Rectangle<int>& r, bool replace) override;
    void fillRect (const Rectangle<float>& r) override;
    void fillRectList (const RectangleList<float>& list) override;
    void fillPath (const Path& path, const AffineTransform& t) override;
    void drawImage (const Image& im, const AffineTransform& t) override;
    void drawLine (const Line<float>& line) override;
    void setFont (const Font& newFont) override;
    const Font& getFont() override;
    uint64_t getFrameId() const override;

    void drawGlyphs (Span<const uint16_t> glyphs,
                     Span<const Point<float>> positions,
                     const AffineTransform& t) override;

private:
    class Impl;
    std::unique_ptr<Impl> impl;

    HIGGS_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (LowLevelGraphicsSoftwareRenderer)
};

} // namespace higgs
