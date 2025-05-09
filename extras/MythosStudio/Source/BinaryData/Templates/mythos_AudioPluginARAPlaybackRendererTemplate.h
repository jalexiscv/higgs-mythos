/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for an ARA playback renderer implementation.

  ==============================================================================
*/

#pragma once

#include <higgs_audio_processors/higgs_audio_processors.h>

//==============================================================================
/**
*/
class %%araplaybackrenderer_class_name%%  : public higgs::ARAPlaybackRenderer
{
public:
    //==============================================================================
    using higgs::ARAPlaybackRenderer::ARAPlaybackRenderer;

    //==============================================================================
    void prepareToPlay (double sampleRate,
                        int maximumSamplesPerBlock,
                        int numChannels,
                        higgs::AudioProcessor::ProcessingPrecision,
                        AlwaysNonRealtime alwaysNonRealtime) override;
    void releaseResources() override;

    //==============================================================================
    bool processBlock (higgs::AudioBuffer<float>& buffer,
                       higgs::AudioProcessor::Realtime realtime,
                       const higgs::AudioPlayHead::PositionInfo& positionInfo) noexcept override;

private:
    //==============================================================================
    double sampleRate = 44100.0;
    int maximumSamplesPerBlock = 4096;
    int numChannels = 1;
    bool useBufferedAudioSourceReader = true;

    HIGGS_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (%%araplaybackrenderer_class_name%%)
};
