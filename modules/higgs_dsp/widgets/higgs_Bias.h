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
    Adds a DC offset (voltage bias) to the audio samples.

    This is a useful preprocessor for asymmetric waveshaping when a waveshaper is
    bookended by a bias on input and a DC-offset removing high pass filter on output.

    This is an extremely simple bias implementation that simply adds a value to a signal.
    More complicated bias behaviours exist in real circuits - for your homework ;).

    @tags{DSP}
*/
template <typename FloatType>
class Bias
{
public:
    Bias() noexcept = default;

    //==============================================================================
    /** Sets the DC bias
        @param newBias DC offset in range [-1, 1]
    */
    void setBias (FloatType newBias) noexcept
    {
        jassert (newBias >= static_cast<FloatType> (-1) && newBias <= static_cast<FloatType> (1));
        bias.setTargetValue (newBias);
    }

    //==============================================================================
    /** Returns the DC bias
        @return DC bias, which should be in the range [-1, 1]
    */
    FloatType getBias() const noexcept              { return bias.getTargetValue(); }

    /** Sets the length of the ramp used for smoothing gain changes. */
    void setRampDurationSeconds (double newDurationSeconds) noexcept
    {
        if (! approximatelyEqual (rampDurationSeconds, newDurationSeconds))
        {
            rampDurationSeconds = newDurationSeconds;
            updateRamp();
        }
    }

    double getRampDurationSeconds() const noexcept  { return rampDurationSeconds; }

    //==============================================================================
    /** Called before processing starts */
    void prepare (const ProcessSpec& spec) noexcept
    {
        sampleRate = spec.sampleRate;
        updateRamp();
    }

    void reset() noexcept
    {
        bias.reset (sampleRate, rampDurationSeconds);
    }

    //==============================================================================
    /** Returns the result of processing a single sample. */
    template <typename SampleType>
    SampleType processSample (SampleType inputSample) noexcept
    {
        return inputSample + bias.getNextValue();
    }

    //==============================================================================
    /** Processes the input and output buffers supplied in the processing context. */
    template <typename ProcessContext>
    void process (const ProcessContext& context) noexcept
    {
        auto&& inBlock  = context.getInputBlock();
        auto&& outBlock = context.getOutputBlock();

        jassert (inBlock.getNumChannels() == outBlock.getNumChannels());
        jassert (inBlock.getNumSamples()  == outBlock.getNumSamples());

        auto len         = inBlock.getNumSamples();
        auto numChannels = inBlock.getNumChannels();

        if (context.isBypassed)
        {
            bias.skip (static_cast<int> (len));

            if (context.usesSeparateInputAndOutputBlocks())
                outBlock.copyFrom (inBlock);

            return;
        }

        if (numChannels == 1)
        {
            auto* src = inBlock.getChannelPointer (0);
            auto* dst = outBlock.getChannelPointer (0);

            for (size_t i = 0; i < len; ++i)
                dst[i] = src[i] + bias.getNextValue();
        }
        else
        {
            HIGGS_BEGIN_IGNORE_WARNINGS_MSVC (6255 6386)
            auto* biases = static_cast<FloatType*> (alloca (sizeof (FloatType) * len));

            for (size_t i = 0; i < len; ++i)
                biases[i] = bias.getNextValue();

            for (size_t chan = 0; chan < numChannels; ++chan)
                FloatVectorOperations::add (outBlock.getChannelPointer (chan),
                                            inBlock.getChannelPointer (chan),
                                            biases, static_cast<int> (len));
            HIGGS_END_IGNORE_WARNINGS_MSVC
        }
    }


private:
    //==============================================================================
    SmoothedValue<FloatType> bias;
    double sampleRate = 0, rampDurationSeconds = 0;

    void updateRamp() noexcept
    {
        if (sampleRate > 0)
            bias.reset (sampleRate, rampDurationSeconds);
    }
};

} // namespace higgs::dsp
