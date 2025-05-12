/*
  ==============================================================================

    %%filename%%
    Created: %%date%%
    Author:  %%author%%

  ==============================================================================
*/

#pragma once

%%include_higgs%%

//==============================================================================
/*
*/
class %%component_class%%  : public higgs::Component
{
public:
    %%component_class%%()
    {
        // In your constructor, you should add any child components, and
        // initialise any special settings that your component needs.

    }

    ~%%component_class%%() override
    {
    }

    void paint (higgs::Graphics& g) override
    {
        /* This demo code just fills the component's background and
           draws some placeholder text to get you started.

           You should replace everything in this method with your own
           drawing code..
        */

        g.fillAll (getLookAndFeel().findColour (higgs::ResizableWindow::backgroundColourId));   // clear the background

        g.setColour (higgs::Colours::grey);
        g.drawRect (getLocalBounds(), 1);   // draw an outline around the component

        g.setColour (higgs::Colours::white);
        g.setFont (higgs::FontOptions (14.0f));
        g.drawText ("%%component_class%%", getLocalBounds(),
                    higgs::Justification::centred, true);   // draw some placeholder text
    }

    void resized() override
    {
        // This method is where you should set the bounds of any child
        // components that your component contains..

    }

private:
    HIGGS_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (%%component_class%%)
};
