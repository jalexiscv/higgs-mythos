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
/** The set of SVG stroke attributes. */
class HIGGS_API  StrokeOptions
{
public:
    /** Returns a copy of these attributes with a new colour. */
    [[nodiscard]] StrokeOptions withColour (Colour x) const
    {
        return withMember (*this, &StrokeOptions::colour, x);
    }

    /** Returns a copy of these attributes with a new dash array. */
    [[nodiscard]] StrokeOptions withDashArray (Span<const float> x) const
    {
        std::vector<float> arr (x.begin(), x.end());
        return withMember (*this, &StrokeOptions::dashArray, std::move (arr));
    }

    /** Returns a copy of these attributes with a new dash offset. */
    [[nodiscard]] StrokeOptions withDashOffset (float x) const
    {
        return withMember (*this, &StrokeOptions::dashOffset, x);
    }

    /** Returns a copy of these attributes with a new line cap style. */
    [[nodiscard]] StrokeOptions withLineCap (PathStrokeType::EndCapStyle x) const
    {
        return withMember (*this, &StrokeOptions::lineCap, x);
    }

    /** Returns a copy of these attributes with a new line join style. */
    [[nodiscard]] StrokeOptions withLineJoin (PathStrokeType::JointStyle x) const
    {
        return withMember (*this, &StrokeOptions::lineJoin, x);
    }

    /** Returns a copy of these attributes with a new miter limit. */
    [[nodiscard]] StrokeOptions withMiterLimit (float x) const
    {
        return withMember (*this, &StrokeOptions::miterLimit, x);
    }

    /** Returns a copy of these attributes with a new stroke width. */
    [[nodiscard]] StrokeOptions withWidth (float x) const
    {
        return withMember (*this, &StrokeOptions::width, x);
    }

    /** @see withColour() */
    const auto& getColour() const          { return colour; }

    /** @see withDashArray() */
    Span<const float> getDashArray() const { return dashArray; }

    /** @see withDashOffset() */
    const auto& getDashOffset() const      { return dashOffset; }

    /** @see withLineCap() */
    const auto& getLineCap() const         { return lineCap; }

    /** @see withLineJoin() */
    const auto& getLineJoin() const        { return lineJoin; }

    /** @see withMiterLimit() */
    const auto& getMiterLimit() const      { return miterLimit; }

    /** @see withWidth() */
    const auto& getWidth() const           { return width; }

    /** Creates a PathStrokeType object based on the current attributes. */
    PathStrokeType createStrokeType() const
    {
        PathStrokeType type { width, lineJoin, lineCap };
        type.setMiterLimit (miterLimit);
        return type;
    }

    /** Equality operator. */
    bool operator== (const StrokeOptions& other) const noexcept;

    /** Inequality operator. */
    bool operator!= (const StrokeOptions& other) const noexcept   { return ! operator== (other); }

private:
    Colour colour = Colours::black;  // A combination of the 'stroke' and 'opacity' attributes.
    std::vector<float> dashArray;
    float dashOffset{};
    PathStrokeType::EndCapStyle lineCap = PathStrokeType::EndCapStyle::butt;
    PathStrokeType::JointStyle lineJoin = PathStrokeType::JointStyle::mitered;
    float miterLimit = 4.0f;
    float width = 1.0f;
};

} // namespace higgs
