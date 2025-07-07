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
class IconButton final : public Button
{
public:
    IconButton (String buttonName, Image imageToDisplay)
        : Button (buttonName),
          iconImage (imageToDisplay)
    {
        setTooltip (buttonName);
    }

    IconButton (String buttonName, Path pathToDisplay)
        : Button (buttonName),
          iconPath (pathToDisplay),
          iconImage (createImageFromPath (iconPath))
    {
        setTooltip (buttonName);
    }

    void setImage (Image newImage)
    {
        iconImage = newImage;
        repaint();
    }

    void setPath (Path newPath)
    {
        iconImage = createImageFromPath (newPath);
        repaint();
    }

    void setBackgroundColour (Colour backgroundColourToUse)
    {
        backgroundColour = backgroundColourToUse;
        usingNonDefaultBackgroundColour = true;
    }

    void setIconInset (int newIconInset)
    {
        iconInset = newIconInset;
        repaint();
    }

    void paintButton (Graphics& g, bool isMouseOverButton, bool isButtonDown) override
    {
        float alpha = 1.0f;

        if (! isEnabled())
        {
            isMouseOverButton = false;
            isButtonDown = false;

            alpha = 0.2f;
        }

        auto fill = isButtonDown ? backgroundColour.darker (0.5f)
                                 : isMouseOverButton ? backgroundColour.darker (0.2f)
                                                     : backgroundColour;

        auto bounds = getLocalBounds();

        if (isButtonDown)
            bounds.reduce (2, 2);

        Path ellipse;
        ellipse.addEllipse (bounds.toFloat());
        g.reduceClipRegion (ellipse);

        g.setColour (fill.withAlpha (alpha));
        g.fillAll();

        g.setOpacity (alpha);
        g.drawImage (iconImage, bounds.reduced (iconInset).toFloat(), RectanglePlacement::fillDestination, false);
    }

private:
    void lookAndFeelChanged() override
    {
        if (! usingNonDefaultBackgroundColour)
            backgroundColour = findColour (defaultButtonBackgroundColourId);

        if (iconPath != Path())
            iconImage = createImageFromPath (iconPath);

        repaint();
    }

    Image createImageFromPath (Path path)
    {
        Image image (Image::ARGB, 250, 250, true);
        Graphics g (image);

        g.setColour (findColour (defaultIconColourId));

        g.fillPath (path, RectanglePlacement (RectanglePlacement::centred)
                            .getTransformToFit (path.getBounds(), image.getBounds().toFloat()));

        return image;
    }

    Path iconPath;
    Image iconImage;
    Colour backgroundColour { findColour (defaultButtonBackgroundColourId) };
    bool usingNonDefaultBackgroundColour = false;
    int iconInset = 2;

    HIGGS_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (IconButton)
};
