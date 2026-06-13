#pragma once

// CMake builds don't use an AppConfig.h, so it's safe to include higgs module headers
// directly. If you need to remain compatible with MythosStudio-generated builds, and
// have called `higgs_generate_higgs_header(<thisTarget>)` in your CMakeLists.txt,
// you could `#include <HiggsHeader.h>` here instead, to make all your module headers visible.
#include <higgs_gui_extra/higgs_gui_extra.h>

//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class MainComponent final : public higgs::Component
{
public:
    //==============================================================================
    MainComponent();

    //==============================================================================
    void paint (higgs::Graphics&) override;
    void resized() override;

private:
    //==============================================================================
    // Your private member variables go here...

    HIGGS_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
