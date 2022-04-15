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

namespace higgs::detail
{

struct StandardCachedComponentImage : public CachedComponentImage
{
    explicit StandardCachedComponentImage (Component& c) noexcept
        : owner (c)
    {
    }

    void paint (Graphics& g) override
    {
        scale = g.getInternalContext().getPhysicalPixelScaleFactor();
        auto compBounds = owner.getLocalBounds();
        auto imageBounds = compBounds * scale;

        if (image.isNull() || image.getBounds() != imageBounds)
        {
            auto tempImageType = g.getInternalContext().getPreferredImageTypeForTemporaryImages();
            image = Image (owner.isOpaque() ? Image::RGB
                                            : Image::ARGB,
                           jmax (1, imageBounds.getWidth()),
                           jmax (1, imageBounds.getHeight()),
                           ! owner.isOpaque(),
                           *tempImageType);
            image.setBackupEnabled (false);
            validArea.clear();
        }

        // If the cached image is outdated but cannot be backed-up, this indicates that the graphics
        // device holding the most recent copy of the cached image has gone away. Therefore, we've
        // effectively lost the contents of the cache, and we must repaint the entire component.
        if (auto ptr = image.getPixelData())
            if (auto* extensions = ptr->getBackupExtensions())
                if (extensions->needsBackup() && ! extensions->canBackup())
                    validArea.clear();

        if (! validArea.containsRectangle (compBounds))
        {
            Graphics imG (image);
            auto& lg = imG.getInternalContext();

            lg.addTransform (AffineTransform::scale (scale));

            for (auto& i : validArea)
                lg.excludeClipRectangle (i);

            if (! owner.isOpaque())
            {
                lg.setFill (Colours::transparentBlack);
                lg.fillRect (compBounds, true);
                lg.setFill (Colours::black);
            }

            owner.paintEntireComponent (imG, true);
        }

        validArea = compBounds;

        g.setColour (Colours::black.withAlpha (owner.getAlpha()));
        g.drawImageTransformed (image, AffineTransform::scale ((float) compBounds.getWidth()  / (float) imageBounds.getWidth(),
                                                               (float) compBounds.getHeight() / (float) imageBounds.getHeight()), false);
    }

    bool invalidateAll() override                            { validArea.clear(); return true; }
    bool invalidate (const Rectangle<int>& area) override    { validArea.subtract (area); return true; }
    void releaseResources() override                         { image = Image(); }

private:
    Image image;
    RectangleList<int> validArea;
    Component& owner;
    float scale = 1.0f;

    HIGGS_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (StandardCachedComponentImage)
};

} // namespace higgs::detail
