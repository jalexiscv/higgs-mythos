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

namespace higgs::detail
{

/*  Class that can visually shape a Unicode string provided a list of Fonts corresponding to
    sub-ranges of the string.
*/
class HIGGS_API  ShapedText
{
public:
    using Options = ShapedTextOptions;

    ShapedText();

    explicit ShapedText (String text);

    ShapedText (String text, Options options);

    /*  Returns the text which was used to construct this object. */
    const String& getText() const;

    Span<const ShapedGlyph> getGlyphs() const;

    /*  Returns the text's codepoint range, to which the glyph under the provided index belongs.

        This range will have a length of at least one, and potentially more than one if ligatures
        are enabled.
    */
    Range<int64> getTextRange (int64 glyphIndex) const;

    bool isLtr (int64 glyphIndex) const;

    int64 getTextIndexForCaret (Point<float> p) const;

    void getGlyphRanges (Range<int64> textRange, std::vector<Range<int64>>& outRanges) const;

    RectangleList<float> getGlyphsBounds (Range<int64> glyphRange) const;

    /*  @see JustifiedText::getGlyphAnchor() */
    GlyphAnchorResult getGlyphAnchor (int64 index) const;

    /*  Returns the widths for each line, that the glyphs would require to be rendered without being
        truncated. This will or will not include the space required by trailing whitespaces in the
        line based on the ShapedTextOptions::withTrailingWhitespacesShouldFit() value.

        This value isn't affected by the Justification parameter, it just reports the amount of
        width that would be required to avoid truncation.
     */
    Span<const float> getMinimumRequiredWidthForLines() const;

    /*  @see JustifiedText::accessTogetherWith */
    template <typename Callable, typename... RangedValues>
    void accessTogetherWith (Callable&& callback, RangedValues&&... rangedValues) const
    {
        getJustifiedText().accessTogetherWith (std::forward<Callable> (callback),
                                               std::forward<RangedValues> (rangedValues)...);
    }

    /*  Draws the text. */
    void draw (const Graphics& g, AffineTransform transform) const;

    /*  @see JustifiedText::getHeight
    */
    float getHeight() const;

    int64 getNumGlyphs() const;

    const detail::RangedValues<LineMetrics>& getLineMetricsForGlyphRange() const;

    const detail::Ranges& getLineTextRanges() const;

    /*  @internal */
    const JustifiedText& getJustifiedText() const;

    /*  @internal */
    const SimpleShapedText& getSimpleShapedText() const;

private:
    class Impl;
    std::shared_ptr<Impl> impl;
};

} // namespace higgs::detail
