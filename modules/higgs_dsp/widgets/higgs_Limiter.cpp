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
void Limiter<SampleType>::setThreshold (SampleType newThreshold)
{
    thresholddB = newThreshold;
    update();
}

template <typename SampleType>
void Limiter<SampleType>::setRelease (SampleType newRelease)
{
    releaseTime = newRelease;
    update();
}

//==============================================================================
template <typename SampleType>
void Limiter<SampleType>::prepare (const ProcessSpec& spec)
{
    jassert (spec.sampleRate > 0);
    jassert (spec.numChannels > 0);

    sampleRate = spec.sampleRate;

    firstStageCompressor.prepare (spec);
    secondStageCompressor.prepare (spec);

    update();
    reset();
}

template <typename SampleType>
void Limiter<SampleType>::reset()
{
    firstStageCompressor.reset();
    secondStageCompressor.reset();

    outputVolume.reset (sampleRate, 0.001);
}

//==============================================================================
template <typename SampleType>
void Limiter<SampleType>::update()
{
    firstStageCompressor.setThreshold ((SampleType) -10.0);
    firstStageCompressor.setRatio     ((SampleType) 4.0);
    firstStageCompressor.setAttack    ((SampleType) 2.0);
    firstStageCompressor.setRelease   ((SampleType) 200.0);

    secondStageCompressor.setThreshold (thresholddB);
    secondStageCompressor.setRatio     ((SampleType) 1000.0);
    secondStageCompressor.setAttack    ((SampleType) 0.001);
    secondStageCompressor.setRelease   (releaseTime);

    auto ratioInverse = (SampleType) (1.0 / 4.0);

    auto gain = (SampleType) std::pow (10.0, 10.0 * (1.0 - ratioInverse) / 40.0);
    gain *= Decibels::decibelsToGain (-thresholddB, (SampleType) -100.0);

    outputVolume.setTargetValue (gain);
}

//==============================================================================
template class Limiter<float>;
template class Limiter<double>;

} // namespace higgs::dsp
