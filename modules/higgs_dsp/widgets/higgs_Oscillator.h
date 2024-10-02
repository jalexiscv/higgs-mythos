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
    Generates a signal based on a user-supplied function.

    @tags{DSP}
*/
template <typename SampleType>
class Oscillator
{
public:
    /** The NumericType is the underlying primitive type used by the SampleType (which
        could be either a primitive or vector)
    */
    using NumericType = typename SampleTypeHelpers::ElementType<SampleType>::Type;

    /** Creates an uninitialised oscillator. Call initialise before first use. */
    Oscillator() = default;

    /** Creates an oscillator with a periodic input function (-pi..pi).

        If lookup table is not zero, then the function will be approximated
        with a lookup table.
    */
    Oscillator (const std::function<NumericType (NumericType)>& function,
                size_t lookupTableNumPoints = 0)
    {
        initialise (function, lookupTableNumPoints);
    }

    /** Returns true if the Oscillator has been initialised. */
    bool isInitialised() const noexcept     { return static_cast<bool> (generator); }

    /** Initialises the oscillator with a waveform. */
    void initialise (const std::function<NumericType (NumericType)>& function,
                     size_t lookupTableNumPoints = 0)
    {
        if (lookupTableNumPoints != 0)
        {
            auto* table = new LookupTableTransform<NumericType> (function,
                                                                 -MathConstants<NumericType>::pi,
                                                                 MathConstants<NumericType>::pi,
                                                                 lookupTableNumPoints);

            lookupTable.reset (table);
            generator = [table] (NumericType x) { return (*table) (x); };
        }
        else
        {
            generator = function;
        }
    }

    //==============================================================================
    /** Sets the frequency of the oscillator. */
    void setFrequency (NumericType newFrequency, bool force = false) noexcept
    {
        if (force)
        {
            frequency.setCurrentAndTargetValue (newFrequency);
            return;
        }

        frequency.setTargetValue (newFrequency);
    }

    /** Returns the current frequency of the oscillator. */
    NumericType getFrequency() const noexcept                    { return frequency.getTargetValue(); }

    //==============================================================================
    /** Called before processing starts. */
    void prepare (const ProcessSpec& spec) noexcept
    {
        sampleRate = static_cast<NumericType> (spec.sampleRate);
        rampBuffer.resize ((int) spec.maximumBlockSize);

        reset();
    }

    /** Resets the internal state of the oscillator */
    void reset() noexcept
    {
        phase.reset();

        if (sampleRate > 0)
            frequency.reset (sampleRate, 0.05);
    }

    //==============================================================================
    /** Returns the result of processing a single sample. */
    SampleType HIGGS_VECTOR_CALLTYPE processSample (SampleType input) noexcept
    {
        jassert (isInitialised());
        auto increment = MathConstants<NumericType>::twoPi * frequency.getNextValue() / sampleRate;
        return input + generator (phase.advance (increment) - MathConstants<NumericType>::pi);
    }

    /** Processes the input and output buffers supplied in the processing context. */
    template <typename ProcessContext>
    void process (const ProcessContext& context) noexcept
    {
        jassert (isInitialised());
        auto&& outBlock = context.getOutputBlock();
        auto&& inBlock  = context.getInputBlock();

        // this is an output-only processor
        jassert (outBlock.getNumSamples() <= static_cast<size_t> (rampBuffer.size()));

        auto len           = outBlock.getNumSamples();
        auto numChannels   = outBlock.getNumChannels();
        auto inputChannels = inBlock.getNumChannels();
        auto baseIncrement = MathConstants<NumericType>::twoPi / sampleRate;

        if (context.isBypassed)
            context.getOutputBlock().clear();

        if (frequency.isSmoothing())
        {
            auto* buffer = rampBuffer.getRawDataPointer();

            for (size_t i = 0; i < len; ++i)
                buffer[i] = phase.advance (baseIncrement * frequency.getNextValue())
                              - MathConstants<NumericType>::pi;

            if (! context.isBypassed)
            {
                size_t ch;

                if (context.usesSeparateInputAndOutputBlocks())
                {
                    for (ch = 0; ch < jmin (numChannels, inputChannels); ++ch)
                    {
                        auto* dst = outBlock.getChannelPointer (ch);
                        auto* src = inBlock.getChannelPointer (ch);

                        for (size_t i = 0; i < len; ++i)
                            dst[i] = src[i] + generator (buffer[i]);
                    }
                }
                else
                {
                    for (ch = 0; ch < jmin (numChannels, inputChannels); ++ch)
                    {
                        auto* dst = outBlock.getChannelPointer (ch);

                        for (size_t i = 0; i < len; ++i)
                            dst[i] += generator (buffer[i]);
                    }
                }

                for (; ch < numChannels; ++ch)
                {
                    auto* dst = outBlock.getChannelPointer (ch);

                    for (size_t i = 0; i < len; ++i)
                        dst[i] = generator (buffer[i]);
                }
            }
        }
        else
        {
            auto freq = baseIncrement * frequency.getNextValue();
            auto p = phase;

            if (context.isBypassed)
            {
                frequency.skip (static_cast<int> (len));
                p.advance (freq * static_cast<NumericType> (len));
            }
            else
            {
                size_t ch;

                if (context.usesSeparateInputAndOutputBlocks())
                {
                    for (ch = 0; ch < jmin (numChannels, inputChannels); ++ch)
                    {
                        p = phase;
                        auto* dst = outBlock.getChannelPointer (ch);
                        auto* src = inBlock.getChannelPointer (ch);

                        for (size_t i = 0; i < len; ++i)
                            dst[i] = src[i] + generator (p.advance (freq) - MathConstants<NumericType>::pi);
                    }
                }
                else
                {
                    for (ch = 0; ch < jmin (numChannels, inputChannels); ++ch)
                    {
                        p = phase;
                        auto* dst = outBlock.getChannelPointer (ch);

                        for (size_t i = 0; i < len; ++i)
                            dst[i] += generator (p.advance (freq) - MathConstants<NumericType>::pi);
                    }
                }

                for (; ch < numChannels; ++ch)
                {
                    p = phase;
                    auto* dst = outBlock.getChannelPointer (ch);

                    for (size_t i = 0; i < len; ++i)
                        dst[i] = generator (p.advance (freq) - MathConstants<NumericType>::pi);
                }
            }

            phase = p;
        }
    }

private:
    //==============================================================================
    std::function<NumericType (NumericType)> generator;
    std::unique_ptr<LookupTableTransform<NumericType>> lookupTable;
    Array<NumericType> rampBuffer;
    SmoothedValue<NumericType> frequency { static_cast<NumericType> (440.0) };
    NumericType sampleRate = 48000.0;
    Phase<NumericType> phase;
};

} // namespace higgs::dsp
