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

struct LookAndFeelHelpers
{
    LookAndFeelHelpers() = delete;

    static Colour createBaseColour (Colour buttonColour,
                                    bool hasKeyboardFocus,
                                    bool shouldDrawButtonAsHighlighted,
                                    bool shouldDrawButtonAsDown) noexcept
    {
        const float sat = hasKeyboardFocus ? 1.3f : 0.9f;
        const Colour baseColour (buttonColour.withMultipliedSaturation (sat));

        if (shouldDrawButtonAsDown)        return baseColour.contrasting (0.2f);
        if (shouldDrawButtonAsHighlighted) return baseColour.contrasting (0.1f);

        return baseColour;
    }

    static TextLayout layoutTooltipText (TypefaceMetricsKind metrics, const String& text, Colour colour) noexcept
    {
        const float tooltipFontSize = 13.0f;
        const int maxToolTipWidth = 400;

        AttributedString s;
        s.setWordWrap (AttributedString::WordWrap::byChar);
        s.setJustification (Justification::centred);
        s.append (text, FontOptions (tooltipFontSize, Font::bold).withMetricsKind (metrics), colour);

        TextLayout tl;
        tl.createLayoutWithBalancedLineLengths (s, (float) maxToolTipWidth);
        return tl;
    }
};

} // namespace higgs::detail
