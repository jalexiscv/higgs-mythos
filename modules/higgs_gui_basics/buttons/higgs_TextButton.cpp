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

TextButton::TextButton()  : Button (String())
{
}

TextButton::TextButton (const String& name) : Button (name)
{
}

TextButton::TextButton (const String& name, const String& toolTip)  : Button (name)
{
    setTooltip (toolTip);
}

TextButton::~TextButton()
{
}

void TextButton::paintButton (Graphics& g, bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown)
{
    auto& lf = getLookAndFeel();

    lf.drawButtonBackground (g, *this,
                             findColour (getToggleState() ? buttonOnColourId : buttonColourId),
                             shouldDrawButtonAsHighlighted, shouldDrawButtonAsDown);

    lf.drawButtonText (g, *this, shouldDrawButtonAsHighlighted, shouldDrawButtonAsDown);
}

void TextButton::colourChanged()
{
    repaint();
}

void TextButton::changeWidthToFitText()
{
    changeWidthToFitText (getHeight());
}

void TextButton::changeWidthToFitText (const int newHeight)
{
    setSize (getBestWidthForHeight (newHeight), newHeight);
}

int TextButton::getBestWidthForHeight (int buttonHeight)
{
    return getLookAndFeel().getTextButtonWidthToFitText (*this, buttonHeight);
}

} // namespace higgs
