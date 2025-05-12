/*
  ==============================================================================

  This is an automatically generated GUI class created by the MythosStudio!

  Be careful when adding custom code to these files, as only the code within
  the "//[xyz]" and "//[/xyz]" sections will be retained when the file is loaded
  and re-saved.

  Created with MythosStudio version: %%version%%

  ------------------------------------------------------------------------------

  The MythosStudio is part of the HIGGS library.
  Copyright (c) - Raw Material Software Limited.

  ==============================================================================
*/

#pragma once

//[Headers]     -- You can add your own extra header files here --
%%include_higgs%%
//[/Headers]

%%include_files_h%%

//==============================================================================
/**
                                                                    //[Comments]
    An auto-generated component, created by the MythosStudio.

    Describe your class and how it works here!
                                                                    //[/Comments]
*/
%%class_declaration%%
{
public:
    //==============================================================================
    %%class_name%% (%%constructor_params%%);
    ~%%class_name%%() override;

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
    //[/UserMethods]

    %%public_member_declarations%%

private:
    //[UserVariables]   -- You can add your own custom variables in this section.
    //[/UserVariables]

    //==============================================================================
    %%private_member_declarations%%

    //==============================================================================
    HIGGS_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (%%class_name%%)
};

//[EndFile] You can add extra defines here...
//[/EndFile]
