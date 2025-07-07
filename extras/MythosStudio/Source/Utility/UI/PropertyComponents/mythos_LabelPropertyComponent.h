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

#pragma once


//==============================================================================
class LabelPropertyComponent final : public PropertyComponent
{
public:
    LabelPropertyComponent (const String& labelText, int propertyHeight = 25,
                            Font labelFont = FontOptions (16.0f, Font::bold),
                            Justification labelJustification = Justification::centred)
        : PropertyComponent (labelText),
          labelToDisplay ({}, labelText)
    {
        setPreferredHeight (propertyHeight);

        labelToDisplay.setJustificationType (labelJustification);
        labelToDisplay.setFont (labelFont);

        addAndMakeVisible (labelToDisplay);
        setLookAndFeel (&lf);
    }

    ~LabelPropertyComponent() override    { setLookAndFeel (nullptr); }

    //==============================================================================
    void refresh() override {}

    void resized() override
    {
        labelToDisplay.setBounds (getLocalBounds());
    }

private:
    //==============================================================================
    struct LabelLookAndFeel final : public MythosStudioLookAndFeel
    {
        void drawPropertyComponentLabel (Graphics&, int, int, PropertyComponent&) override {}
    };

    void lookAndFeelChanged() override
    {
        labelToDisplay.setColour (Label::textColourId, MythosStudioApplication::getApp().lookAndFeel.findColour (defaultTextColourId));
    }

    //==============================================================================
    LabelLookAndFeel lf;
    Label labelToDisplay;
};
