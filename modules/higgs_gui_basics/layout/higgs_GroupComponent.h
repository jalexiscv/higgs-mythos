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
    A component that draws an outline around itself and has an optional title at
    the top, for drawing an outline around a group of controls.


    @tags{GUI}
*/
class HIGGS_API  GroupComponent    : public Component
{
public:
    //==============================================================================
    /** Creates a GroupComponent.

        @param componentName    the name to give the component
        @param labelText        the text to show at the top of the outline
    */
    GroupComponent (const String& componentName = {},
                    const String& labelText = {});

    /** Destructor. */
    ~GroupComponent() override;

    //==============================================================================
    /** Changes the text that's shown at the top of the component. */
    void setText (const String& newText);

    /** Returns the currently displayed text label. */
    String getText() const;

    /** Sets the positioning of the text label.
        (The default is Justification::left)
        @see getTextLabelPosition
    */
    void setTextLabelPosition (Justification justification);

    /** Returns the current text label position.
        @see setTextLabelPosition
    */
    Justification getTextLabelPosition() const noexcept           { return justification; }

    //==============================================================================
    /** A set of colour IDs to use to change the colour of various aspects of the component.

        These constants can be used either via the Component::setColour(), or LookAndFeel::setColour()
        methods.

        @see Component::setColour, Component::findColour, LookAndFeel::setColour, LookAndFeel::findColour
    */
    enum ColourIds
    {
        outlineColourId     = 0x1005400,    /**< The colour to use for drawing the line around the edge. */
        textColourId        = 0x1005410     /**< The colour to use to draw the text label. */
    };

    //==============================================================================
    /** This abstract base class is implemented by LookAndFeel classes. */
    struct HIGGS_API  LookAndFeelMethods
    {
        virtual ~LookAndFeelMethods() = default;

        virtual void drawGroupComponentOutline (Graphics&, int w, int h, const String& text,
                                                const Justification&, GroupComponent&) = 0;
    };

    //==============================================================================
    /** @internal */
    void paint (Graphics&) override;
    /** @internal */
    void enablementChanged() override;
    /** @internal */
    void colourChanged() override;
    /** @internal */
    std::unique_ptr<AccessibilityHandler> createAccessibilityHandler() override;

private:
    String text;
    Justification justification;

    HIGGS_DECLARE_NON_COPYABLE (GroupComponent)
};

} // namespace higgs
