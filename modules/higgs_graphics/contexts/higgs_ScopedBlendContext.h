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
class HIGGS_API ScopedBlendContextOptions
{
public:
    ScopedBlendContextOptions() = default;

    /** Creates an object using the provided blend mode. */
    explicit ScopedBlendContextOptions (BlendMode mode)
        : blendMode (mode)
    {}

    /** Specifies the blend mode to be applied when blending the backing buffer into the parent
        buffer.

        @see BlendMode
     */
    [[nodiscard]] ScopedBlendContextOptions withBlendMode (BlendMode x) const
    {
        return withMember (*this, &ScopedBlendContextOptions::blendMode, x);
    }

    /** When this option is set, the backing buffer's luminosity is converted to alpha values prior
        to blending it into the parent buffer.

        After conversion, the RGB components are zero, and the A component is calculated using the
        equation in the SVG specification A = ((0.2125 * R) + (0.7154 * G) + (0.0721 * B)) * A.
    */
    [[nodiscard]] ScopedBlendContextOptions withLuminanceMaskSet (bool x = true) const
    {
        return withMember (*this, &ScopedBlendContextOptions::luminanceMask, x);
    }

    /** Allows specifying an additional clip for the blend context, reducing the necessary size of
        the backing buffer.

        By default the backing buffer will be the same size as the parent context's clip region.
        Even if this value is provided, it is ignored for certain blending modes such as
        destinationIn and destinationOut.
    */
    [[nodiscard]] ScopedBlendContextOptions withScopeClip (Rectangle<int> x) const
    {
        return withMember (*this, &ScopedBlendContextOptions::scopeClip, x);
    }

    /** Allows specifying an additional opacity value that is applied prior to blending the buffer
        into the parent.
    */
    [[nodiscard]] ScopedBlendContextOptions withOpacity (float x) const
    {
        return withMember (*this, &ScopedBlendContextOptions::opacity, x);
    }

    /** @see withBlendMode() */
    const auto& getBlendMode() const     { return blendMode; }

    /** @see withLuminanceMaskSet() */
    const auto& isLuminanceMask() const  { return luminanceMask; }

    /** @see withScopeClip() */
    const auto& getScopeClip() const     { return scopeClip; }

    /** @see withOpacity() */
    const auto& getOpacity() const       { return opacity; }

private:
    BlendMode blendMode = BlendMode::sourceOver;
    bool luminanceMask = false;
    std::optional<Rectangle<int>> scopeClip;
    float opacity = 1.0f;
};

//==============================================================================
/** Uses RAII to create a temporary image that is blended into the parent context
    when this object goes out of scope.
*/
class HIGGS_API ScopedBlendContext
{
public:
    using Options = ScopedBlendContextOptions;

    /** Constructor.

        The image drawn into the context returned by getContext() will be blended into the context
        passed in here when this object is destroyed.

        @see ScopedBlendContextOptions
    */
    ScopedBlendContext (Graphics& g, const Options& options);

    /**  Returns the context for drawing the image that will be blended into the parent context. */
    Graphics& getContext()
    {
        return *blendContext;
    }

    /** Returns true if the clip region for this context is empty. This allows the call site to
        omit draw calls that wouldn't have a visible effect anyway.
    */
    bool isClipEmpty() const
    {
        return clipIsEmpty;
    }

    /** Destructor */
    ~ScopedBlendContext();

private:
    /*  Implements https://developer.mozilla.org/en-US/docs/Web/SVG/Reference/Attribute/mask-type#luminance
    */
    static void convertLuminanceToAlpha (Image& image);

    Graphics createBlendContext();

    Graphics& parent;
    Options options;

    Image buffer;
    Point<int> bufferPosition;
    bool clipIsEmpty = false;

    // This object needs to be destroyed before we blend its buffer into the parent in the destructor.
    std::optional<Graphics> blendContext;

    AffineTransform blendContextTransform;
};

} // namespace higgs
