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
class UTF8Component final : public Component
{
public:
    UTF8Component()
        : desc (String(),
                "Type any string into the box, and it'll be shown below as a portable UTF-8 literal, "
                "ready to cut-and-paste into your source-code...")
    {
        desc.setJustificationType (Justification::centred);
        addAndMakeVisible (desc);

        userText.setMultiLine (true, true);
        userText.setReturnKeyStartsNewLine (true);
        addAndMakeVisible (userText);
        userText.onTextChange = [this] { update(); };
        userText.onEscapeKey  = [this] { getTopLevelComponent()->exitModalState (0); };

        resultText.setFont (getAppSettings().appearance.getCodeFont().withHeight (13.0f));
        resultText.setMultiLine (true, true);
        resultText.setReadOnly (true);
        resultText.setSelectAllWhenFocused (true);
        addAndMakeVisible (resultText);

        userText.setText (getLastText());
    }

    void update()
    {
        getLastText() = userText.getText();
        resultText.setText (CodeHelpers::stringLiteral (getLastText(), 100), false);
    }

    void resized() override
    {
        auto r = getLocalBounds().reduced (8);
        desc.setBounds (r.removeFromTop (44));
        r.removeFromTop (8);
        userText.setBounds (r.removeFromTop (r.getHeight() / 2));
        r.removeFromTop (8);
        resultText.setBounds (r);
    }

    void lookAndFeelChanged() override
    {
        userText.applyFontToAllText (userText.getFont());
        resultText.applyFontToAllText (resultText.getFont());
    }

private:
    Label desc;
    TextEditor userText, resultText;

    String& getLastText()
    {
        static String t;
        return t;
    }
};
