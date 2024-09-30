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
template <typename SampleType>
NoiseGate<SampleType>::NoiseGate()
{
    update();

    RMSFilter.setLevelCalculationType (BallisticsFilterLevelCalculationType::RMS);
    RMSFilter.setAttackTime  (static_cast<SampleType> (0.0));
    RMSFilter.setReleaseTime (static_cast<SampleType> (50.0));
}

template <typename SampleType>
void NoiseGate<SampleType>::setThreshold (SampleType newValue)
{
    thresholddB = newValue;
    update();
}

template <typename SampleType>
void NoiseGate<SampleType>::setRatio (SampleType newRatio)
{
    jassert (newRatio >= static_cast<SampleType> (1.0));

    ratio = newRatio;
    update();
}

template <typename SampleType>
void NoiseGate<SampleType>::setAttack (SampleType newAttack)
{
    attackTime = newAttack;
    update();
}

template <typename SampleType>
void NoiseGate<SampleType>::setRelease (SampleType newRelease)
{
    releaseTime = newRelease;
    update();
}

//==============================================================================
template <typename SampleType>
void NoiseGate<SampleType>::prepare (const ProcessSpec& spec)
{
    jassert (spec.sampleRate > 0);
    jassert (spec.numChannels > 0);

    sampleRate = spec.sampleRate;

    RMSFilter.prepare (spec);
    envelopeFilter.prepare (spec);

    update();
    reset();
}

template <typename SampleType>
void NoiseGate<SampleType>::reset()
{
    RMSFilter.reset();
    envelopeFilter.reset();
}

//==============================================================================
template <typename SampleType>
SampleType NoiseGate<SampleType>::processSample (int channel, SampleType sample)
{
    // RMS ballistics filter
    auto env = RMSFilter.processSample (channel, sample);

    // Ballistics filter
    env = envelopeFilter.processSample (channel, env);

    // VCA
    auto gain = (env > threshold) ? static_cast<SampleType> (1.0)
                                  : std::pow (env * thresholdInverse, currentRatio - static_cast<SampleType> (1.0));

    // Output
    return gain * sample;
}

template <typename SampleType>
void NoiseGate<SampleType>::update()
{
    threshold = Decibels::decibelsToGain (thresholddB, static_cast<SampleType> (-200.0));
    thresholdInverse = static_cast<SampleType> (1.0) / threshold;
    currentRatio = ratio;

    envelopeFilter.setAttackTime  (attackTime);
    envelopeFilter.setReleaseTime (releaseTime);
}

//==============================================================================
template class NoiseGate<float>;
template class NoiseGate<double>;

} // namespace higgs::dsp
