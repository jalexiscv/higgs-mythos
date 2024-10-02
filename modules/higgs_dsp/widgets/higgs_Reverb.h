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
    Processor wrapper around higgs::Reverb for easy integration into ProcessorChain.

    @tags{DSP}
*/
class Reverb
{
public:
    //==============================================================================
    /** Creates an uninitialised Reverb processor. Call prepare() before first use. */
    Reverb() = default;

    //==============================================================================
    using Parameters = higgs::Reverb::Parameters;

    /** Returns the reverb's current parameters. */
    const Parameters& getParameters() const noexcept    { return reverb.getParameters(); }

    /** Applies a new set of parameters to the reverb.
        Note that this doesn't attempt to lock the reverb, so if you call this in parallel with
        the process method, you may get artifacts.
    */
    void setParameters (const Parameters& newParams)    { reverb.setParameters (newParams); }

    /** Returns true if the reverb is enabled. */
    bool isEnabled() const noexcept                     { return enabled; }

    /** Enables/disables the reverb. */
    void setEnabled (bool newValue) noexcept            { enabled = newValue; }

    //==============================================================================
    /** Initialises the reverb. */
    void prepare (const ProcessSpec& spec)
    {
        reverb.setSampleRate (spec.sampleRate);
    }

    /** Resets the reverb's internal state. */
    void reset() noexcept
    {
        reverb.reset();
    }

    //==============================================================================
    /** Applies the reverb to a mono or stereo buffer. */
    template <typename ProcessContext>
    void process (const ProcessContext& context) noexcept
    {
        const auto& inputBlock = context.getInputBlock();
        auto& outputBlock = context.getOutputBlock();
        const auto numInChannels = inputBlock.getNumChannels();
        const auto numOutChannels = outputBlock.getNumChannels();
        const auto numSamples = outputBlock.getNumSamples();

        jassert (inputBlock.getNumSamples() == numSamples);

        outputBlock.copyFrom (inputBlock);

        if (! enabled || context.isBypassed)
            return;

        if (numInChannels == 1 && numOutChannels == 1)
        {
            reverb.processMono (outputBlock.getChannelPointer (0), (int) numSamples);
        }
        else if (numInChannels == 2 && numOutChannels == 2)
        {
            reverb.processStereo (outputBlock.getChannelPointer (0),
                                  outputBlock.getChannelPointer (1),
                                  (int) numSamples);
        }
        else
        {
            jassertfalse;   // invalid channel configuration
        }
    }

private:
    //==============================================================================
    higgs::Reverb reverb;
    bool enabled = true;
};

} // namespace higgs::dsp
