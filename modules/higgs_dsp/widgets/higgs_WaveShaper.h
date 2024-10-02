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
    Applies waveshaping to audio samples as single samples or AudioBlocks.

    @tags{DSP}
*/
template <typename FloatType, typename Function = FloatType (*) (FloatType)>
struct WaveShaper
{
    Function functionToUse;

    //==============================================================================
    /** Called before processing starts. */
    void prepare (const ProcessSpec&) noexcept   {}

    //==============================================================================
    /** Returns the result of processing a single sample. */
    template <typename SampleType>
    SampleType HIGGS_VECTOR_CALLTYPE processSample (SampleType inputSample) const noexcept
    {
        return functionToUse (inputSample);
    }

    /** Processes the input and output buffers supplied in the processing context. */
    template <typename ProcessContext>
    void process (const ProcessContext& context) const noexcept
    {
        if (context.isBypassed)
        {
            if (context.usesSeparateInputAndOutputBlocks())
                context.getOutputBlock().copyFrom (context.getInputBlock());
        }
        else
        {
            AudioBlock<FloatType>::process (context.getInputBlock(),
                                            context.getOutputBlock(),
                                            functionToUse);
        }
    }

    void reset() noexcept {}
};

//==============================================================================
#if ! ((HIGGS_MAC || HIGGS_IOS) && HIGGS_CLANG && __clang_major__ < 10)
template <typename Functor>
static WaveShaper<typename std::invoke_result<Functor>, Functor> CreateWaveShaper (Functor functionToUse)   { return {functionToUse}; }
#else
template <typename Functor>
static WaveShaper<typename std::result_of<Functor>, Functor> CreateWaveShaper (Functor functionToUse)   { return {functionToUse}; }
#endif

} // namespace higgs::dsp
