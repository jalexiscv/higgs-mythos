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

AudioParameterInt::AudioParameterInt (const ParameterID& idToUse, const String& nameToUse,
                                      int minValue, int maxValue, int def,
                                      const AudioParameterIntAttributes& attributes)
    : RangedAudioParameter (idToUse, nameToUse, attributes.getAudioProcessorParameterWithIDAttributes()),
      range ([minValue, maxValue]
             {
                 NormalisableRange<float> rangeWithInterval { (float) minValue, (float) maxValue,
                                                              [] (float start, float end, float v) { return jlimit (start, end, v * (end - start) + start); },
                                                              [] (float start, float end, float v) { return jlimit (0.0f, 1.0f, (v - start) / (end - start)); },
                                                              [] (float start, float end, float v) { return (float) roundToInt (higgs::jlimit (start, end, v)); } };
                  rangeWithInterval.interval = 1.0f;
                  return rangeWithInterval;
             }()),
      value ((float) def),
      defaultValue (convertTo0to1 ((float) def)),
      stringFromIntFunction (attributes.getStringFromValueFunction() != nullptr
                                 ? attributes.getStringFromValueFunction()
                                 : [] (int v, int) { return String (v); }),
      intFromStringFunction (attributes.getValueFromStringFunction() != nullptr
                                 ? attributes.getValueFromStringFunction()
                                 : [] (const String& text) { return text.getIntValue(); })
{
    jassert (minValue < maxValue); // must have a non-zero range of values!
}

AudioParameterInt::~AudioParameterInt()
{
    #if __cpp_lib_atomic_is_always_lock_free
     static_assert (std::atomic<float>::is_always_lock_free,
                    "AudioParameterInt requires a lock-free std::atomic<float>");
    #endif
}

float AudioParameterInt::getValue() const                                { return convertTo0to1 (value); }
void AudioParameterInt::setValue (float newValue)                        { value = convertFrom0to1 (newValue); valueChanged (get()); }
float AudioParameterInt::getDefaultValue() const                         { return defaultValue; }
int AudioParameterInt::getNumSteps() const                               { return ((int) getNormalisableRange().getRange().getLength()) + 1; }
float AudioParameterInt::getValueForText (const String& text) const      { return convertTo0to1 ((float) intFromStringFunction (text)); }
String AudioParameterInt::getText (float v, int length) const            { return stringFromIntFunction ((int) convertFrom0to1 (v), length); }
void AudioParameterInt::valueChanged (int)                               {}

AudioParameterInt& AudioParameterInt::operator= (int newValue)
{
    if (get() != newValue)
        setValueNotifyingHost (convertTo0to1 ((float) newValue));

    return *this;
}


//==============================================================================
//==============================================================================
#if HIGGS_UNIT_TESTS

struct AudioParameterIntTests final : public UnitTest
{
    AudioParameterIntTests()
        : UnitTest ("AudioParameterInt", UnitTestCategories::audioProcessorParameters)
    {}

    void runTest() override
    {
        beginTest ("Three options switches at the correct points");
        {
            AudioParameterInt intParam ({}, {}, 1, 3, 1);

            intParam.setValueNotifyingHost (0.0f);
            expectEquals (intParam.get(), 1);

            intParam.setValueNotifyingHost (0.2f);
            expectEquals (intParam.get(), 1);

            intParam.setValueNotifyingHost (0.3f);
            expectEquals (intParam.get(), 2);

            intParam.setValueNotifyingHost (0.7f);
            expectEquals (intParam.get(), 2);

            intParam.setValueNotifyingHost (0.8f);
            expectEquals (intParam.get(), 3);

            intParam.setValueNotifyingHost (1.0f);
            expectEquals (intParam.get(), 3);
        }

        beginTest ("Out-of-bounds input");
        {
            AudioParameterInt intParam ({}, {}, -1, 2, 0);

            intParam.setValueNotifyingHost (-0.5f);
            expectEquals (intParam.get(), -1);

            intParam.setValueNotifyingHost (1.5f);
            expectEquals (intParam.get(), 2);

            intParam = -5;
            expectEquals (intParam.get(), -1);

            intParam = 5;
            expectEquals (intParam.get(), 2);
        }
    }
};

static AudioParameterIntTests audioParameterIntTests;

#endif

} // namespace higgs
