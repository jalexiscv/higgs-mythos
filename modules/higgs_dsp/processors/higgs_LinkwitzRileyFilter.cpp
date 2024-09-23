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
LinkwitzRileyFilter<SampleType>::LinkwitzRileyFilter()
{
    update();
}

//==============================================================================
template <typename SampleType>
void LinkwitzRileyFilter<SampleType>::setType (Type newType)
{
    filterType = newType;
}

template <typename SampleType>
void LinkwitzRileyFilter<SampleType>::setCutoffFrequency (SampleType newCutoffFrequencyHz)
{
    jassert (isPositiveAndBelow (newCutoffFrequencyHz, static_cast<SampleType> (sampleRate * 0.5)));

    cutoffFrequency = newCutoffFrequencyHz;
    update();
}

//==============================================================================
template <typename SampleType>
void LinkwitzRileyFilter<SampleType>::prepare (const ProcessSpec& spec)
{
    jassert (spec.sampleRate > 0);
    jassert (spec.numChannels > 0);

    sampleRate = spec.sampleRate;
    update();

    s1.resize (spec.numChannels);
    s2.resize (spec.numChannels);
    s3.resize (spec.numChannels);
    s4.resize (spec.numChannels);

    reset();
}

template <typename SampleType>
void LinkwitzRileyFilter<SampleType>::reset()
{
    for (auto s : { &s1, &s2, &s3, &s4 })
        std::fill (s->begin(), s->end(), static_cast<SampleType> (0));
}

template <typename SampleType>
void LinkwitzRileyFilter<SampleType>::snapToZero() noexcept
{
    for (auto s : { &s1, &s2, &s3, &s4 })
        for (auto& element : *s)
            util::snapToZero (element);
}

//==============================================================================
template <typename SampleType>
SampleType LinkwitzRileyFilter<SampleType>::processSample (int channel, SampleType inputValue)
{
    auto yH = (inputValue - (R2 + g) * s1[(size_t) channel] - s2[(size_t) channel]) * h;

    auto yB = g * yH + s1[(size_t) channel];
    s1[(size_t) channel] = g * yH + yB;

    auto yL = g * yB + s2[(size_t) channel];
    s2[(size_t) channel] = g * yB + yL;

    if (filterType == Type::allpass)
        return yL - R2 * yB + yH;

    auto yH2 = ((filterType == Type::lowpass ? yL : yH) - (R2 + g) * s3[(size_t) channel] - s4[(size_t) channel]) * h;

    auto yB2 = g * yH2 + s3[(size_t) channel];
    s3[(size_t) channel] = g * yH2 + yB2;

    auto yL2 = g * yB2 + s4[(size_t) channel];
    s4[(size_t) channel] = g * yB2 + yL2;

    return filterType == Type::lowpass ? yL2 : yH2;
}

template <typename SampleType>
void LinkwitzRileyFilter<SampleType>::processSample (int channel, SampleType inputValue, SampleType &outputLow, SampleType &outputHigh)
{
    auto yH = (inputValue - (R2 + g) * s1[(size_t) channel] - s2[(size_t) channel]) * h;

    auto yB = g * yH + s1[(size_t) channel];
    s1[(size_t) channel] = g * yH + yB;

    auto yL = g * yB + s2[(size_t) channel];
    s2[(size_t) channel] = g * yB + yL;

    auto yH2 = (yL - (R2 + g) * s3[(size_t) channel] - s4[(size_t) channel]) * h;

    auto yB2 = g * yH2 + s3[(size_t) channel];
    s3[(size_t) channel] = g * yH2 + yB2;

    auto yL2 = g * yB2 + s4[(size_t) channel];
    s4[(size_t) channel] = g * yB2 + yL2;

    outputLow = yL2;
    outputHigh = yL - R2 * yB + yH - yL2;
}

template <typename SampleType>
void LinkwitzRileyFilter<SampleType>::update()
{
    g  = (SampleType) std::tan (MathConstants<double>::pi * cutoffFrequency / sampleRate);
    R2 = (SampleType) std::sqrt (2.0);
    h  = (SampleType) (1.0 / (1.0 + R2 * g + g * g));
}

//==============================================================================
template class LinkwitzRileyFilter<float>;
template class LinkwitzRileyFilter<double>;

} // namespace higgs::dsp
