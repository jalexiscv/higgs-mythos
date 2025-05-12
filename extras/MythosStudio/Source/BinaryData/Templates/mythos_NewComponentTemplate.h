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
    %%component_class%%();
    ~%%component_class%%() override;

    void paint (higgs::Graphics&) override;
    void resized() override;

private:
    HIGGS_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (%%component_class%%)
};
