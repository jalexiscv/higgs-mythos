/*
  ==============================================================================

   This file is part of the Higgs Mythos framework.
   Copyright (c) Raw Material Software Limited

   Higgs Mythos is free software: you can redistribute it and/or modify
   it under the terms of the GNU Affero General Public License as published
   by the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   Higgs Mythos is distributed in the hope that it will be useful, but
   WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
   GNU Affero General Public License for more details.

   You should have received a copy of the GNU Affero General Public License
   along with Higgs Mythos. If not, see <https://www.gnu.org/licenses/>.

  ==============================================================================
*/


/*******************************************************************************
 The block below describes the properties of this module, and is read by
 the MythosStudio to automatically generate project code that uses it.
 For details about the syntax and how to create or use a module, see the
 Higgs Mythos Module Format.md file.


 BEGIN_HIGGS_MODULE_DECLARATION

  ID:                 higgs_audio_processors_headless
  vendor:             higgs
  version:            9.0.5
  name:               Higgs Mythos audio processor classes without UI
  description:        Classes for loading and playing VST, AU, LADSPA, or internally-generated audio processors without UI.
  website:            https://github.com/jalexiscv/higgs-mythos
  license:            AGPLv3
  minimumCppStandard: 17

  dependencies:       higgs_audio_basics higgs_events
  OSXFrameworks:      CoreAudio CoreMIDI AudioToolbox
  iOSFrameworks:      AudioToolbox

 END_HIGGS_MODULE_DECLARATION

*******************************************************************************/


#pragma once
#define HIGGS_AUDIO_PROCESSORS_HEADLESS_H_INCLUDED

#include <higgs_audio_basics/higgs_audio_basics.h>
#include <higgs_events/higgs_events.h>

//==============================================================================
/** Config: HIGGS_PLUGINHOST_VST
    Enables the VST audio plugin hosting classes. You will need to have the VST2 SDK files in your header search paths. You can obtain the VST2 SDK files from on older version of the VST3 SDK.

    @see VSTPluginFormat, VST3PluginFormat, AudioPluginFormat, AudioPluginFormatManager, HIGGS_PLUGINHOST_AU, HIGGS_PLUGINHOST_VST3, HIGGS_PLUGINHOST_LADSPA
*/
#ifndef HIGGS_PLUGINHOST_VST
 #define HIGGS_PLUGINHOST_VST 0
#endif

/** Config: HIGGS_PLUGINHOST_VST3
    Enables the VST3 audio plugin hosting classes.

    @see VSTPluginFormat, VST3PluginFormat, AudioPluginFormat, AudioPluginFormatManager, HIGGS_PLUGINHOST_VST, HIGGS_PLUGINHOST_AU, HIGGS_PLUGINHOST_LADSPA
*/
#ifndef HIGGS_PLUGINHOST_VST3
 #define HIGGS_PLUGINHOST_VST3 0
#endif

/** Config: HIGGS_PLUGINHOST_AU
    Enables the AudioUnit plugin hosting classes. This is Mac-only, of course.

    @see AudioUnitPluginFormat, AudioPluginFormat, AudioPluginFormatManager, HIGGS_PLUGINHOST_VST, HIGGS_PLUGINHOST_VST3, HIGGS_PLUGINHOST_LADSPA
*/
#ifndef HIGGS_PLUGINHOST_AU
 #define HIGGS_PLUGINHOST_AU 0
#endif

/** Config: HIGGS_PLUGINHOST_LADSPA
    Enables the LADSPA plugin hosting classes. This is Linux-only, of course.

    @see LADSPAPluginFormat, AudioPluginFormat, AudioPluginFormatManager, HIGGS_PLUGINHOST_VST, HIGGS_PLUGINHOST_VST3, HIGGS_PLUGINHOST_AU
 */
#ifndef HIGGS_PLUGINHOST_LADSPA
 #define HIGGS_PLUGINHOST_LADSPA 0
#endif

/** Config: HIGGS_PLUGINHOST_LV2
    Enables the LV2 plugin hosting classes.
 */
