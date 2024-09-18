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

namespace higgs::dsp
{

//==============================================================================
template <typename SampleType, typename InterpolationType>
DelayLine<SampleType, InterpolationType>::DelayLine()
    : DelayLine (0)
{
}

template <typename SampleType, typename InterpolationType>
DelayLine<SampleType, InterpolationType>::DelayLine (int maximumDelayInSamples)
{
    jassert (maximumDelayInSamples >= 0);

    sampleRate = 44100.0;

    setMaximumDelayInSamples (maximumDelayInSamples);
}

//==============================================================================
template <typename SampleType, typename InterpolationType>
void DelayLine<SampleType, InterpolationType>::setDelay (SampleType newDelayInSamples)
{
    auto upperLimit = (SampleType) getMaximumDelayInSamples();
    jassert (isPositiveAndNotGreaterThan (newDelayInSamples, upperLimit));

    delay     = jlimit ((SampleType) 0, upperLimit, newDelayInSamples);
    delayInt  = static_cast<int> (std::floor (delay));
    delayFrac = delay - (SampleType) delayInt;

    updateInternalVariables();
}

template <typename SampleType, typename InterpolationType>
SampleType DelayLine<SampleType, InterpolationType>::getDelay() const
{
    return delay;
}

//==============================================================================
template <typename SampleType, typename InterpolationType>
void DelayLine<SampleType, InterpolationType>::prepare (const ProcessSpec& spec)
{
    jassert (spec.numChannels > 0);

    bufferData.setSize ((int) spec.numChannels, totalSize, false, false, true);

    writePos.resize (spec.numChannels);
    readPos.resize  (spec.numChannels);

    v.resize (spec.numChannels);
    sampleRate = spec.sampleRate;

    reset();
}

template <typename SampleType, typename InterpolationType>
void DelayLine<SampleType, InterpolationType>::setMaximumDelayInSamples (int maxDelayInSamples)
{
    jassert (maxDelayInSamples >= 0);
    totalSize = jmax (4, maxDelayInSamples + 2);
    bufferData.setSize ((int) bufferData.getNumChannels(), totalSize, false, false, true);
    reset();
}

template <typename SampleType, typename InterpolationType>
void DelayLine<SampleType, InterpolationType>::reset()
{
    for (auto vec : { &writePos, &readPos })
        std::fill (vec->begin(), vec->end(), 0);

    std::fill (v.begin(), v.end(), static_cast<SampleType> (0));

    bufferData.clear();
}

//==============================================================================
template <typename SampleType, typename InterpolationType>
void DelayLine<SampleType, InterpolationType>::pushSample (int channel, SampleType sample)
{
    bufferData.setSample (channel, writePos[(size_t) channel], sample);
    writePos[(size_t) channel] = (writePos[(size_t) channel] + totalSize - 1) % totalSize;
}

template <typename SampleType, typename InterpolationType>
SampleType DelayLine<SampleType, InterpolationType>::popSample (int channel, SampleType delayInSamples, bool updateReadPointer)
{
    if (delayInSamples >= 0)
        setDelay (delayInSamples);

    auto result = interpolateSample (channel);

    if (updateReadPointer)
        readPos[(size_t) channel] = (readPos[(size_t) channel] + totalSize - 1) % totalSize;

    return result;
}

//==============================================================================
template class DelayLine<float,  DelayLineInterpolationTypes::None>;
template class DelayLine<double, DelayLineInterpolationTypes::None>;
template class DelayLine<float,  DelayLineInterpolationTypes::Linear>;
template class DelayLine<double, DelayLineInterpolationTypes::Linear>;
template class DelayLine<float,  DelayLineInterpolationTypes::Lagrange3rd>;
template class DelayLine<double, DelayLineInterpolationTypes::Lagrange3rd>;
template class DelayLine<float,  DelayLineInterpolationTypes::Thiran>;
template class DelayLine<double, DelayLineInterpolationTypes::Thiran>;

} // namespace higgs::dsp
