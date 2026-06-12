#pragma once

#include <higgs_audio_processors/higgs_audio_processors.h>

//==============================================================================
class AudioPluginAudioProcessor final : public higgs::AudioProcessor
{
public:
    //==============================================================================
    AudioPluginAudioProcessor();
    ~AudioPluginAudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;

    void processBlock (higgs::AudioBuffer<float>&, higgs::MidiBuffer&) override;
    using AudioProcessor::processBlock;

    //==============================================================================
    higgs::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const higgs::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const higgs::String getProgramName (int index) override;
    void changeProgramName (int index, const higgs::String& newName) override;

    //==============================================================================
    void getStateInformation (higgs::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

private:
    //==============================================================================
    HIGGS_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AudioPluginAudioProcessor)
};
