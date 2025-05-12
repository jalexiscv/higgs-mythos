#pragma once

%%include_higgs%%

//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class %%content_component_class%%  : public higgs::Component
{
public:
    //==============================================================================
    %%content_component_class%%()
    {
        setSize (600, 400);
    }

    ~%%content_component_class%%() override
    {
    }

    //==============================================================================
    void paint (higgs::Graphics& g) override
    {
        // (Our component is opaque, so we must completely fill the background with a solid colour)
        g.fillAll (getLookAndFeel().findColour (higgs::ResizableWindow::backgroundColourId));

        g.setFont (higgs::FontOptions (16.0f));
        g.setColour (higgs::Colours::white);
        g.drawText ("Hello World!", getLocalBounds(), higgs::Justification::centred, true);
    }

    void resized() override
    {
        // This is called when the %%content_component_class%% is resized.
        // If you add any child components, this is where you should
        // update their positions.
    }


private:
    //==============================================================================
    // Your private member variables go here...


    HIGGS_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (%%content_component_class%%)
};
