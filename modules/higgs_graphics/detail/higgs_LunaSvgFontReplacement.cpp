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

/*  Very nearly a drop in replacement for lunasvg::Font. This allows us to bypass lunasvg's Font
    rendering and plug in HIGGS's Font related functionality.
*/
class LunaSvgFontReplacement
{
public:
    LunaSvgFontReplacement() = default;

    LunaSvgFontReplacement (float size, const std::string& family, bool bold, bool italic)
    {
        const auto getStyleFlags = [] (const bool b, const bool i) noexcept
        {
            int flags{};

            if (b) flags |= Font::bold;
            if (i) flags |= Font::italic;

            return flags;
        };

        font = std::invoke (
            [&]
            {
                higgs::StringArray tokens;
                tokens.addTokens (family, ",", "");
                std::vector<String> fallbacks ((size_t) tokens.size());

                std::transform (tokens.begin(),
                                tokens.end(),
                                fallbacks.begin(),
                                [] (const String& s) { return s.trim(); });

                auto options = FontOptions{}.withPointHeight (size)
                                            .withFallbacks (fallbacks)
                                            .withStyleFlags (getStyleFlags (bold, italic));

                if (! fallbacks.empty())
                    options = options.withName (fallbacks[0]);

                return Font { options };
            });

        GlyphArrangement arr;
        arr.addLineOfText (*font, "x", 0.0f, 0.0f);

        fontXHeight = std::invoke ([&]
        {
            if (arr.getNumGlyphs() == 0)
                return font->getHeight();

            auto typefacePtr = font->getTypefacePtr();

            if (typefacePtr == nullptr)
                return 0.0f;

            const auto xBoundsNormalised = typefacePtr->getGlyphBounds (arr.getGlyph (0).getGlyphIndex());
            return xBoundsNormalised.getHeight() * font->getHeightInPoints();
        });
    }

    float ascent() const
    {
        if (! font.has_value())
            return 0.0f;

        return font->getAscentInPoints();
    }

    float descent() const
    {
        if (! font.has_value())
            return 0.0f;

        return font->getDescentInPoints();
    }

    float height() const
    {
        if (! font.has_value())
            return 0.0f;

        // This is a little strange. Checking the numerical values inside lunasvg, we need to return
        // the font height, NOT the point height. But fontXHeight needs to be calculated using the
        // point height. This way we get the same text layout as in lunasvg. But the mix of the two
        // values is perplexing.
        return font->getHeight();
    }

    float lineGap() const
    {
        if (! font.has_value())
            return 0.0f;

        return font->getLineGapInPoints();
    }

    float xHeight() const
    {
        return fontXHeight;
    }

    float measureText (const std::u32string_view& text) const
    {
        if (! font.has_value())
            return 0.0f;

        const auto s = higgs::String { higgs::CharPointer_UTF32 (reinterpret_cast<const CharPointer_UTF32::CharType*> (text.data())),
                                      text.length() };
        return GlyphArrangement::getStringWidth (*font, s);
    }

    float size() const
    {
        if (! font.has_value())
            return 0.0f;

        return font->getHeightInPoints();
    }

    bool isNull() const
    {
        if (! font.has_value())
            return 0.0f;

        return ! font.has_value();
    }

    const Font& getFont() const
    {
        return *font;
    }

private:
    std::optional<Font> font;
    float fontXHeight = 0.0f;
};

} // namespace higgs::detail
