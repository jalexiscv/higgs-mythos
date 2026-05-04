/*
  ==============================================================================

   This file is part of the HIGGS framework examples.
   Copyright (c) Raw Material Software Limited

   The code included in this file is provided under the terms of the ISC license
   http://www.isc.org/downloads/software-support-policy/isc-license. Permission
   to use, copy, modify, and/or distribute this software for any purpose with or
   without fee is hereby granted provided that the above copyright notice and
   this permission notice appear in all copies.

   THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES WITH
   REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY
   AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT,
   INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM
   LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR
   OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
   PERFORMANCE OF THIS SOFTWARE.

  ==============================================================================
*/

/*******************************************************************************
 The block below describes the properties of this PIP. A PIP is a short snippet
 of code that can be read by the MythosStudio and used to generate a HIGGS project.

 BEGIN_HIGGS_PIP_METADATA

 name:             SIMDRegisterDemo
 version:          1.0.0
 vendor:           HIGGS
 website:          http://higgsmythos.com
 description:      SIMD register demo using the DSP module.

 dependencies:     higgs_audio_basics, higgs_audio_devices, higgs_audio_formats,
                   higgs_audio_processors, higgs_audio_utils, higgs_core,
                   higgs_data_structures, higgs_dsp, higgs_events, higgs_graphics,
                   higgs_gui_basics, higgs_gui_extra, higgs_audio_processors_headless
 exporters:        xcode_mac, vs2022, vs2026, linux_make

 moduleFlags:      HIGGS_STRICT_REFCOUNTEDPOINTER=1

 type:             Component
 mainClass:        SIMDRegisterDemo

 useLocalCopy:     1

 END_HIGGS_PIP_METADATA

*******************************************************************************/

#pragma once

#include "../Assets/DemoUtilities.h"
#include "../Assets/DSPDemos_Common.h"

using namespace dsp;

//==============================================================================
struct SIMDRegisterDemoDSP
{
    void prepare (const ProcessSpec& spec)
    {
        sampleRate = spec.sampleRate;

        iirCoefficients = IIR::Coefficients<float>::makeLowPass (sampleRate, 440.0f);
        iir = IIR::Filter<SIMDRegister<float>> (iirCoefficients);

        static constexpr auto registerSize = SIMDRegister<float>::size();
        const auto numChannelsSIMD = (spec.numChannels + registerSize - 1) / registerSize;
        interleaved = AudioBlock<SIMDRegister<float>> (interleavedBlockData, numChannelsSIMD, spec.maximumBlockSize);

        scratchChannel.clear();
        scratchChannel.resize (spec.maximumBlockSize);

        auto specSIMD = spec;
        specSIMD.numChannels = (uint32_t) numChannelsSIMD;
        iir.prepare (specSIMD);
    }

    void process (const ProcessContextReplacing<float>& context)
    {
        jassert (context.getInputBlock().getNumSamples()  == context.getOutputBlock().getNumSamples());
        jassert (context.getInputBlock().getNumChannels() == context.getOutputBlock().getNumChannels());

        SIMDInterleavingHelpers::interleave (context.getInputBlock(), interleaved, scratchChannel);

        iir.process (ProcessContextReplacing (interleaved));

        SIMDInterleavingHelpers::deinterleave (interleaved, context.getOutputBlock(), scratchChannel);
    }

    void reset()
    {
        iir.reset();
    }

    void updateParameters()
    {
        if (approximatelyEqual (sampleRate, 0.0))
            return;

        auto cutoff = static_cast<float> (cutoffParam.getCurrentValue());
        auto qVal   = static_cast<float> (qParam.getCurrentValue());

        switch (typeParam.getCurrentSelectedID())
        {
            case 1:   *iirCoefficients = IIR::ArrayCoefficients<float>::makeLowPass  (sampleRate, cutoff, qVal); break;
            case 2:   *iirCoefficients = IIR::ArrayCoefficients<float>::makeHighPass (sampleRate, cutoff, qVal); break;
            case 3:   *iirCoefficients = IIR::ArrayCoefficients<float>::makeBandPass (sampleRate, cutoff, qVal); break;
            default:  break;
        }
    }

    //==============================================================================
    IIR::Coefficients<float>::Ptr iirCoefficients;
    IIR::Filter<SIMDRegister<float>> iir;

    AudioBlock<SIMDRegister<float>> interleaved;
    std::vector<float> scratchChannel;

    HeapBlock<char> interleavedBlockData;

    ChoiceParameter typeParam { { "Low-pass", "High-pass", "Band-pass" }, 1, "Type" };
    SliderParameter cutoffParam { { 20.0, 20000.0 }, 0.5, 440.0f, "Cutoff", "Hz" };
    SliderParameter qParam { { 0.3, 20.0 }, 0.5, 0.7, "Q" };

    std::vector<DSPDemoParameterBase*> parameters { &typeParam, &cutoffParam, &qParam };
    double sampleRate = 0.0;
};

struct SIMDRegisterDemo final : public Component
{
    SIMDRegisterDemo()
    {
        addAndMakeVisible (fileReaderComponent);
        setSize (750, 500);
    }

    void resized() override
    {
        fileReaderComponent.setBounds (getLocalBounds());
    }

    AudioFileReaderComponent<SIMDRegisterDemoDSP> fileReaderComponent;
};
