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
FirstOrderTPTFilter<SampleType>::FirstOrderTPTFilter()
{
    update();
}

//==============================================================================
template <typename SampleType>
void FirstOrderTPTFilter<SampleType>::setType (Type newValue)
{
    filterType = newValue;
}

template <typename SampleType>
void FirstOrderTPTFilter<SampleType>::setCutoffFrequency (SampleType newValue)
{
    jassert (isPositiveAndBelow (newValue, static_cast<SampleType> (sampleRate * 0.5)));

    cutoffFrequency = newValue;
    update();
}

//==============================================================================
template <typename SampleType>
void FirstOrderTPTFilter<SampleType>::prepare (const ProcessSpec& spec)
{
    jassert (spec.sampleRate > 0);
    jassert (spec.numChannels > 0);

    sampleRate = spec.sampleRate;
    s1.resize (spec.numChannels);

    update();
    reset();
}

template <typename SampleType>
void FirstOrderTPTFilter<SampleType>::reset()
{
    reset (static_cast<SampleType> (0));
}

template <typename SampleType>
void FirstOrderTPTFilter<SampleType>::reset (SampleType newValue)
{
    std::fill (s1.begin(), s1.end(), newValue);
}

//==============================================================================
template <typename SampleType>
SampleType FirstOrderTPTFilter<SampleType>::processSample (int channel, SampleType inputValue)
{
    auto& s = s1[(size_t) channel];

    auto v = G * (inputValue - s);
    auto y = v + s;
    s = y + v;

    switch (filterType)
    {
        case Type::lowpass:   return y;
        case Type::highpass:  return inputValue - y;
        case Type::allpass:   return 2 * y - inputValue;
        default:              break;
    }

    jassertfalse;
    return y;
}

template <typename SampleType>
void FirstOrderTPTFilter<SampleType>::snapToZero() noexcept
{
    for (auto& s : s1)
        util::snapToZero (s);
}

//==============================================================================
template <typename SampleType>
void FirstOrderTPTFilter<SampleType>::update()
{
    auto g = SampleType (std::tan (higgs::MathConstants<double>::pi * cutoffFrequency / sampleRate));
    G = g / (1 + g);
}

//==============================================================================
template class FirstOrderTPTFilter<float>;
template class FirstOrderTPTFilter<double>;

} // namespace higgs::dsp
