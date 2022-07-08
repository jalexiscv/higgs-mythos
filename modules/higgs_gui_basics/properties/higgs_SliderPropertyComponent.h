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
    A PropertyComponent that shows its value as a slider.

    @see PropertyComponent, Slider

    @tags{GUI}
*/
class HIGGS_API  SliderPropertyComponent   : public PropertyComponent
{
protected:
    //==============================================================================
    /** Creates the property component.

        The ranges, interval and skew factor are passed to the Slider component.

        If you need to customise the slider in other ways, your constructor can
        access the slider member variable and change it directly.
    */
    SliderPropertyComponent (const String& propertyName,
                             double rangeMin,
                             double rangeMax,
                             double interval,
                             double skewFactor = 1.0,
                             bool symmetricSkew = false);

public:
    //==============================================================================
    /** Creates the property component.

        The ranges, interval and skew factor are passed to the Slider component.

        If you need to customise the slider in other ways, your constructor can
        access the slider member variable and change it directly.

        Note that if you call this constructor then you must use the Value to interact with
        the value, and you can't override the class with your own setValue or getValue methods.
        If you want to use those methods, call the other constructor instead.
    */
    SliderPropertyComponent (const Value& valueToControl,
                             const String& propertyName,
                             double rangeMin,
                             double rangeMax,
                             double interval,
                             double skewFactor = 1.0,
                             bool symmetricSkew = false);

    /** Destructor. */
    ~SliderPropertyComponent() override;


    //==============================================================================
    /** Called when the user moves the slider to change its value.

        Your subclass must use this method to update whatever item this property
        represents.
    */
    virtual void setValue (double newValue);

    /** Returns the value that the slider should show. */
    virtual double getValue() const;


    //==============================================================================
    /** @internal */
    void refresh() override;

protected:
    /** The slider component being used in this component.
        Your subclass has access to this in case it needs to customise it in some way.
    */
    Slider slider;

private:
    //==============================================================================
    HIGGS_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SliderPropertyComponent)
};

} // namespace higgs
