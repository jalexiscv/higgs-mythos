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

//==============================================================================
/**
    A component that simply displays an image.

    Use setImage to give it an image, and it'll display it - simple as that!

    @tags{GUI}
*/
class HIGGS_API  ImageComponent  : public Component,
                                  public SettableTooltipClient
{
public:
    //==============================================================================
    /** Creates an ImageComponent. */
    ImageComponent (const String& componentName = String());

    /** Destructor. */
    ~ImageComponent() override;

    //==============================================================================
    /** Sets the image that should be displayed. */
    void setImage (const Image& newImage);

    /** Sets the image that should be displayed, and its placement within the component. */
    void setImage (const Image& newImage,
                   RectanglePlacement placementToUse);

    /** Returns the current image. */
    const Image& getImage() const;

    /** Sets the method of positioning that will be used to fit the image within the component's bounds.
        By default the positioning is centred, and will fit the image inside the component's bounds
        whilst keeping its aspect ratio correct, but you can change it to whatever layout you need.
    */
    void setImagePlacement (RectanglePlacement newPlacement);

    /** Returns the current image placement. */
    RectanglePlacement getImagePlacement() const;

    //==============================================================================
    /** @internal */
    void paint (Graphics&) override;
    /** @internal */
    std::unique_ptr<AccessibilityHandler> createAccessibilityHandler() override;

private:
    Image image;
    RectanglePlacement placement;

    HIGGS_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ImageComponent)
};

} // namespace higgs
