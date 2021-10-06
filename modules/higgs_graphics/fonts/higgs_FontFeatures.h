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
/** Represents a single OpenType font feature.

    OpenType font features are typographic options that can be enabled or disabled
    to control text rendering behavior. Each feature is identified by a
    four-character tag (like 'liga' for standard ligatures or 'smcp' for small caps).
    These features allow fine-grained control over how text is displayed, including
    ligatures, number styles, stylistic alternates, and positioning adjustments.

    The tag must be exactly 4 characters long.

    @see FontOptions, Font

    @tags{Graphics}
*/
class HIGGS_API FontFeatureTag final
{
public:
    /** Constructs a feature from the specified tag string. */
    constexpr FontFeatureTag (const char (&string)[5])
        : tag { (uint32) string[0] << 24
              | (uint32) string[1] << 16
              | (uint32) string[2] << 8
              | (uint32) string[3] }
    {
    }

    /** Constructs a feature from the specified tag value. */
    constexpr explicit FontFeatureTag (uint32 tagValue)
        : tag (tagValue)
    {
    }

    /** Creates a new FontFeature with the specified tag. */
    [[nodiscard]] static FontFeatureTag fromString (const String& tagString);

    /** Returns a string representation of this tag. */
    String toString() const;

    /** Returns the Harfbuzz compatible OpenType tag as an unsigned 32-bit integer. */
    constexpr uint32 getTag() const { return tag; }

    /** Comparison based on tag value. */
    [[nodiscard]] constexpr bool operator<  (FontFeatureTag other) const { return tag <  other.tag; }
    /** Comparison based on tag value. */
    [[nodiscard]] constexpr bool operator<= (FontFeatureTag other) const { return tag <= other.tag; }
    /** Comparison based on tag value. */
    [[nodiscard]] constexpr bool operator>  (FontFeatureTag other) const { return tag >  other.tag; }
    /** Comparison based on tag value. */
    [[nodiscard]] constexpr bool operator>= (FontFeatureTag other) const { return tag >= other.tag; }
    /** Comparison based on tag value. */
    [[nodiscard]] constexpr bool operator== (FontFeatureTag other) const { return tag == other.tag; }
    /** Comparison based on tag value. */
    [[nodiscard]] constexpr bool operator!= (FontFeatureTag other) const { return tag != other.tag; }

private:
    FontFeatureTag() = default;
    uint32 tag;
};

/** Represents a single OpenType font feature setting.

    A font feature setting combines a FontFeatureTag with an optional value that
    controls the behavior of that feature. For example, a 'liga' (standard ligatures)
    feature can be enabled (value=1) or disabled (value=0), while other features like
    'salt' (stylistic alternates) might accept a range of values to select specific
    alternates.

    @see FontFeatureTag, FontOptions, Font

    @tags{Graphics}
*/
class HIGGS_API FontFeatureSetting final
{
    constexpr auto tie() const;

public:
    /** Common feature values for convenience. */
    /** Enable this feature (value=1). */
    static constexpr auto featureEnabled  = 1;
    /** Disable this feature (value=0). */
    static constexpr auto featureDisabled = 0;

    /** Constructs a feature setting with the specified tag and optional value. */
    constexpr FontFeatureSetting (FontFeatureTag featureTag, uint32 featureValue) noexcept
        : tag (featureTag),
          value (featureValue)
    {
    }

    [[nodiscard]] constexpr bool operator<  (const FontFeatureSetting& other) const;
    [[nodiscard]] constexpr bool operator<= (const FontFeatureSetting& other) const;
    [[nodiscard]] constexpr bool operator>  (const FontFeatureSetting& other) const;
    [[nodiscard]] constexpr bool operator>= (const FontFeatureSetting& other) const;
    [[nodiscard]] constexpr bool operator== (const FontFeatureSetting& other) const;
    [[nodiscard]] constexpr bool operator!= (const FontFeatureSetting& other) const;

    /** The OpenType feature tag. */
    FontFeatureTag tag;

    /** The value for this feature.
        Common values are 0 (featureDisabled) and 1 (featureEnabled), but some features
        support additional values for specific behaviors.
    */
    uint32 value;
};

/** Represents a single OpenType variable font setting.

    This allows for precise typographic control, enabling smooth transitions
    between different font styles without requiring separate font files for
    each variation.

    Supported settings and ranges can be queried with Typeface::getSupportedVariables.

    @see FontFeatureTag, FontOptions, Font, Typeface

    @tags{Graphics}
*/
class HIGGS_API FontVariableSetting final
{
    constexpr auto tie() const;

public:
    /** Constructs a variable font setting with the specified variable tag and value. */
    constexpr FontVariableSetting (FontFeatureTag variableTag, float variableValue) noexcept
        : tag (variableTag),
          value (variableValue)
    {
    }

    [[nodiscard]] constexpr bool operator<  (const FontVariableSetting& other) const;
    [[nodiscard]] constexpr bool operator<= (const FontVariableSetting& other) const;
    [[nodiscard]] constexpr bool operator>  (const FontVariableSetting& other) const;
    [[nodiscard]] constexpr bool operator>= (const FontVariableSetting& other) const;
    [[nodiscard]] constexpr bool operator!= (const FontVariableSetting& other) const;
    [[nodiscard]] constexpr bool operator== (const FontVariableSetting& other) const;

    /** The OpenType variation axis tag. */
    FontFeatureTag tag;

    /** The value for this axis. */
    float value;
};

} // namespace higgs
