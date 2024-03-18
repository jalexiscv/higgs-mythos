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

AudioParameterBool::AudioParameterBool (const ParameterID& idToUse,
                                        const String& nameToUse,
                                        bool def,
                                        const AudioParameterBoolAttributes& attributes)
    : RangedAudioParameter (idToUse, nameToUse, attributes.getAudioProcessorParameterWithIDAttributes()),
      value (def ? 1.0f : 0.0f),
      valueDefault (def),
      stringFromBoolFunction (attributes.getStringFromValueFunction() != nullptr
                                  ? attributes.getStringFromValueFunction()
                                  : [] (bool v, int) { return v ? TRANS ("On") : TRANS ("Off"); }),
      boolFromStringFunction (attributes.getValueFromStringFunction() != nullptr
                                  ? attributes.getValueFromStringFunction()
                                  : [] (const String& text)
                                    {
                                        static const StringArray onStrings { TRANS ("on"), TRANS ("yes"), TRANS ("true") };
                                        static const StringArray offStrings { TRANS ("off"), TRANS ("no"), TRANS ("false") };

                                        String lowercaseText (text.toLowerCase());

                                        for (auto& testText : onStrings)
                                            if (lowercaseText == testText)
                                                return true;

                                        for (auto& testText : offStrings)
                                            if (lowercaseText == testText)
                                                return false;

                                        return text.getIntValue() != 0;
                                    })
{
}

AudioParameterBool::~AudioParameterBool()
{
    #if __cpp_lib_atomic_is_always_lock_free
     static_assert (std::atomic<float>::is_always_lock_free,
                    "AudioParameterBool requires a lock-free std::atomic<float>");
    #endif
}

float AudioParameterBool::getValue() const                               { return value; }
void AudioParameterBool::setValue (float newValue)                       { value = newValue; valueChanged (get()); }
float AudioParameterBool::getDefaultValue() const                        { return valueDefault; }
int AudioParameterBool::getNumSteps() const                              { return 2; }
bool AudioParameterBool::isDiscrete() const                              { return true; }
bool AudioParameterBool::isBoolean() const                               { return true; }
void AudioParameterBool::valueChanged (bool)                             {}

float AudioParameterBool::getValueForText (const String& text) const
{
    return boolFromStringFunction (text) ? 1.0f : 0.0f;
}

String AudioParameterBool::getText (float v, int maximumLength) const
{
    return stringFromBoolFunction (v >= 0.5f, maximumLength);
}

AudioParameterBool& AudioParameterBool::operator= (bool newValue)
{
    if (get() != newValue)
        setValueNotifyingHost (newValue ? 1.0f : 0.0f);

    return *this;
}

} // namespace higgs
