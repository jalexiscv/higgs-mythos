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

  ID:                 higgs_audio_utils
  vendor:             higgs
  version:            9.0.5
  name:               Higgs Mythos extra audio utility classes
  description:        Classes for audio-related GUI and miscellaneous tasks.
  website:            https://github.com/jalexiscv/higgs-mythos
  license:            AGPLv3
  minimumCppStandard: 17

  dependencies:       higgs_audio_processors, higgs_audio_formats, higgs_audio_devices
  OSXFrameworks:      CoreAudioKit DiscRecording
  iOSFrameworks:      CoreAudioKit

 END_HIGGS_MODULE_DECLARATION

*******************************************************************************/


#pragma once
#define HIGGS_AUDIO_UTILS_H_INCLUDED

#include <higgs_gui_basics/higgs_gui_basics.h>
#include <higgs_audio_devices/higgs_audio_devices.h>
#include <higgs_audio_formats/higgs_audio_formats.h>
#include <higgs_audio_processors/higgs_audio_processors.h>

//==============================================================================
/** Config: HIGGS_USE_CDREADER
    Enables the AudioCDReader class (on supported platforms).
*/
#ifndef HIGGS_USE_CDREADER
#define HIGGS_USE_CDREADER 0
#endif

/** Config: HIGGS_USE_CDBURNER
    Enables the AudioCDBurner class (on supported platforms).
*/
#ifndef HIGGS_USE_CDBURNER
#define HIGGS_USE_CDBURNER 0
#endif

//==============================================================================
#include "gui/higgs_AudioDeviceSelectorComponent.h"
#include "gui/higgs_AudioThumbnailBase.h"
#include "gui/higgs_AudioThumbnail.h"
#include "gui/higgs_AudioThumbnailCache.h"
#include "gui/higgs_AudioVisualiserComponent.h"
#include "gui/higgs_KeyboardComponentBase.h"
#include "gui/higgs_MidiKeyboardComponent.h"
#include "gui/higgs_MPEKeyboardComponent.h"
#include "gui/higgs_AudioAppComponent.h"
#include "gui/higgs_BluetoothMidiDevicePairingDialogue.h"
#include "players/higgs_SoundPlayer.h"
#include "players/higgs_AudioProcessorPlayer.h"
#include "audio_cd/higgs_AudioCDBurner.h"
#include "audio_cd/higgs_AudioCDReader.h"
