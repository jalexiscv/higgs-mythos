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

ImagePreviewComponent::ImagePreviewComponent()
{
}

ImagePreviewComponent::~ImagePreviewComponent()
{
}

//==============================================================================
void ImagePreviewComponent::getThumbSize (int& w, int& h) const
{
    auto availableW = proportionOfWidth (0.97f);
    auto availableH = getHeight() - 13 * 4;

    auto scale = jmin (1.0,
                       availableW / (double) w,
                       availableH / (double) h);

    w = roundToInt (scale * w);
    h = roundToInt (scale * h);
}

void ImagePreviewComponent::selectedFileChanged (const File& file)
{
    if (fileToLoad != file)
    {
        fileToLoad = file;
        startTimer (100);
    }
}

void ImagePreviewComponent::timerCallback()
{
    stopTimer();

    currentThumbnail = Image();
    currentDetails.clear();
    repaint();

    FileInputStream in (fileToLoad);

    if (in.openedOk() && fileToLoad.existsAsFile())
    {
        if (auto format = ImageFileFormat::findImageFormatForStream (in))
        {
            currentThumbnail = format->decodeImage (in);

            if (currentThumbnail.isValid())
            {
                auto w = currentThumbnail.getWidth();
                auto h = currentThumbnail.getHeight();

                currentDetails
                    << fileToLoad.getFileName() << "\n"
                    << format->getFormatName() << "\n"
                    << w << " x " << h << " pixels\n"
                    << File::descriptionOfSizeInBytes (fileToLoad.getSize());

                getThumbSize (w, h);

                currentThumbnail = currentThumbnail.rescaled (w, h);
            }
        }
    }
}

void ImagePreviewComponent::paint (Graphics& g)
{
    if (currentThumbnail.isValid())
    {
        g.setFont (13.0f);

        auto w = currentThumbnail.getWidth();
        auto h = currentThumbnail.getHeight();
        getThumbSize (w, h);

        const int numLines = 4;
        auto totalH = 13 * numLines + h + 4;
        auto y = (getHeight() - totalH) / 2;

        g.drawImageWithin (currentThumbnail,
                           (getWidth() - w) / 2, y, w, h,
                           RectanglePlacement::centred | RectanglePlacement::onlyReduceInSize,
                           false);

        g.drawFittedText (currentDetails,
                          0, y + h + 4, getWidth(), 100,
                          Justification::centredTop, numLines);
    }
}

//==============================================================================
std::unique_ptr<AccessibilityHandler> ImagePreviewComponent::createAccessibilityHandler()
{
    return std::make_unique<AccessibilityHandler> (*this, AccessibilityRole::image);
}

} // namespace higgs
