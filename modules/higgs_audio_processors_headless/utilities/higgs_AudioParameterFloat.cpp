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

AudioParameterFloat::AudioParameterFloat (const ParameterID& idToUse,
                                          const String& nameToUse,
                                          NormalisableRange<float> r,
                                          float def,
                                          const AudioParameterFloatAttributes& attributes)
    : RangedAudioParameter (idToUse, nameToUse, attributes.getAudioProcessorParameterWithIDAttributes()),
      range (r),
      value (def),
      valueDefault (def),
      stringFromValueFunction (attributes.getStringFromValueFunction()),
      valueFromStringFunction (attributes.getValueFromStringFunction())
{
    if (stringFromValueFunction == nullptr)
    {
        auto numDecimalPlacesToDisplay = [this]
        {
            int numDecimalPlaces = 7;

            if (! approximatelyEqual (range.interval, 0.0f))
            {
                if (approximatelyEqual (std::abs (range.interval - std::floor (range.interval)), 0.0f))
                    return 0;

                auto v = std::abs (roundToInt (range.interval * pow (10, numDecimalPlaces)));

                while ((v % 10) == 0 && numDecimalPlaces > 0)
                {
                    --numDecimalPlaces;
                    v /= 10;
                }
            }

            return numDecimalPlaces;
        }();

        stringFromValueFunction = [numDecimalPlacesToDisplay] (float v, int length)
        {
            String asText (v, numDecimalPlacesToDisplay);
            return length > 0 ? asText.substring (0, length) : asText;
        };
    }

    if (valueFromStringFunction == nullptr)
        valueFromStringFunction = [] (const String& text) { return text.getFloatValue(); };
}

AudioParameterFloat::AudioParameterFloat (const ParameterID& pid, const String& nm, float minValue, float maxValue, float def)
   : AudioParameterFloat (pid, nm, { minValue, maxValue, 0.01f }, def)
{
}

AudioParameterFloat::~AudioParameterFloat()
{
    #if __cpp_lib_atomic_is_always_lock_free
     static_assert (std::atomic<float>::is_always_lock_free,
                    "AudioParameterFloat requires a lock-free std::atomic<float>");
    #endif
}

float AudioParameterFloat::getValue() const                              { return convertTo0to1 (value); }
void AudioParameterFloat::setValue (float newValue)                      { value = convertFrom0to1 (newValue); valueChanged (get()); }
float AudioParameterFloat::getDefaultValue() const                       { return convertTo0to1 (valueDefault); }
int AudioParameterFloat::getNumSteps() const                             { return AudioProcessorParameterWithID::getNumSteps(); }
String AudioParameterFloat::getText (float v, int length) const          { return stringFromValueFunction (convertFrom0to1 (v), length); }
float AudioParameterFloat::getValueForText (const String& text) const    { return convertTo0to1 (valueFromStringFunction (text)); }
void AudioParameterFloat::valueChanged (float)                           {}

AudioParameterFloat& AudioParameterFloat::operator= (float newValue)
{
    if (! approximatelyEqual ((float) value, newValue))
        setValueNotifyingHost (convertTo0to1 (newValue));

    return *this;
}

} // namespace higgs
