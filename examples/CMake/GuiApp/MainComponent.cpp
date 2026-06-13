#include "MainComponent.h"

//==============================================================================
MainComponent::MainComponent()
{
    setSize (600, 400);
}

//==============================================================================
void MainComponent::paint (higgs::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (higgs::ResizableWindow::backgroundColourId));

    g.setFont (higgs::FontOptions (16.0f));
    g.setColour (higgs::Colours::white);
    g.drawText ("Hello World!", getLocalBounds(), higgs::Justification::centred, true);
}

void MainComponent::resized()
{
    // This is called when the MainComponent is resized.
    // If you add any child components, this is where you should
    // update their positions.
}
