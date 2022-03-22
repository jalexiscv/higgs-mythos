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
    A button showing an underlined weblink, that will launch the link
    when it's clicked.

    @see Button

    @tags{GUI}
*/
class HIGGS_API  HyperlinkButton  : public Button
{
public:
    //==============================================================================
    /** Creates a HyperlinkButton.

        @param linkText     the text that will be displayed in the button - this is
                            also set as the Component's name, but the text can be
                            changed later with the Button::setButtonText() method
        @param linkURL      the URL to launch when the user clicks the button
    */
    HyperlinkButton (const String& linkText,
                     const URL& linkURL);

    /** Creates a HyperlinkButton. */
    HyperlinkButton();

    /** Destructor. */
    ~HyperlinkButton() override;

    //==============================================================================
    /** Changes the font to use for the text.

        If resizeToMatchComponentHeight is true, the font's height will be adjusted
        to match the size of the component.
    */
    void setFont (const Font& newFont,
                  bool resizeToMatchComponentHeight,
                  Justification justificationType = Justification::horizontallyCentred);

    //==============================================================================
    /** A set of colour IDs to use to change the colour of various aspects of the link.

        These constants can be used either via the Component::setColour(), or LookAndFeel::setColour()
        methods.

        @see Component::setColour, Component::findColour, LookAndFeel::setColour, LookAndFeel::findColour
    */
    enum ColourIds
    {
        textColourId             = 0x1001f00, /**< The colour to use for the URL text. */
    };

    //==============================================================================
    /** Changes the URL that the button will trigger. */
    void setURL (const URL& newURL) noexcept;

    /** Returns the URL that the button will trigger. */
    const URL& getURL() const noexcept                          { return url; }

    //==============================================================================
    /** Resizes the button horizontally to fit snugly around the text.

        This won't affect the button's height.
    */
    void changeWidthToFitText();

    //==============================================================================
    /** Sets the style of justification to be used for positioning the text.
        (The default is Justification::centred)
    */
    void setJustificationType (Justification justification);

    /** Returns the type of justification, as set in setJustificationType(). */
    Justification getJustificationType() const noexcept         { return justification; }

    /** @internal */
    std::unique_ptr<AccessibilityHandler> createAccessibilityHandler() override;

protected:
    //==============================================================================
    /** @internal */
    void clicked() override;
    /** @internal */
    void colourChanged() override;
    /** @internal */
    void paintButton (Graphics&, bool, bool) override;

private:
    //==============================================================================
    using Button::clicked;
    Font getFontToUse() const;

    //==============================================================================
    URL url;
    Font font;
    bool resizeFont;
    Justification justification;

    HIGGS_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (HyperlinkButton)
};

} // namespace higgs
