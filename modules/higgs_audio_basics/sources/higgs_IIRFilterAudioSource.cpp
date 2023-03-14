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

IIRFilterAudioSource::IIRFilterAudioSource (AudioSource* const inputSource,
                                            const bool deleteInputWhenDeleted)
    : input (inputSource, deleteInputWhenDeleted)
{
    jassert (inputSource != nullptr);

    for (int i = 2; --i >= 0;)
        iirFilters.add (new IIRFilter());
}

IIRFilterAudioSource::~IIRFilterAudioSource()  {}

//==============================================================================
void IIRFilterAudioSource::setCoefficients (const IIRCoefficients& newCoefficients)
{
    for (int i = iirFilters.size(); --i >= 0;)
        iirFilters.getUnchecked (i)->setCoefficients (newCoefficients);
}

void IIRFilterAudioSource::makeInactive()
{
    for (int i = iirFilters.size(); --i >= 0;)
        iirFilters.getUnchecked (i)->makeInactive();
}

//==============================================================================
void IIRFilterAudioSource::prepareToPlay (int samplesPerBlockExpected, double sampleRate)
{
    input->prepareToPlay (samplesPerBlockExpected, sampleRate);

    for (int i = iirFilters.size(); --i >= 0;)
        iirFilters.getUnchecked (i)->reset();
}

void IIRFilterAudioSource::releaseResources()
{
    input->releaseResources();
}

void IIRFilterAudioSource::getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill)
{
    input->getNextAudioBlock (bufferToFill);

    const int numChannels = bufferToFill.buffer->getNumChannels();

    while (numChannels > iirFilters.size())
        iirFilters.add (new IIRFilter (*iirFilters.getUnchecked (0)));

    for (int i = 0; i < numChannels; ++i)
        iirFilters.getUnchecked (i)
            ->processSamples (bufferToFill.buffer->getWritePointer (i, bufferToFill.startSample),
                              bufferToFill.numSamples);
}

} // namespace higgs
