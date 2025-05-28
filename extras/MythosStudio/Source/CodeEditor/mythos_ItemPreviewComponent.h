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
class ItemPreviewComponent final : public Component
{
public:
    ItemPreviewComponent (const File& f)  : file (f)
    {
        setOpaque (true);
        tryToLoadImage();
    }

    void paint (Graphics& g) override
    {
        g.fillAll (findColour (backgroundColourId));

        if (drawable != nullptr)
        {
            auto contentBounds = drawable->getDrawableBounds();

            if (auto* dc = dynamic_cast<DrawableComposite*> (drawable.get()))
            {
                auto r = dc->getContentArea();

                if (! r.isEmpty())
                    contentBounds = r;
            }

            auto area = RectanglePlacement (RectanglePlacement::centred | RectanglePlacement::onlyReduceInSize)
                            .appliedTo (contentBounds, Rectangle<float> (4.0f, 22.0f, (float) getWidth() - 8.0f, (float) getHeight() - 26.0f));

            Path p;
            p.addRectangle (area);
            DropShadow (Colours::black.withAlpha (0.5f), 6, Point<int> (0, 1)).drawForPath (g, p);

            g.fillCheckerBoard (area, 24.0f, 24.0f, Colour (0xffffffff), Colour (0xffeeeeee));

            drawable->draw (g, 1.0f, RectanglePlacement (RectanglePlacement::stretchToFit)
                                        .getTransformToFit (contentBounds, area.toFloat()));
        }

        g.setFont (FontOptions (14.0f, Font::bold));
        g.setColour (findColour (defaultTextColourId));
        g.drawMultiLineText (facts.joinIntoString ("\n"), 10, 15, getWidth() - 16);
    }

private:
    StringArray facts;
    File file;
    std::unique_ptr<Drawable> drawable;

    void tryToLoadImage()
    {
        facts.clear();
        facts.add (file.getFullPathName());
        drawable.reset();

        if (auto input = std::unique_ptr<FileInputStream> (file.createInputStream()))
        {
            auto totalSize = input->getTotalLength();
            String formatName;

            if (auto* format = ImageFileFormat::findImageFormatForStream (*input))
                formatName = " " + format->getFormatName();

            input.reset();

            auto image = ImageCache::getFromFile (file);

            if (image.isValid())
            {
                auto* d = new DrawableImage();
                d->setImage (image);
                drawable.reset (d);

                facts.add (String (image.getWidth()) + " x " + String (image.getHeight()) + formatName);
            }

            if (totalSize > 0)
                facts.add (File::descriptionOfSizeInBytes (totalSize));
        }

        if (drawable == nullptr)
            if (const auto s = file.loadFileAsString(); s.contains ("<svg"))
                drawable = Drawable::createFromSVGString (s);

        facts.removeEmptyStrings (true);
    }

    HIGGS_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ItemPreviewComponent)
};
