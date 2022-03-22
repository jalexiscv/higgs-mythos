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

HyperlinkButton::HyperlinkButton (const String& linkText,
                                  const URL& linkURL)
   : Button (linkText),
     url (linkURL),
     font (withDefaultMetrics (FontOptions { 14.0f, Font::underlined })),
     resizeFont (true),
     justification (Justification::centred)
{
    setMouseCursor (MouseCursor::PointingHandCursor);
    setTooltip (linkURL.toString (false));
}

HyperlinkButton::HyperlinkButton()
   : Button (String()),
     font (withDefaultMetrics (FontOptions { 14.0f, Font::underlined })),
     resizeFont (true),
     justification (Justification::centred)
{
    setMouseCursor (MouseCursor::PointingHandCursor);
}

HyperlinkButton::~HyperlinkButton()
{
}

//==============================================================================
void HyperlinkButton::setFont (const Font& newFont,
                               const bool resizeToMatchComponentHeight,
                               Justification justificationType)
{
    font = newFont;
    resizeFont = resizeToMatchComponentHeight;
    justification = justificationType;
    repaint();
}

void HyperlinkButton::setURL (const URL& newURL) noexcept
{
    url = newURL;
    setTooltip (newURL.toString (false));
}

Font HyperlinkButton::getFontToUse() const
{
    if (resizeFont)
        return font.withHeight ((float) getHeight() * 0.7f);

    return font;
}

void HyperlinkButton::changeWidthToFitText()
{
    setSize (GlyphArrangement::getStringWidthInt (getFontToUse(), getButtonText()) + 6, getHeight());
}

void HyperlinkButton::setJustificationType (Justification newJustification)
{
    if (justification != newJustification)
    {
        justification = newJustification;
        repaint();
    }
}

void HyperlinkButton::colourChanged()
{
    repaint();
}

//==============================================================================
void HyperlinkButton::clicked()
{
    if (url.isWellFormed())
        url.launchInDefaultBrowser();
}

void HyperlinkButton::paintButton (Graphics& g,
                                   bool shouldDrawButtonAsHighlighted,
                                   bool shouldDrawButtonAsDown)
{
    const Colour textColour (findColour (textColourId));

    if (isEnabled())
        g.setColour ((shouldDrawButtonAsHighlighted) ? textColour.darker ((shouldDrawButtonAsDown) ? 1.3f : 0.4f)
                                         : textColour);
    else
        g.setColour (textColour.withMultipliedAlpha (0.4f));

    g.setFont (getFontToUse());

    g.drawText (getButtonText(), getLocalBounds().reduced (1, 0),
                justification.getOnlyHorizontalFlags() | Justification::verticallyCentred,
                true);
}

std::unique_ptr<AccessibilityHandler> HyperlinkButton::createAccessibilityHandler()
{
    return std::make_unique<detail::ButtonAccessibilityHandler> (*this, AccessibilityRole::hyperlink);
}

} // namespace higgs
