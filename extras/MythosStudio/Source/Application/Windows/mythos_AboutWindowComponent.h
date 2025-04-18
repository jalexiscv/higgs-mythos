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
class AboutWindowComponent final : public Component
{
public:
    AboutWindowComponent()
    {
        addAndMakeVisible (titleLabel);
        titleLabel.setJustificationType (Justification::centred);
        titleLabel.setFont (FontOptions (35.0f, Font::FontStyleFlags::bold));

        auto buildDate = Time::getCompilationDate();
        addAndMakeVisible (versionLabel);
        versionLabel.setText ("HIGGS v" + MythosStudioApplication::getApp().getApplicationVersion()
                              + "\nBuild date: " + String (buildDate.getDayOfMonth())
                                                 + " " + Time::getMonthName (buildDate.getMonth(), true)
                                                 + " " + String (buildDate.getYear()),
                              dontSendNotification);

        versionLabel.setJustificationType (Justification::centred);
        addAndMakeVisible (copyrightLabel);
        copyrightLabel.setJustificationType (Justification::centred);

        addAndMakeVisible (aboutButton);
        aboutButton.setTooltip ( {} );
    }

    void resized() override
    {
        auto bounds = getLocalBounds();
        bounds.removeFromBottom (20);

        auto leftSlice   = bounds.removeFromLeft (150);
        auto centreSlice = bounds.withTrimmedRight (150);

        higgsLogoBounds = leftSlice.removeFromTop (150).toFloat();
        higgsLogoBounds.setWidth (higgsLogoBounds.getWidth() + 100);
        higgsLogoBounds.setHeight (higgsLogoBounds.getHeight() + 100);

        auto titleHeight = 40;

        centreSlice.removeFromTop ((centreSlice.getHeight() / 2) - (titleHeight / 2));

        titleLabel.setBounds (centreSlice.removeFromTop (titleHeight));

        centreSlice.removeFromTop (10);
        versionLabel.setBounds (centreSlice.removeFromTop (40));

        centreSlice.removeFromTop (10);
        aboutButton.setBounds (centreSlice.removeFromTop (20));

        copyrightLabel.setBounds (getLocalBounds().removeFromBottom (50));
    }

    void paint (Graphics& g) override
    {
        g.fillAll (findColour (backgroundColourId));

        if (higgsLogo != nullptr)
            higgsLogo->drawWithin (g, higgsLogoBounds.translated (-75, -75), RectanglePlacement::centred, 1.0);
    }

private:
    Label titleLabel { "title", "MYTHOSSTUDIO" },
          versionLabel { "version" },
          copyrightLabel { "copyright", String (CharPointer_UTF8 ("\xc2\xa9")) + String (" 2020 Raw Material Software Limited") };

    HyperlinkButton aboutButton { "About Us", URL ("https://higgsmythos.com") };

    Rectangle<float> higgsLogoBounds;

    std::unique_ptr<Drawable> higgsLogo { Drawable::createFromImageData (BinaryData::higgs_icon_png,
                                                                        BinaryData::higgs_icon_pngSize) };

    HIGGS_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AboutWindowComponent)
};
