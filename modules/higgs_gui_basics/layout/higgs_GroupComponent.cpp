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

GroupComponent::GroupComponent (const String& name,
                                const String& labelText)
    : Component (name),
      text (labelText),
      justification (Justification::left)
{
    setInterceptsMouseClicks  (false, true);
}

GroupComponent::~GroupComponent() {}

void GroupComponent::setText (const String& newText)
{
    if (text != newText)
    {
        text = newText;
        repaint();
    }
}

String GroupComponent::getText() const
{
    return text;
}

void GroupComponent::setTextLabelPosition (Justification newJustification)
{
    if (justification != newJustification)
    {
        justification = newJustification;
        repaint();
    }
}

void GroupComponent::paint (Graphics& g)
{
    getLookAndFeel().drawGroupComponentOutline (g, getWidth(), getHeight(),
                                                text, justification, *this);
}

void GroupComponent::enablementChanged()    { repaint(); }
void GroupComponent::colourChanged()        { repaint(); }

//==============================================================================
std::unique_ptr<AccessibilityHandler> GroupComponent::createAccessibilityHandler()
{
    return std::make_unique<AccessibilityHandler> (*this, AccessibilityRole::group);
}

} // namespace higgs
