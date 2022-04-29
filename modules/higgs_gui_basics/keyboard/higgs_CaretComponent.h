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

    @tags{GUI}
*/
class HIGGS_API  CaretComponent   : public Component,
                                   private Timer
{
public:
    //==============================================================================
    /** Creates the caret component.
        The keyFocusOwner is an optional component which the caret will check, making
        itself visible only when the keyFocusOwner has keyboard focus.
    */
    CaretComponent (Component* keyFocusOwner);

    /** Destructor. */
    ~CaretComponent() override;

    //==============================================================================
    /** Sets the caret's position to place it next to the given character.
        The area is the rectangle containing the entire character that the caret is
        positioned on, so by default a vertical-line caret may choose to just show itself
        at the left of this area. You can override this method to customise its size.
        This method will also force the caret to reset its timer and become visible (if
        appropriate), so that as it moves, you can see where it is.
    */
    virtual void setCaretPosition (const Rectangle<int>& characterArea);

    /** A set of colour IDs to use to change the colour of various aspects of the caret.
        These constants can be used either via the Component::setColour(), or LookAndFeel::setColour()
        methods.
        @see Component::setColour, Component::findColour, LookAndFeel::setColour, LookAndFeel::findColour
    */
    enum ColourIds
    {
        caretColourId    = 0x1000204, /**< The colour with which to draw the caret. */
    };

    //==============================================================================
    /** @internal */
    void paint (Graphics&) override;

private:
    Component* owner;

    bool shouldBeShown() const;
    void timerCallback() override;

    HIGGS_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (CaretComponent)
};

} // namespace higgs
