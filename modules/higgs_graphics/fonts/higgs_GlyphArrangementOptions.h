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

/** Options that can be used to affect the layout produced by GlyphArrangement::addFittedText.

    @see GlyphArrangement::addFittedText

    @tags{Graphics}
*/
class HIGGS_API GlyphArrangementOptions final
{
public:
    /** We increment the distance between the baselines of subsequent lines with this value.

        Line spacing is added below the line's descender, and doesn't affect the first line's
        baseline.

        The total distance between baselines is lineHeight * lineHeightMultiple + lineSpacing.
    */
    [[nodiscard]] GlyphArrangementOptions withLineSpacing (float x) const
    {
        return withMember (*this, &GlyphArrangementOptions::lineSpacing, x);
    }

    /** We multiply the original distance between the baselines of subsequent lines with this value.

        The line height multiple is applied to both the ascender and descender, hence it affects
        the first line's baseline.

        The total distance between baselines is lineHeight * lineHeightMultiple + lineSpacing.
    */
    [[nodiscard]] GlyphArrangementOptions withLineHeightMultiple (float x) const
    {
        return withMember (*this, &GlyphArrangementOptions::lineHeightMultiple, x);
    }

    /** @see withLineSpacing() */
    const auto& getLineSpacing() const        { return lineSpacing; }

    /** @see withLineHeightMultiple() */
    const auto& getLineHeightMultiple() const { return lineHeightMultiple; }

    /** Equality operator. */
    [[nodiscard]] bool operator== (const GlyphArrangementOptions& other) const;
    /** Inequality operator. */
    [[nodiscard]] bool operator!= (const GlyphArrangementOptions& other) const;
    /** Less-than operator. Allows GlyphArrangementOptions to be used as keys in a map. */
    [[nodiscard]] bool operator<  (const GlyphArrangementOptions& other) const;
    /** Less-than-or-equal operator. */
    [[nodiscard]] bool operator<= (const GlyphArrangementOptions& other) const;
    /** Greater-than operator. */
    [[nodiscard]] bool operator>  (const GlyphArrangementOptions& other) const;
    /** Greater-than-or-equal operator. */
    [[nodiscard]] bool operator>= (const GlyphArrangementOptions& other) const;

private:
    auto tie() const noexcept;

    float lineSpacing = 0.0f;
    float lineHeightMultiple = 1.0f;
};

} // namespace higgs
