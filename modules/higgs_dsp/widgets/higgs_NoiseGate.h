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

/**
    A simple noise gate with standard threshold, ratio, attack time and
    release time controls. Can be used as an expander if the ratio is low.

    @tags{DSP}
*/
template <typename SampleType>
class NoiseGate
{
public:
    //==============================================================================
    /** Constructor. */
    NoiseGate();

    //==============================================================================
    /** Sets the threshold in dB of the noise-gate.*/
    void setThreshold (SampleType newThreshold);

    /** Sets the ratio of the noise-gate (must be higher or equal to 1).*/
    void setRatio (SampleType newRatio);

    /** Sets the attack time in milliseconds of the noise-gate.*/
    void setAttack (SampleType newAttack);

    /** Sets the release time in milliseconds of the noise-gate.*/
    void setRelease (SampleType newRelease);

    //==============================================================================
    /** Initialises the processor. */
    void prepare (const ProcessSpec& spec);

    /** Resets the internal state variables of the processor. */
    void reset();

    //==============================================================================
    /** Processes the input and output samples supplied in the processing context. */
    template <typename ProcessContext>
    void process (const ProcessContext& context) noexcept
    {
        const auto& inputBlock = context.getInputBlock();
        auto& outputBlock      = context.getOutputBlock();
        const auto numChannels = outputBlock.getNumChannels();
        const auto numSamples  = outputBlock.getNumSamples();

        jassert (inputBlock.getNumChannels() == numChannels);
        jassert (inputBlock.getNumSamples() == numSamples);

        if (context.isBypassed)
        {
            outputBlock.copyFrom (inputBlock);
            return;
        }

        for (size_t channel = 0; channel < numChannels; ++channel)
        {
            auto* inputSamples  = inputBlock .getChannelPointer (channel);
            auto* outputSamples = outputBlock.getChannelPointer (channel);

            for (size_t i = 0; i < numSamples; ++i)
                outputSamples[i] = processSample ((int) channel, inputSamples[i]);
        }
    }

    /** Performs the processing operation on a single sample at a time. */
    SampleType processSample (int channel, SampleType inputValue);

private:
    //==============================================================================
    void update();

    //==============================================================================
    SampleType threshold, thresholdInverse, currentRatio;
    BallisticsFilter<SampleType> envelopeFilter, RMSFilter;

    double sampleRate = 44100.0;
    SampleType thresholddB = -100, ratio = 10.0, attackTime = 1.0, releaseTime = 100.0;
};

} // namespace higgs::dsp
