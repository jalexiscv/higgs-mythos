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
    A PropertyComponent that contains a button.

    This type of property component can be used if you need a button to trigger some
    kind of action.

    @see PropertyComponent

    @tags{GUI}
*/
class HIGGS_API  ButtonPropertyComponent  : public PropertyComponent
{
public:
    //==============================================================================
    /** Creates a button component.

        @param propertyName         the property name to be passed to the PropertyComponent
        @param triggerOnMouseDown   this is passed to the Button::setTriggeredOnMouseDown() method
    */
    ButtonPropertyComponent (const String& propertyName,
                             bool triggerOnMouseDown);

    /** Destructor. */
    ~ButtonPropertyComponent() override;

    //==============================================================================
    /** Called when the user clicks the button.
    */
    virtual void buttonClicked() = 0;

    /** Returns the string that should be displayed in the button.

        If you need to change this string, call refresh() to update the component.
    */
    virtual String getButtonText() const = 0;

    //==============================================================================
    /** @internal */
    void refresh() override;

private:
    TextButton button;

    HIGGS_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ButtonPropertyComponent)
};

} // namespace higgs
