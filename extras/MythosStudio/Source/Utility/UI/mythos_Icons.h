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
struct Icon
{
    Icon() = default;

    Icon (const Path& pathToUse, Colour pathColour)
        : path (pathToUse),
          colour (pathColour)
    {
    }

    void draw (Graphics& g, const higgs::Rectangle<float>& area, bool isCrossedOut) const
    {
        if (! path.isEmpty())
        {
            g.setColour (colour);

            const RectanglePlacement placement (RectanglePlacement::centred | RectanglePlacement::onlyReduceInSize);
            g.fillPath (path, placement.getTransformToFit (path.getBounds(), area));

            if (isCrossedOut)
            {
                g.setColour (Colours::red.withAlpha (0.8f));
                g.drawLine ((float) area.getX(), area.getY() + area.getHeight() * 0.2f,
                            (float) area.getRight(), area.getY() + area.getHeight() * 0.8f, 3.0f);
            }
        }
    }

    Icon withContrastingColourTo (Colour background) const
    {
        return Icon (path, background.contrasting (colour, 0.6f));
    }

    Icon withColour (Colour newColour)
    {
        return Icon (path, newColour);
    }

    Path path;
    Colour colour;
};

//==============================================================================
class Icons
{
public:
    Icons();

    Path imageDoc, config, graph, info, warning, user, closedFolder, exporter, fileExplorer, file,
         modules, openFolder, settings, singleModule, plus, android, linux, xcode, visualStudio;

private:
    HIGGS_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Icons)
};

const Icons& getIcons();
