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
BallisticsFilter<SampleType>::BallisticsFilter()
{
    setAttackTime (attackTime);
    setReleaseTime (releaseTime);
}

template <typename SampleType>
void BallisticsFilter<SampleType>::setAttackTime (SampleType attackTimeMs)
{
    attackTime = attackTimeMs;
    cteAT = calculateLimitedCte (static_cast<SampleType> (attackTime));
}

template <typename SampleType>
void BallisticsFilter<SampleType>::setReleaseTime (SampleType releaseTimeMs)
{
    releaseTime = releaseTimeMs;
    cteRL = calculateLimitedCte (static_cast<SampleType> (releaseTime));
}

template <typename SampleType>
void BallisticsFilter<SampleType>::setLevelCalculationType (LevelCalculationType newLevelType)
{
    levelType = newLevelType;
    reset();
}

template <typename SampleType>
void BallisticsFilter<SampleType>::prepare (const ProcessSpec& spec)
{
    jassert (spec.sampleRate > 0);
    jassert (spec.numChannels > 0);

    sampleRate = spec.sampleRate;
    expFactor  = -2.0 * MathConstants<double>::pi * 1000.0 / sampleRate;

    setAttackTime  (attackTime);
    setReleaseTime (releaseTime);

    yold.resize (spec.numChannels);

    reset();
}

template <typename SampleType>
void BallisticsFilter<SampleType>::reset()
{
    reset (0);
}

template <typename SampleType>
void BallisticsFilter<SampleType>::reset (SampleType initialValue)
{
    for (auto& old : yold)
        old = initialValue;
}

template <typename SampleType>
SampleType BallisticsFilter<SampleType>::processSample (int channel, SampleType inputValue)
{
    jassert (isPositiveAndBelow (channel, yold.size()));

    if (levelType == LevelCalculationType::RMS)
        inputValue *= inputValue;
    else
        inputValue = std::abs (inputValue);

    SampleType cte = (inputValue > yold[(size_t) channel] ? cteAT : cteRL);

    SampleType result = inputValue + cte * (yold[(size_t) channel] - inputValue);
    yold[(size_t) channel] = result;

    if (levelType == LevelCalculationType::RMS)
        return std::sqrt (result);

    return result;
}

template <typename SampleType>
void BallisticsFilter<SampleType>::snapToZero() noexcept
{
    for (auto& old : yold)
        util::snapToZero (old);
}

template <typename SampleType>
SampleType BallisticsFilter<SampleType>::calculateLimitedCte (SampleType timeMs) const noexcept
{
    return timeMs < static_cast<SampleType> (1.0e-3) ? 0
                                                     : static_cast<SampleType> (std::exp (expFactor / timeMs));
}

//==============================================================================
template class BallisticsFilter<float>;
template class BallisticsFilter<double>;

} // namespace higgs::dsp
