/*

    IMPORTANT! This file is auto-generated each time you save your
    project - if you alter its contents, your changes may be overwritten!

    This is the header file that your files should include in order to get all the
    HIGGS library headers. You should avoid including the HIGGS headers directly in
    your own source files, because that wouldn't pick up the correct configuration
    options for your app.

*/

#pragma once


#include <higgs_analytics/higgs_analytics.h>
#include <higgs_audio_basics/higgs_audio_basics.h>
#include <higgs_audio_devices/higgs_audio_devices.h>
#include <higgs_audio_formats/higgs_audio_formats.h>
#include <higgs_audio_processors/higgs_audio_processors.h>
#include <higgs_audio_processors_headless/higgs_audio_processors_headless.h>
#include <higgs_audio_utils/higgs_audio_utils.h>
#include <higgs_core/higgs_core.h>
#include <higgs_cryptography/higgs_cryptography.h>
#include <higgs_data_structures/higgs_data_structures.h>
#include <higgs_dsp/higgs_dsp.h>
#include <higgs_events/higgs_events.h>
#include <higgs_graphics/higgs_graphics.h>
#include <higgs_gui_basics/higgs_gui_basics.h>
#include <higgs_gui_extra/higgs_gui_extra.h>
#include <higgs_javascript/higgs_javascript.h>
#include <higgs_midi_ci/higgs_midi_ci.h>
#include <higgs_opengl/higgs_opengl.h>
#include <higgs_osc/higgs_osc.h>
#include <higgs_product_unlocking/higgs_product_unlocking.h>


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
    const char* const  projectName    = "UnitTestRunner";
    const char* const  companyName    = "Raw Material Software Limited";
    const char* const  versionString  = "1.0.0";
    const int          versionNumber  = 0x10000;
}
#endif
