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

void AudioIODeviceCallback::audioDeviceIOCallbackWithContext ([[maybe_unused]] const float* const* inputChannelData,
                                                              [[maybe_unused]] int numInputChannels,
                                                              [[maybe_unused]] float* const* outputChannelData,
                                                              [[maybe_unused]] int numOutputChannels,
                                                              [[maybe_unused]] int numSamples,
                                                              [[maybe_unused]] const AudioIODeviceCallbackContext& context) {}

//==============================================================================
AudioIODevice::AudioIODevice (const String& deviceName, const String& deviceTypeName)
    : name (deviceName), typeName (deviceTypeName)
{
}

AudioIODevice::~AudioIODevice() {}

void AudioIODeviceCallback::audioDeviceError (const String&)    {}
bool AudioIODevice::setAudioPreprocessingEnabled (bool)         { return false; }
bool AudioIODevice::hasControlPanel() const                     { return false; }
int  AudioIODevice::getXRunCount() const noexcept               { return -1; }

bool AudioIODevice::showControlPanel()
{
    jassertfalse;    // this should only be called for devices which return true from
                     // their hasControlPanel() method.
    return false;
}

} // namespace higgs
