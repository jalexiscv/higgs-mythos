/*
  ==============================================================================

    This file contains the basic framework code for a HIGGS plugin processor.

  ==============================================================================
*/

#pragma once

%%app_headers%%

//==============================================================================
/**
*/
class %%filter_class_name%%  : public higgs::AudioProcessor
                            #if HiggsPlugin_Enable_ARA
                             , public higgs::AudioProcessorARAExtension
                            #endif
{
public:
    //==============================================================================
    %%filter_class_name%%();
    ~%%filter_class_name%%() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef HiggsPlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (higgs::AudioBuffer<float>&, higgs::MidiBuffer&) override;

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
    HIGGS_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (%%filter_class_name%%)
};
