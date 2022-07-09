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

ImageComponent::ImageComponent (const String& name)
    : Component (name),
      placement (RectanglePlacement::centred)
{
}

ImageComponent::~ImageComponent()
{
}

void ImageComponent::setImage (const Image& newImage)
{
    if (image != newImage)
    {
        image = newImage;
        repaint();
    }
}

void ImageComponent::setImage (const Image& newImage, RectanglePlacement placementToUse)
{
    if (image != newImage || placement != placementToUse)
    {
        image = newImage;
        placement = placementToUse;
        repaint();
    }
}

void ImageComponent::setImagePlacement (RectanglePlacement newPlacement)
{
    if (placement != newPlacement)
    {
        placement = newPlacement;
        repaint();
    }
}

const Image& ImageComponent::getImage() const
{
    return image;
}

RectanglePlacement ImageComponent::getImagePlacement() const
{
    return placement;
}

void ImageComponent::paint (Graphics& g)
{
    g.setOpacity (1.0f);
    g.drawImage (image, getLocalBounds().toFloat(), placement);
}

//==============================================================================
std::unique_ptr<AccessibilityHandler> ImageComponent::createAccessibilityHandler()
{
    class ImageComponentAccessibilityHandler final : public AccessibilityHandler
    {
    public:
        explicit ImageComponentAccessibilityHandler (ImageComponent& imageComponentToWrap)
            : AccessibilityHandler (imageComponentToWrap, AccessibilityRole::image),
              imageComponent (imageComponentToWrap)
        {
        }

        String getHelp() const override   { return imageComponent.getTooltip(); }

    private:
        ImageComponent& imageComponent;

        //==============================================================================
        HIGGS_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ImageComponentAccessibilityHandler)
    };

    return std::make_unique<ImageComponentAccessibilityHandler> (*this);
}

} // namespace higgs
