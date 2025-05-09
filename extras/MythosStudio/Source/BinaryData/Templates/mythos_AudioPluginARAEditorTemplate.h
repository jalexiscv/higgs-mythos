/*
  ==============================================================================

    This file contains the basic framework code for a HIGGS plugin editor.

  ==============================================================================
*/

#pragma once

%%editor_headers%%

//==============================================================================
/**
*/
class %%editor_class_name%%  : public higgs::AudioProcessorEditor
                            #if HiggsPlugin_Enable_ARA
                             , public higgs::AudioProcessorEditorARAExtension
                            #endif
{
public:
    %%editor_class_name%% (%%filter_class_name%%&);
    ~%%editor_class_name%%() override;

    //==============================================================================
    void paint (higgs::Graphics&) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    %%filter_class_name%%& audioProcessor;

    HIGGS_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (%%editor_class_name%%)
};
