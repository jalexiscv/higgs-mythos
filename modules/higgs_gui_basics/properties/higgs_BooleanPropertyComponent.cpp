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

BooleanPropertyComponent::BooleanPropertyComponent (const String& name,
                                                    const String& buttonTextWhenTrue,
                                                    const String& buttonTextWhenFalse)
    : PropertyComponent (name),
      onText (buttonTextWhenTrue),
      offText (buttonTextWhenFalse)
{
    addAndMakeVisible (button);
    button.setClickingTogglesState (false);
    button.onClick = [this] { setState (! getState()); };
}

BooleanPropertyComponent::BooleanPropertyComponent (const Value& valueToControl,
                                                    const String& name,
                                                    const String& buttonText)
    : PropertyComponent (name),
      onText (buttonText),
      offText (buttonText)
{
    addAndMakeVisible (button);
    button.setClickingTogglesState (false);
    button.setButtonText (buttonText);
    button.getToggleStateValue().referTo (valueToControl);
    button.setClickingTogglesState (true);
}

BooleanPropertyComponent::~BooleanPropertyComponent()
{
}

void BooleanPropertyComponent::setState (const bool newState)
{
    button.setToggleState (newState, sendNotification);
}

bool BooleanPropertyComponent::getState() const
{
    return button.getToggleState();
}

void BooleanPropertyComponent::paint (Graphics& g)
{
    PropertyComponent::paint (g);

    g.setColour (findColour (backgroundColourId));
    g.fillRect (button.getBounds());

    g.setColour (findColour (outlineColourId));
    g.drawRect (button.getBounds());
}

void BooleanPropertyComponent::refresh()
{
    button.setToggleState (getState(), dontSendNotification);
    button.setButtonText (button.getToggleState() ? onText : offText);
}

} // namespace higgs
