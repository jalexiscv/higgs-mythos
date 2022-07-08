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
    A PropertyComponent that shows its value as an expandable list of ToggleButtons.

    This type of property component contains a list of options where multiple options
    can be selected at once.

    @see PropertyComponent, PropertyPanel

    @tags{GUI}
*/
class MultiChoicePropertyComponent    : public PropertyComponent
{
private:
    /** Delegating constructor. */
    MultiChoicePropertyComponent (const String&, const StringArray&, const Array<var>&);

public:
    /** Creates the component. Note that the underlying var object that the Value refers to must be an array.

        @param valueToControl       the value that the ToggleButtons will read and control
        @param propertyName         the name of the property
        @param choices              the list of possible values that will be represented
        @param correspondingValues  a list of values corresponding to each item in the 'choices' StringArray.
                                    These are the values that will be read and written to the
                                    valueToControl value. This array must contain the same number of items
                                    as the choices array
        @param maxChoices           the maximum number of values which can be selected at once. The default of
                                    -1 will not limit the number that can be selected
    */
    MultiChoicePropertyComponent (const Value& valueToControl,
                                  const String& propertyName,
                                  const StringArray& choices,
                                  const Array<var>& correspondingValues,
                                  int maxChoices = -1);

    /** Creates the component using a ValueTreePropertyWithDefault object. This will select the default options.

        @param valueToControl       the ValueTreePropertyWithDefault object that contains the Value object that the ToggleButtons will read and control.
        @param propertyName         the name of the property
        @param choices              the list of possible values that will be represented
        @param correspondingValues  a list of values corresponding to each item in the 'choices' StringArray.
                                    These are the values that will be read and written to the
                                    valueToControl value. This array must contain the same number of items
                                    as the choices array
        @param maxChoices           the maximum number of values which can be selected at once. The default of
                                    -1 will not limit the number that can be selected
    */
    MultiChoicePropertyComponent (const ValueTreePropertyWithDefault& valueToControl,
                                  const String& propertyName,
                                  const StringArray& choices,
                                  const Array<var>& correspondingValues,
                                  int maxChoices = -1);

    //==============================================================================
    /** Returns true if the list of options is expanded. */
    bool isExpanded() const noexcept    { return expanded; }

    /** Returns true if the list of options has been truncated and can be expanded. */
    bool isExpandable() const noexcept  { return expandable; }

    /** Expands or shrinks the list of options if they are not all visible.

        N.B. This will just set the preferredHeight value of the PropertyComponent and attempt to
        call PropertyPanel::resized(), so if you are not displaying this object in a PropertyPanel
        then you should use the onHeightChange callback to resize it when the height changes.

        @see onHeightChange
    */
    void setExpanded (bool expanded) noexcept;

    /** You can assign a lambda to this callback object to have it called when the
        height of this component changes in response to being expanded/collapsed.

        @see setExpanded
    */
    std::function<void()> onHeightChange;

    //==============================================================================
    /** @internal */
    void paint (Graphics& g) override;
    /** @internal */
    void resized() override;
    /** @internal */
    void refresh() override {}

private:
    //==============================================================================
    class MultiChoiceRemapperSource;
    class MultiChoiceRemapperSourceWithDefault;

    //==============================================================================
    static int getTotalButtonsHeight (int);
    void lookAndFeelChanged() override;

    //==============================================================================
    static constexpr int collapsedHeight = 125;
    static constexpr int buttonHeight = 25;
    static constexpr int expandAreaHeight = 20;

    int maxHeight = 0, numHidden = 0;
    bool expandable = false, expanded = false;

    ValueTreePropertyWithDefault value;
    OwnedArray<ToggleButton> choiceButtons;
    ShapeButton expandButton { "Expand", Colours::transparentBlack, Colours::transparentBlack, Colours::transparentBlack };

    //==============================================================================
    HIGGS_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MultiChoicePropertyComponent)
};

} // namespace higgs
