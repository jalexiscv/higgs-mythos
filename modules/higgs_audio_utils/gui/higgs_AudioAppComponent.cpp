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

AudioAppComponent::AudioAppComponent()
    : deviceManager (defaultDeviceManager),
      usingCustomDeviceManager (false)
{
}

AudioAppComponent::AudioAppComponent (AudioDeviceManager& adm)
    : deviceManager (adm),
      usingCustomDeviceManager (true)
{
}

AudioAppComponent::~AudioAppComponent()
{
    // If you hit this then your derived class must call shutdown audio in
    // destructor!
    jassert (audioSourcePlayer.getCurrentSource() == nullptr);
}

void AudioAppComponent::setAudioChannels (int numInputChannels, int numOutputChannels, const XmlElement* const xml)
{
    String audioError;

    if (usingCustomDeviceManager && xml == nullptr)
    {
        auto setup = deviceManager.getAudioDeviceSetup();

        if (setup.inputChannels.countNumberOfSetBits() != numInputChannels
             || setup.outputChannels.countNumberOfSetBits() != numOutputChannels)
        {
            setup.inputChannels.clear();
            setup.outputChannels.clear();

            setup.inputChannels.setRange (0, numInputChannels, true);
            setup.outputChannels.setRange (0, numOutputChannels, true);

            audioError = deviceManager.setAudioDeviceSetup (setup, false);
        }
    }
    else
    {
        audioError = deviceManager.initialise (numInputChannels, numOutputChannels, xml, true);
    }

    jassert (audioError.isEmpty());

    deviceManager.addAudioCallback (&audioSourcePlayer);
    audioSourcePlayer.setSource (this);
}

void AudioAppComponent::shutdownAudio()
{
    audioSourcePlayer.setSource (nullptr);
    deviceManager.removeAudioCallback (&audioSourcePlayer);

    // other audio callbacks may still be using the device
    if (! usingCustomDeviceManager)
        deviceManager.closeAudioDevice();
}

} // namespace higgs