#ifndef HIGGS_PLUGINHOST_LV2
 #define HIGGS_PLUGINHOST_LV2 0
#endif

/** Config: HIGGS_PLUGINHOST_ARA
    Enables the ARA plugin extension hosting classes. You will need to download the ARA SDK and specify the
    path to it either in the MythosStudio, using higgs_set_ara_sdk_path() in your CMake project file.

    The directory can be obtained by recursively cloning https://github.com/Celemony/ARA_SDK and checking out
    the tag releases/2.1.0.
*/
#ifndef HIGGS_PLUGINHOST_ARA
 #define HIGGS_PLUGINHOST_ARA 0
#endif

/** Config: HIGGS_CUSTOM_VST3_SDK
    If enabled, the embedded VST3 SDK in HIGGS will not be added to the project and instead you should
    add the path to your custom VST3 SDK to the project's header search paths. Most users shouldn't
    need to enable this and should just use the version of the SDK included with HIGGS.
*/
#ifndef HIGGS_CUSTOM_VST3_SDK
 #define HIGGS_CUSTOM_VST3_SDK 0
#endif

//==============================================================================
#include <higgs_audio_processors_headless/format/higgs_PluginFormatDefs.h>
#include <higgs_audio_processors_headless/processors/higgs_AudioProcessorListener.h>
#include <higgs_audio_processors_headless/utilities/higgs_AAXClientExtensions.h>
#include <higgs_audio_processors_headless/utilities/higgs_VST2ClientExtensions.h>
#include <higgs_audio_processors_headless/utilities/higgs_VST3Interface.h>
#include <higgs_audio_processors_headless/utilities/higgs_VST3ClientExtensions.h>
#include <higgs_audio_processors_headless/format_types/higgs_ARACommon.h>
#include <higgs_audio_processors_headless/utilities/higgs_AudioPluginExtensions.h>
#include <higgs_audio_processors_headless/processors/higgs_AudioProcessorParameter.h>
#include <higgs_audio_processors_headless/processors/higgs_HostedAudioProcessorParameter.h>
#include <higgs_audio_processors_headless/processors/higgs_AudioProcessorParameterGroup.h>
#include <higgs_audio_processors_headless/processors/higgs_AudioProcessor.h>
#include <higgs_audio_processors_headless/processors/higgs_PluginDescription.h>
#include <higgs_audio_processors_headless/processors/higgs_AudioPluginInstance.h>
#include <higgs_audio_processors_headless/processors/higgs_AudioProcessorGraph.h>
#include <higgs_audio_processors_headless/format/higgs_AudioPluginFormat.h>
#include <higgs_audio_processors_headless/utilities/higgs_AudioProcessorParameterWithID.h>
#include <higgs_audio_processors_headless/utilities/higgs_RangedAudioParameter.h>
#include <higgs_audio_processors_headless/utilities/higgs_AudioParameterFloat.h>
#include <higgs_audio_processors_headless/utilities/higgs_AudioParameterInt.h>
#include <higgs_audio_processors_headless/utilities/higgs_AudioParameterBool.h>
#include <higgs_audio_processors_headless/utilities/higgs_AudioParameterChoice.h>
#include <higgs_audio_processors_headless/utilities/ARA/higgs_ARADebug.h>
#include <higgs_audio_processors_headless/utilities/ARA/higgs_ARA_utils.h>
#include <higgs_audio_processors_headless/format_types/higgs_AudioUnitPluginFormatHeadless.h>
#include <higgs_audio_processors_headless/format_types/higgs_LADSPAPluginFormatHeadless.h>
#include <higgs_audio_processors_headless/format_types/higgs_LV2PluginFormatHeadless.h>
#include <higgs_audio_processors_headless/format_types/higgs_VST3PluginFormatHeadless.h>
#include <higgs_audio_processors_headless/format_types/higgs_VSTPluginFormatHeadless.h>
#include <higgs_audio_processors_headless/format_types/higgs_ARAHosting.h>
#include <higgs_audio_processors_headless/format/higgs_AudioPluginFormatManager.h>
