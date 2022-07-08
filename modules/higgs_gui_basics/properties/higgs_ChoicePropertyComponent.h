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
    A PropertyComponent that shows its value as a combo box.

    This type of property component contains a list of options and has a
    combo box to choose one.

    Your subclass's constructor must add some strings to the choices StringArray
    and these are shown in the list.

    The getIndex() method will be called to find out which option is the currently
    selected one. If you call refresh() it will call getIndex() to check whether
    the value has changed, and will update the combo box if needed.

    If the user selects a different item from the list, setIndex() will be
    called to let your class process this.

    @see PropertyComponent, PropertyPanel

    @tags{GUI}
*/
class HIGGS_API  ChoicePropertyComponent    : public PropertyComponent
{
private:
    /** Delegating constructor. */
    ChoicePropertyComponent (const String&, const StringArray&, const Array<var>&);

protected:
    /** Creates the component.
        Your subclass's constructor must add a list of options to the choices member variable.
    */
    ChoicePropertyComponent (const String& propertyName);

public:
    /** Creates the component.

        Note that if you call this constructor then you must use the Value to interact with the
        index, and you can't override the class with your own setIndex or getIndex methods.
        If you want to use those methods, call the other constructor instead.

        @param valueToControl       the value that the combo box will read and control
        @param propertyName         the name of the property
        @param choices              the list of possible values that the drop-down list will contain
        @param correspondingValues  a list of values corresponding to each item in the 'choices' StringArray.
                                    These are the values that will be read and written to the
                                    valueToControl value. This array must contain the same number of items
                                    as the choices array
    */
    ChoicePropertyComponent (const Value& valueToControl,
                             const String& propertyName,
                             const StringArray& choices,
                             const Array<var>& correspondingValues);

    /** Creates the component using a ValueTreePropertyWithDefault object. This will add an item to the ComboBox for the
        default value with an ID of -1.

        @param valueToControl       the ValueTreePropertyWithDefault object that contains the Value object that the combo box will read and control.
        @param propertyName         the name of the property
        @param choices              the list of possible values that the drop-down list will contain
        @param correspondingValues  a list of values corresponding to each item in the 'choices' StringArray.
                                    These are the values that will be read and written to the
                                    valueToControl value. This array must contain the same number of items
                                    as the choices array
    */
    ChoicePropertyComponent (const ValueTreePropertyWithDefault& valueToControl,
                             const String& propertyName,
                             const StringArray& choices,
                             const Array<var>& correspondingValues);

    /** Creates the component using a ValueTreePropertyWithDefault object, adding an item to the ComboBox for the
        default value with an ID of -1 as well as adding separate "Enabled" and "Disabled" options.

        This is useful for simple on/off choices that also need a default value.
    */
    ChoicePropertyComponent (const ValueTreePropertyWithDefault& valueToControl,
                             const String& propertyName);

    //==============================================================================
    /** Called when the user selects an item from the combo box.

        Your subclass must use this callback to update the value that this component
        represents. The index is the index of the chosen item in the choices
        StringArray.
    */
    virtual void setIndex (int newIndex);

    /** Returns the index of the item that should currently be shown.
        This is the index of the item in the choices StringArray that will be shown.
    */
    virtual int getIndex() const;

    /** Returns the list of options. */
    const StringArray& getChoices() const;

    //==============================================================================
    /** @internal */
    void refresh() override;

protected:
    /** The list of options that will be shown in the combo box.

        Your subclass must populate this array in its constructor. If any empty
        strings are added, these will be replaced with horizontal separators (see
        ComboBox::addSeparator() for more info).
    */
    StringArray choices;

private:
    //==============================================================================
    void initialiseComboBox (const Value&);
    void refreshChoices();
    void refreshChoices (const String&);

    void changeIndex();

    //==============================================================================
    ValueTreePropertyWithDefault value;
    ComboBox comboBox;
    bool isCustomClass = false;

    //==============================================================================
    HIGGS_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ChoicePropertyComponent)
};

} // namespace higgs
