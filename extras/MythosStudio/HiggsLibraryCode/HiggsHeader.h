/*

    IMPORTANT! This file is auto-generated each time you save your
    project - if you alter its contents, your changes may be overwritten!

    This is the header file that your files should include in order to get all the
    HIGGS library headers. You should avoid including the HIGGS headers directly in
    your own source files, because that wouldn't pick up the correct configuration
    options for your app.

*/

#pragma once


#include <higgs_build_tools/higgs_build_tools.h>
#include <higgs_core/higgs_core.h>
#include <higgs_cryptography/higgs_cryptography.h>
#include <higgs_data_structures/higgs_data_structures.h>
#include <higgs_events/higgs_events.h>
#include <higgs_graphics/higgs_graphics.h>
#include <higgs_gui_basics/higgs_gui_basics.h>
#include <higgs_gui_extra/higgs_gui_extra.h>

#include "BinaryData.h"

#if defined (HIGGS_MYTHOSSTUDIO_VERSION) && HIGGS_MYTHOSSTUDIO_VERSION < HIGGS_VERSION
 /** If you've hit this error then the version of the MythosStudio that was used to generate this project is
     older than the version of the HIGGS modules being included. To fix this error, re-save your project
     using the latest version of the MythosStudio or, if you aren't using the MythosStudio to manage your project,
     remove the HIGGS_MYTHOSSTUDIO_VERSION define.
 */
 #error "This project was last saved using an outdated version of the MythosStudio! Re-save this project with the latest version to fix this error."
#endif

#if ! DONT_SET_USING_HIGGS_NAMESPACE
 // If your code uses a lot of HIGGS classes, then this will obviously save you
 // a lot of typing, but can be disabled by setting DONT_SET_USING_HIGGS_NAMESPACE.
 using namespace higgs;
#endif

#if ! HIGGS_DONT_DECLARE_PROJECTINFO
namespace ProjectInfo
{
    const char* const  projectName    = "MythosStudio";
    const char* const  companyName    = "Raw Material Software Limited";
    const char* const  versionString  = "9.0.5";
    const int          versionNumber  = 0x90005;
}
#endif
