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
class ContentViewComponent final : public Component
{
public:
    ContentViewComponent()
    {
        setTitle ("Content");
        setFocusContainerType (Component::FocusContainerType::focusContainer);

        addAndMakeVisible (logoComponent);

        addAndMakeVisible (fileNameLabel);
        fileNameLabel.setJustificationType (Justification::centred);
    }

    void resized() override
    {
        auto bounds = getLocalBounds();

        fileNameLabel.setBounds (bounds.removeFromTop (15));

        if (content != nullptr)
            content->setBounds (bounds);
        else
            logoComponent.setBounds (bounds);
    }

    Component* getCurrentComponent() noexcept
    {
        return content.get();
    }

    void setContent (std::unique_ptr<Component> newContent,
                     const String& labelText)
    {
        content = std::move (newContent);
        addAndMakeVisible (content.get());

        fileNameLabel.setVisible (labelText.isNotEmpty());
        fileNameLabel.setText (labelText, dontSendNotification);

        resized();
    }

private:
    class LogoComponent final : public Component
    {
    public:
        void paint (Graphics& g) override
        {
            g.setColour (findColour (defaultTextColourId));

            auto bounds = getLocalBounds();
            bounds.reduce (bounds.getWidth() / 6, bounds.getHeight() / 6);

            g.setFont (15.0f);
            g.drawFittedText (versionInfo, bounds.removeFromBottom (50), Justification::centredBottom, 3);

            if (logo != nullptr)
                logo->drawWithin (g, bounds.withTrimmedBottom (bounds.getHeight() / 4).toFloat(),
                                  RectanglePlacement (RectanglePlacement::centred), 1.0f);
        }

    private:
        std::unique_ptr<Drawable> logo = []() -> std::unique_ptr<Drawable>
        {

            if (const auto s = String::createStringFromData (BinaryData::background_logo_svg,
                                                             BinaryData::background_logo_svgSize);
                s.contains ("<svg"))
            {
                return Drawable::createFromSVGString (s);
            }

            jassertfalse;
            return {};
        }();

        String versionInfo = SystemStats::getHIGGSVersion()
                             + newLine
                             + MythosStudioApplication::getApp().getVersionDescription();
    };

    std::unique_ptr<Component> content;
    LogoComponent logoComponent;
    Label fileNameLabel;

    HIGGS_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ContentViewComponent)
};
