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
class MythosStudioLookAndFeel : public LookAndFeel_V4
{
public:
    MythosStudioLookAndFeel();
    ~MythosStudioLookAndFeel() override;

    void drawTabButton (TabBarButton& button, Graphics&, bool isMouseOver, bool isMouseDown) override;
    int getTabButtonBestWidth (TabBarButton&, int tabDepth) override;
    void drawTabAreaBehindFrontButton (TabbedButtonBar&, Graphics&, int, int) override {}

    void drawPropertyComponentBackground (Graphics&, int, int, PropertyComponent&) override {}
    void drawPropertyComponentLabel (Graphics&, int width, int height, PropertyComponent&) override;
    Rectangle<int> getPropertyComponentContentPosition (PropertyComponent&) override;

    void drawButtonBackground (Graphics&, Button&, const Colour& backgroundColour,
                               bool isMouseOverButton, bool isButtonDown) override;
    void drawButtonText (Graphics&, TextButton&, bool isMouseOverButton, bool isButtonDown) override;
    void drawToggleButton (Graphics&, ToggleButton&, bool isMouseOverButton, bool isButtonDown) override;

    void drawTextEditorOutline (Graphics&, int, int, TextEditor&) override {}
    void fillTextEditorBackground (Graphics&, int width, int height, TextEditor&) override;

    void layoutFileBrowserComponent (FileBrowserComponent&, DirectoryContentsDisplayComponent*,
                                     FilePreviewComponent*, ComboBox* currentPathBox,
                                     TextEditor* filenameBox,Button* goUpButton) override;
    void drawFileBrowserRow (Graphics&, int width, int height, const File&, const String& filename, Image* icon,
                             const String& fileSizeDescription, const String& fileTimeDescription,
                             bool isDirectory, bool isItemSelected, int itemIndex, DirectoryContentsDisplayComponent&) override;

    void drawCallOutBoxBackground (CallOutBox&, Graphics&, const Path&, Image&) override;

    void drawMenuBarBackground (Graphics&, int width, int height, bool isMouseOverBar, MenuBarComponent&) override;

    void drawMenuBarItem (Graphics&, int width, int height,
                          int itemIndex, const String& itemText,
                          bool isMouseOverItem, bool isMenuOpen, bool isMouseOverBar,
                          MenuBarComponent&) override;

    void drawResizableFrame (Graphics&, int w, int h, const BorderSize<int>&) override;

    void drawComboBox (Graphics&, int width, int height, bool isButtonDown,
                       int buttonX, int buttonY, int buttonW, int buttonH,
                       ComboBox&) override;

    void drawTreeviewPlusMinusBox (Graphics&, const Rectangle<float>& area,
                                   Colour backgroundColour, bool isItemOpen, bool isMouseOver) override;

    ProgressBar::Style getDefaultProgressBarStyle (const ProgressBar&) override;

    //==============================================================================
    static Path getArrowPath (Rectangle<float> arrowZone, int direction,
                              bool filled, Justification justification);
    static Path getChoiceComponentArrowPath (Rectangle<float> arrowZone);

    static Font getPropertyComponentFont()                                       { return FontOptions { 14.0f, Font::FontStyleFlags::bold }; }
    static int getTextWidthForPropertyComponent (const PropertyComponent& pc)    { return jmin (200, pc.getWidth() / 2); }

    static ColourScheme getMythosStudioDarkColourScheme()
    {
        return { 0xff323e44, 0xff263238, 0xff323e44,
                 0xff8e989b, 0xffffffff, 0xffa45c94,
                 0xffffffff, 0xff181f22, 0xffffffff };
    }

    //==============================================================================
    void setupColours();

private:

    HIGGS_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MythosStudioLookAndFeel)
};
