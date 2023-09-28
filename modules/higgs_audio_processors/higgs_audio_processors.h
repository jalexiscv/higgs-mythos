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

  ID:                 higgs_audio_processors
  vendor:             higgs
  version:            9.0.5
  name:               Higgs Mythos audio processor classes
  description:        Classes for loading and playing VST, AU, LADSPA, or internally-generated audio processors.
  website:            https://github.com/jalexiscv/higgs-mythos
  license:            AGPLv3
  minimumCppStandard: 17

  dependencies:       higgs_gui_extra, higgs_audio_processors_headless

 END_HIGGS_MODULE_DECLARATION

*******************************************************************************/


#pragma once
#define HIGGS_AUDIO_PROCESSORS_H_INCLUDED

#include <higgs_audio_processors_headless/higgs_audio_processors_headless.h>
#include <higgs_gui_extra/higgs_gui_extra.h>

//==============================================================================
#include "processors/higgs_AudioProcessorEditorHostContext.h"
#include "processors/higgs_AudioProcessorEditor.h"
#include "processors/higgs_GenericAudioProcessorEditor.h"
#include "format/higgs_AudioPluginFormatManagerHelpers.h"
#include "scanning/higgs_KnownPluginList.h"
#include "format_types/higgs_AudioUnitPluginFormat.h"
#include "format_types/higgs_LADSPAPluginFormat.h"
#include "format_types/higgs_LV2PluginFormat.h"
#include "format_types/higgs_VST3PluginFormat.h"
#include "format_types/higgs_VSTPluginFormat.h"
#include "scanning/higgs_PluginDirectoryScanner.h"
#include "scanning/higgs_PluginListComponent.h"
#include "utilities/higgs_ParameterAttachments.h"
#include "utilities/higgs_AudioProcessorValueTreeState.h"
#include "utilities/higgs_PluginHostType.h"

//==============================================================================
// These declarations are here to avoid missing-prototype warnings in user code.

// If you're implementing a plugin, you should supply a body for
// this function in your own code.
higgs::AudioProcessor* HIGGS_CALLTYPE createPluginFilter();

// If you are implementing an ARA enabled plugin, you need to
// implement this function somewhere in the codebase by returning
// SubclassOfARADocumentControllerSpecialisation::createARAFactory<SubclassOfARADocumentControllerSpecialisation>();
#if HiggsPlugin_Enable_ARA
 const ARA::ARAFactory* HIGGS_CALLTYPE createARAFactory();
#endif
