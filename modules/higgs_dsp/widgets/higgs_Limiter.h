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
    A simple limiter with standard threshold and release time controls, featuring
    two compressors and a hard clipper at 0 dB.

    @tags{DSP}
*/
template <typename SampleType>
class Limiter
{
public:
    //==============================================================================
    /** Constructor. */
    Limiter() = default;

    //==============================================================================
    /** Sets the threshold in dB of the limiter.*/
    void setThreshold (SampleType newThreshold);

    /** Sets the release time in milliseconds of the limiter.*/
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
        jassert (inputBlock.getNumSamples()  == numSamples);

        if (context.isBypassed)
        {
            outputBlock.copyFrom (inputBlock);
            return;
        }

        firstStageCompressor.process (context);

        auto secondContext = ProcessContextReplacing<SampleType> (outputBlock);
        secondStageCompressor.process (secondContext);

        outputBlock.multiplyBy (outputVolume);

        for (size_t channel = 0; channel < numChannels; ++channel)
        {
            FloatVectorOperations::clip (outputBlock.getChannelPointer (channel), outputBlock.getChannelPointer (channel),
                                         (SampleType) -1.0, (SampleType) 1.0, (int) numSamples);
        }
    }

private:
    //==============================================================================
    void update();

    //==============================================================================
    Compressor<SampleType> firstStageCompressor, secondStageCompressor;
    SmoothedValue<SampleType, ValueSmoothingTypes::Linear> outputVolume;

    double sampleRate = 44100.0;
    SampleType thresholddB = -10.0, releaseTime = 100.0;
};

} // namespace higgs::dsp
