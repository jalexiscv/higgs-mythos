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

#ifdef HIGGS_AUDIO_UTILS_H_INCLUDED
 /* When you add this cpp file to your project, you mustn't include it in a file where you've
    already included any other headers - just put it inside a file on its own, possibly with your config
    flags preceding it, but don't include anything else. That also includes avoiding any automatic prefix
    header files that the compiler may be using.
 */
 #error "Incorrect use of HIGGS cpp file"
#endif

#define HIGGS_CORE_INCLUDE_NATIVE_HEADERS 1
#define HIGGS_CORE_INCLUDE_JNI_HELPERS 1
#define HIGGS_CORE_INCLUDE_OBJC_HELPERS 1
#define HIGGS_CORE_INCLUDE_COM_SMART_PTR 1

#include "higgs_audio_utils.h"
#include <higgs_gui_extra/higgs_gui_extra.h>

#if HIGGS_MAC
 #import <DiscRecording/DiscRecording.h>
 #import <CoreAudioKit/CABTLEMIDIWindowController.h>
#elif HIGGS_IOS
 #import <CoreAudioKit/CoreAudioKit.h>
#elif HIGGS_WINDOWS
 #if HIGGS_USE_CDBURNER
  /* You'll need the Platform SDK for these headers - if you don't have it and don't
     need to use CD-burning, then you might just want to set the HIGGS_USE_CDBURNER flag
     to 0, to avoid these includes.
  */
  #include <imapi.h>
  #include <imapierror.h>
 #endif
#endif

#include "gui/higgs_AudioDeviceSelectorComponent.cpp"
#include "gui/higgs_AudioThumbnail.cpp"
#include "gui/higgs_AudioThumbnailCache.cpp"
#include "gui/higgs_AudioVisualiserComponent.cpp"
#include "gui/higgs_KeyboardComponentBase.cpp"
#include "gui/higgs_MidiKeyboardComponent.cpp"
#include "gui/higgs_MPEKeyboardComponent.cpp"
#include "gui/higgs_AudioAppComponent.cpp"
#include "players/higgs_SoundPlayer.cpp"
#include "players/higgs_AudioProcessorPlayer.cpp"
#include "audio_cd/higgs_AudioCDReader.cpp"

#if HIGGS_MAC
 #include "native/higgs_BluetoothMidiDevicePairingDialogue_mac.mm"

 #if HIGGS_USE_CDREADER
  #include "native/higgs_AudioCDReader_mac.mm"
 #endif

 #if HIGGS_USE_CDBURNER
  #include "native/higgs_AudioCDBurner_mac.mm"
 #endif

#elif HIGGS_IOS
 #include "native/higgs_BluetoothMidiDevicePairingDialogue_ios.mm"

#elif HIGGS_ANDROID
 #include "native/higgs_BluetoothMidiDevicePairingDialogue_android.cpp"

#elif HIGGS_LINUX || HIGGS_BSD
 #if HIGGS_USE_CDREADER
  #include "native/higgs_AudioCDReader_linux.cpp"
 #endif

 #include "native/higgs_BluetoothMidiDevicePairingDialogue_linux.cpp"

#elif HIGGS_WINDOWS
 #include "native/higgs_BluetoothMidiDevicePairingDialogue_windows.cpp"

 #if HIGGS_USE_CDREADER
  #include "native/higgs_AudioCDReader_windows.cpp"
 #endif

 #if HIGGS_USE_CDBURNER
  #include "native/higgs_AudioCDBurner_windows.cpp"
 #endif

#endif
