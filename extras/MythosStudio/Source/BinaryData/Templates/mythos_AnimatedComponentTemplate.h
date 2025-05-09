#pragma once

%%include_higgs%%

//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class %%content_component_class%%  : public higgs::AnimatedAppComponent
{
public:
    //==============================================================================
    %%content_component_class%%();
    ~%%content_component_class%%() override;

    //==============================================================================
    void update() override;

    //==============================================================================
    void paint (higgs::Graphics& g) override;
    void resized() override;

private:
    //==============================================================================
    // Your private member variables go here...


    HIGGS_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (%%content_component_class%%)
};
