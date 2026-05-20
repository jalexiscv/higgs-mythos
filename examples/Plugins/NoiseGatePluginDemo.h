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

 name:             NoiseGatePlugin
 version:          1.0.0
 vendor:           HIGGS
 website:          http://higgsmythos.com
 description:      Noise gate audio plugin.

 dependencies:     higgs_audio_basics, higgs_audio_devices, higgs_audio_formats,
                   higgs_audio_plugin_client, higgs_audio_processors,
                   higgs_audio_utils, higgs_core, higgs_data_structures,
                   higgs_events, higgs_graphics, higgs_gui_basics, higgs_gui_extra,
                   higgs_audio_processors_headless
 exporters:        xcode_mac, vs2022, vs2026

 moduleFlags:      HIGGS_STRICT_REFCOUNTEDPOINTER=1

 type:             AudioProcessor
 mainClass:        NoiseGate

 useLocalCopy:     1

 END_HIGGS_PIP_METADATA

*******************************************************************************/

#pragma once


//==============================================================================
class NoiseGate final : public AudioProcessor
{
public:
    //==============================================================================
    NoiseGate()
        : AudioProcessor (BusesProperties().withInput  ("Input",     AudioChannelSet::stereo())
                                           .withOutput ("Output",    AudioChannelSet::stereo())
                                           .withInput  ("Sidechain", AudioChannelSet::stereo()))
    {
        addParameter (threshold = new AudioParameterFloat ({ "threshold", 1 }, "Threshold", 0.0f, 1.0f, 0.5f));
        addParameter (alpha     = new AudioParameterFloat ({ "alpha",     1 }, "Alpha",     0.0f, 1.0f, 0.8f));
    }

    //==============================================================================
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override
    {
        // the sidechain can take any layout, the main bus needs to be the same on the input and output
        return layouts.getMainInputChannelSet() == layouts.getMainOutputChannelSet()
                 && ! layouts.getMainInputChannelSet().isDisabled();
    }

    //==============================================================================
    void prepareToPlay (double, int) override { lowPassCoeff = 0.0f; sampleCountDown = 0; }
    void releaseResources() override {}

    void processBlock (AudioBuffer<float>& buffer, MidiBuffer&) override
    {
        auto mainInputOutput = getBusBuffer (buffer, true, 0);
        auto sideChainInput  = getBusBuffer (buffer, true, 1);

        auto alphaCopy     = alpha->get();
        auto thresholdCopy = threshold->get();

        for (int j = 0; j < buffer.getNumSamples(); ++j)
        {
            auto mixedSamples = 0.0f;

            for (int i = 0; i < sideChainInput.getNumChannels(); ++i)
                mixedSamples += sideChainInput.getReadPointer (i)[j];

            mixedSamples /= static_cast<float> (sideChainInput.getNumChannels());
            lowPassCoeff = (alphaCopy * lowPassCoeff) + ((1.0f - alphaCopy) * mixedSamples);

            if (lowPassCoeff >= thresholdCopy)
                sampleCountDown = (int) getSampleRate();

            // very in-effective way of doing this
            for (int i = 0; i < mainInputOutput.getNumChannels(); ++i)
                *mainInputOutput.getWritePointer (i, j) = sampleCountDown > 0 ? *mainInputOutput.getReadPointer (i, j) : 0.0f;

            if (sampleCountDown > 0)
                --sampleCountDown;
        }
    }

    using AudioProcessor::processBlock;

    //==============================================================================
    AudioProcessorEditor* createEditor() override            { return new GenericAudioProcessorEditor (*this); }
    bool hasEditor() const override                          { return true; }
    const String getName() const override                    { return "NoiseGate"; }
    bool acceptsMidi() const override                        { return false; }
    bool producesMidi() const override                       { return false; }
    double getTailLengthSeconds() const override             { return 0.0; }
    int getNumPrograms() override                            { return 1; }
    int getCurrentProgram() override                         { return 0; }
    void setCurrentProgram (int) override                    {}
    const String getProgramName (int) override               { return "None"; }
    void changeProgramName (int, const String&) override     {}
    bool isVST2() const noexcept                             { return (wrapperType == wrapperType_VST); }

    //==============================================================================
    void getStateInformation (MemoryBlock& destData) override
    {
        MemoryOutputStream stream (destData, true);

        stream.writeFloat (*threshold);
        stream.writeFloat (*alpha);
    }

    void setStateInformation (const void* data, int sizeInBytes) override
    {
        MemoryInputStream stream (data, static_cast<size_t> (sizeInBytes), false);

        threshold->setValueNotifyingHost (stream.readFloat());
        alpha->setValueNotifyingHost     (stream.readFloat());
    }

private:
    //==============================================================================
    AudioParameterFloat* threshold;
    AudioParameterFloat* alpha;
    int sampleCountDown;

    float lowPassCoeff;

    //==============================================================================
    HIGGS_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (NoiseGate)
};
