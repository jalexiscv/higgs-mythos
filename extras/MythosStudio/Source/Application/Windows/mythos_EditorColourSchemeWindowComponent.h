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

#include "../../Utility/UI/PropertyComponents/mythos_ColourPropertyComponent.h"

//==============================================================================
class EditorColourSchemeWindowComponent final : public Component
{
public:
    EditorColourSchemeWindowComponent()
    {
        if (getAppSettings().monospacedFontNames.size() == 0)
            changeContent (new AppearanceEditor::FontScanPanel());
        else
            changeContent (new AppearanceEditor::EditorPanel());
    }

    void paint (Graphics& g) override
    {
        g.fillAll (findColour (backgroundColourId));
    }

    void resized() override
    {
       content->setBounds (getLocalBounds());
    }

    void changeContent (Component* newContent)
    {
        content.reset (newContent);
        addAndMakeVisible (newContent);
        content->setBounds (getLocalBounds().reduced (10));
    }

private:
    std::unique_ptr<Component> content;

    //==============================================================================
    struct AppearanceEditor
    {
        struct FontScanPanel final : public Component,
                                     private Timer
        {
            FontScanPanel()
            {
                fontsToScan = Font::findAllTypefaceNames();
                startTimer (1);
            }

            void paint (Graphics& g) override
            {
                g.fillAll (findColour (backgroundColourId));

                g.setFont (14.0f);
                g.setColour (findColour (defaultTextColourId));
                g.drawFittedText ("Scanning for fonts..", getLocalBounds(), Justification::centred, 2);

                const auto size = 30;
                getLookAndFeel().drawSpinningWaitAnimation (g, Colours::white, (getWidth() - size) / 2, getHeight() / 2 - 50, size, size);
            }

            void timerCallback() override
            {
                repaint();

                if (fontsToScan.size() == 0)
                {
                    getAppSettings().monospacedFontNames = fontsFound;

                    if (auto* owner = findParentComponentOfClass<EditorColourSchemeWindowComponent>())
                        owner->changeContent (new EditorPanel());
                }
                else
                {
                    if (isMonospacedTypeface (fontsToScan[0]))
                        fontsFound.add (fontsToScan[0]);

                    fontsToScan.remove (0);
                }
            }

            // A rather hacky trick to select only the fixed-pitch fonts..
            // This is unfortunately a bit slow, but will work on all platforms.
            static bool isMonospacedTypeface (const String& name)
            {
                const Font font = FontOptions (name, 20.0f, Font::plain);

                const auto width = GlyphArrangement::getStringWidthInt (font, "....");

                return width == GlyphArrangement::getStringWidthInt (font, "WWWW")
                    && width == GlyphArrangement::getStringWidthInt (font, "0000")
                    && width == GlyphArrangement::getStringWidthInt (font, "1111")
                    && width == GlyphArrangement::getStringWidthInt (font, "iiii");
            }

            StringArray fontsToScan, fontsFound;
        };

        //==============================================================================
        struct EditorPanel final : public Component
        {
            EditorPanel()
                : loadButton ("Load Scheme..."),
                  saveButton ("Save Scheme...")
            {
                rebuildProperties();
                addAndMakeVisible (panel);

                addAndMakeVisible (loadButton);
                addAndMakeVisible (saveButton);

                loadButton.onClick = [this] { loadScheme(); };
                saveButton.onClick = [this] { saveScheme (false); };

                lookAndFeelChanged();

                saveSchemeState();
            }

            ~EditorPanel() override
            {
                if (hasSchemeBeenModifiedSinceSave())
                    saveScheme (true);
            }

            void rebuildProperties()
            {
                auto& scheme = getAppSettings().appearance;

                Array<PropertyComponent*> props;
                auto fontValue = scheme.getCodeFontValue();
                props.add (FontNameValueSource::createProperty ("Code Editor Font", fontValue));
                props.add (FontSizeValueSource::createProperty ("Font Size", fontValue));

                const auto colourNames = scheme.getColourNames();

                for (int i = 0; i < colourNames.size(); ++i)
                    props.add (new ColourPropertyComponent (nullptr, colourNames[i],
                                                            scheme.getColourValue (colourNames[i]),
                                                            Colours::white, false));

                panel.clear();
                panel.addProperties (props);
            }

            void resized() override
            {
                auto r = getLocalBounds();
                panel.setBounds (r.removeFromTop (getHeight() - 28).reduced (10, 2));
                loadButton.setBounds (r.removeFromLeft (getWidth() / 2).reduced (10, 1));
                saveButton.setBounds (r.reduced (10, 1));
            }

        private:
            PropertyPanel panel;
            TextButton loadButton, saveButton;

            Font codeFont { FontOptions{} };
            Array<var> colourValues;

