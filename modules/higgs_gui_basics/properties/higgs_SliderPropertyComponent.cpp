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

SliderPropertyComponent::SliderPropertyComponent (const String& name,
                                                  const double rangeMin,
                                                  const double rangeMax,
                                                  const double interval,
                                                  const double skewFactor,
                                                  bool symmetricSkew)
    : PropertyComponent (name)
{
    addAndMakeVisible (slider);

    slider.setRange (rangeMin, rangeMax, interval);
    slider.setSkewFactor (skewFactor, symmetricSkew);
    slider.setSliderStyle (Slider::LinearBar);

    slider.onValueChange = [this]
    {
        if (! approximatelyEqual (getValue(), slider.getValue()))
            setValue (slider.getValue());
    };
}

SliderPropertyComponent::SliderPropertyComponent (const Value& valueToControl,
                                                  const String& name,
                                                  const double rangeMin,
                                                  const double rangeMax,
                                                  const double interval,
                                                  const double skewFactor,
                                                  bool symmetricSkew)
    : PropertyComponent (name)
{
    addAndMakeVisible (slider);

    slider.setRange (rangeMin, rangeMax, interval);
    slider.setSkewFactor (skewFactor, symmetricSkew);
    slider.setSliderStyle (Slider::LinearBar);

    slider.getValueObject().referTo (valueToControl);
}

SliderPropertyComponent::~SliderPropertyComponent()
{
}

void SliderPropertyComponent::setValue (const double /*newValue*/)
{
}

double SliderPropertyComponent::getValue() const
{
    return slider.getValue();
}

void SliderPropertyComponent::refresh()
{
    slider.setValue (getValue(), dontSendNotification);
}

} // namespace higgs
