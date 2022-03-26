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
    A type of button designed to go on a toolbar.

    This simple button can have two Drawable objects specified - one for normal
    use and another one (optionally) for the button's "on" state if it's a
    toggle button.

    @see Toolbar, ToolbarItemFactory, ToolbarItemComponent, Drawable, Button

    @tags{GUI}
*/
class HIGGS_API  ToolbarButton   : public ToolbarItemComponent
{
public:
    //==============================================================================
    /** Creates a ToolbarButton.

        @param itemId       the ID for this toolbar item type. This is passed through to the
                            ToolbarItemComponent constructor
        @param labelText    the text to display on the button (if the toolbar is using a style
                            that shows text labels). This is passed through to the
                            ToolbarItemComponent constructor
        @param normalImage  a drawable object that the button should use as its icon. The object
                            that is passed-in here will be kept by this object and will be
                            deleted when no longer needed or when this button is deleted.
        @param toggledOnImage  a drawable object that the button can use as its icon if the button
                            is in a toggled-on state (see the Button::getToggleState() method). If
                            nullptr is passed-in here, then the normal image will be used instead,
                            regardless of the toggle state. The object that is passed-in here will be
                            owned by this object and will be deleted when no longer needed or when
                            this button is deleted.
    */
    ToolbarButton (int itemId,
                   const String& labelText,
                   std::unique_ptr<Drawable> normalImage,
                   std::unique_ptr<Drawable> toggledOnImage);

    /** Destructor. */
    ~ToolbarButton() override;


    //==============================================================================
    /** @internal */
    bool getToolbarItemSizes (int toolbarDepth, bool isToolbarVertical, int& preferredSize,
                              int& minSize, int& maxSize) override;
    /** @internal */
    void paintButtonArea (Graphics&, int width, int height, bool isMouseOver, bool isMouseDown) override;
    /** @internal */
    void contentAreaChanged (const Rectangle<int>&) override;
    /** @internal */
    void buttonStateChanged() override;
    /** @internal */
    void resized() override;
    /** @internal */
    void enablementChanged() override;

private:
    //==============================================================================
    std::unique_ptr<DrawableComponent> normalImage, toggledOnImage;
    DrawableComponent* currentImage = nullptr;

    void updateDrawable();
    DrawableComponent* getImageToUse() const;
    void setCurrentImage (DrawableComponent*);

    HIGGS_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ToolbarButton)
};

} // namespace higgs