            void saveScheme (bool isExit)
            {
                chooser = std::make_unique<FileChooser> ("Select a file in which to save this colour-scheme...",
                                                         getAppSettings().appearance.getSchemesFolder()
                                                         .getNonexistentChildFile ("Scheme", AppearanceSettings::getSchemeFileSuffix()),
                                                         AppearanceSettings::getSchemeFileWildCard());
                auto chooserFlags = FileBrowserComponent::saveMode
                                  | FileBrowserComponent::canSelectFiles
                                  | FileBrowserComponent::warnAboutOverwriting;

                chooser->launchAsync (chooserFlags, [this, isExit] (const FileChooser& fc)
                {
                    if (fc.getResult() == File{})
                    {
                        if (isExit)
                            restorePreviousScheme();

                        return;
                    }

                    File file (fc.getResult().withFileExtension (AppearanceSettings::getSchemeFileSuffix()));
                    getAppSettings().appearance.writeToFile (file);
                    getAppSettings().appearance.refreshPresetSchemeList();

                    saveSchemeState();
                    MythosStudioApplication::getApp().selectEditorColourSchemeWithName (file.getFileNameWithoutExtension());
                });
            }

            void loadScheme()
            {
                chooser = std::make_unique<FileChooser> ("Please select a colour-scheme file to load...",
                                                         getAppSettings().appearance.getSchemesFolder(),
                                                         AppearanceSettings::getSchemeFileWildCard());
                auto chooserFlags = FileBrowserComponent::openMode
                                  | FileBrowserComponent::canSelectFiles;

                chooser->launchAsync (chooserFlags, [this] (const FileChooser& fc)
                {
                    if (fc.getResult() == File{})
                        return;

                    if (getAppSettings().appearance.readFromFile (fc.getResult()))
                    {
                        rebuildProperties();
                        saveSchemeState();
                    }
                });
            }

            void lookAndFeelChanged() override
            {
                loadButton.setColour (TextButton::buttonColourId,
                                      findColour (secondaryButtonBackgroundColourId));
            }

            void saveSchemeState()
            {
                auto& appearance = getAppSettings().appearance;
                const auto colourNames = appearance.getColourNames();

                codeFont = appearance.getCodeFont();

                colourValues.clear();
                for (int i = 0; i < colourNames.size(); ++i)
                    colourValues.add (appearance.getColourValue (colourNames[i]).getValue());
            }

            bool hasSchemeBeenModifiedSinceSave()
            {
                auto& appearance = getAppSettings().appearance;
                const auto colourNames = appearance.getColourNames();

                if (codeFont != appearance.getCodeFont())
                    return true;

                for (int i = 0; i < colourNames.size(); ++i)
                    if (colourValues[i] != appearance.getColourValue (colourNames[i]).getValue())
                        return true;

                return false;
            }

            void restorePreviousScheme()
            {
                auto& appearance = getAppSettings().appearance;
                const auto colourNames = appearance.getColourNames();

                appearance.getCodeFontValue().setValue (codeFont.toString());

                for (int i = 0; i < colourNames.size(); ++i)
                    appearance.getColourValue (colourNames[i]).setValue (colourValues[i]);
            }

            std::unique_ptr<FileChooser> chooser;

            HIGGS_DECLARE_NON_COPYABLE (EditorPanel)
        };

        //==============================================================================
        struct FontNameValueSource final : public ValueSourceFilter
        {
            FontNameValueSource (const Value& source)  : ValueSourceFilter (source) {}

            var getValue() const override
            {
                return Font::fromString (sourceValue.toString()).getTypefaceName();
            }

            void setValue (const var& newValue) override
            {
                auto font = Font::fromString (sourceValue.toString());
                font.setTypefaceName (newValue.toString().isEmpty() ? Font::getDefaultMonospacedFontName()
                                      : newValue.toString());
                sourceValue = font.toString();
            }

            static ChoicePropertyComponent* createProperty (const String& title, const Value& value)
            {
                auto fontNames = getAppSettings().monospacedFontNames;

                Array<var> values;
                values.add (Font::getDefaultMonospacedFontName());
                values.add (var());

                for (int i = 0; i < fontNames.size(); ++i)
                    values.add (fontNames[i]);

                StringArray names;
                names.add ("<Default Monospaced>");
                names.add (String());
                names.addArray (getAppSettings().monospacedFontNames);

                return new ChoicePropertyComponent (Value (new FontNameValueSource (value)),
                                                    title, names, values);
            }
        };

        //==============================================================================
        struct FontSizeValueSource final : public ValueSourceFilter
        {
            FontSizeValueSource (const Value& source)  : ValueSourceFilter (source) {}

            var getValue() const override
            {
                return Font::fromString (sourceValue.toString()).getHeight();
            }

            void setValue (const var& newValue) override
            {
                sourceValue = Font::fromString (sourceValue.toString()).withHeight (newValue).toString();
            }

            static PropertyComponent* createProperty (const String& title, const Value& value)
            {
                return new SliderPropertyComponent (Value (new FontSizeValueSource (value)),
                                                    title, 5.0, 40.0, 0.1, 0.5);
            }
        };
    };

    HIGGS_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (EditorColourSchemeWindowComponent)
};
