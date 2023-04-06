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

#ifdef HIGGS_AUDIO_DEVICES_H_INCLUDED
 /* When you add this cpp file to your project, you mustn't include it in a file where you've
    already included any other headers - just put it inside a file on its own, possibly with your config
    flags preceding it, but don't include anything else. That also includes avoiding any automatic prefix
    header files that the compiler may be using.
 */
 #error "Incorrect use of HIGGS cpp file"
#endif

#define HIGGS_CORE_INCLUDE_OBJC_HELPERS 1
#define HIGGS_CORE_INCLUDE_COM_SMART_PTR 1
#define HIGGS_CORE_INCLUDE_JNI_HELPERS 1
#define HIGGS_CORE_INCLUDE_NATIVE_HEADERS 1
#define HIGGS_EVENTS_INCLUDE_WIN32_MESSAGE_WINDOW 1

#ifndef HIGGS_USE_WINRT_MIDI
 #define HIGGS_USE_WINRT_MIDI 0
#endif

#if HIGGS_USE_WINRT_MIDI
 #define HIGGS_EVENTS_INCLUDE_WINRT_WRAPPER 1
#endif

#include "higgs_audio_devices.h"

#include "midi_io/higgs_WaitFreeListeners.h"
#include "midi_io/higgs_WaitFreeListeners.cpp"
#include "midi_io/higgs_MidiDeviceListConnectionBroadcaster.cpp"

#include "midi_io/ump/higgs_UMPBlock.cpp"
#include "midi_io/ump/higgs_UMPIOHelpers.cpp"
#include "midi_io/ump/higgs_UMPInput.cpp"
#include "midi_io/ump/higgs_UMPOutput.cpp"
#include "midi_io/ump/higgs_UMPLegacyVirtualInput.cpp"
#include "midi_io/ump/higgs_UMPLegacyVirtualOutput.cpp"
#include "midi_io/ump/higgs_UMPVirtualEndpoint.cpp"
#include "midi_io/ump/higgs_UMPSession.cpp"
#include "midi_io/ump/higgs_UMPEndpoint.cpp"
#include "midi_io/ump/higgs_UMPStaticDeviceInfo.cpp"
#include "midi_io/ump/higgs_UMPEndpoints.cpp"
#include "midi_io/ump/higgs_UMPEndpointId.cpp"

#include "audio_io/higgs_SampleRateHelpers.cpp"
#include "midi_io/higgs_MidiDevices.cpp"

//==============================================================================
#if HIGGS_MAC || HIGGS_IOS
 #include <higgs_audio_basics/native/higgs_CoreAudioTimeConversions_mac.h>
 #include <higgs_audio_basics/native/higgs_AudioWorkgroup_mac.h>
#endif

#if HIGGS_MAC
 #define Point CarbonDummyPointName
 #define Component CarbonDummyCompName
 #import <CoreAudio/AudioHardware.h>
 #import <CoreMIDI/CoreMIDI.h>
 #import <AudioToolbox/AudioServices.h>
 #undef Point
 #undef Component

 #include "native/higgs_CoreAudio_mac.cpp"

#elif HIGGS_IOS
 #import <AudioToolbox/AudioToolbox.h>
 #import <AVFoundation/AVFoundation.h>
 #import <CoreMIDI/CoreMIDI.h>

 #if HIGGS_MODULE_AVAILABLE_higgs_graphics
  #include <higgs_graphics/native/higgs_CoreGraphicsHelpers_mac.h>
 #endif

 #include "native/higgs_Audio_ios.cpp"

//==============================================================================
#elif HIGGS_WINDOWS
 #include <cfgmgr32.h>

 #if ! HIGGS_DONT_AUTOLINK_TO_WIN32_LIBRARIES
  #pragma comment (lib, "cfgmgr32.lib")
 #endif

 #if HIGGS_WASAPI
  #include <mmreg.h>
  #include "native/higgs_WASAPI_windows.cpp"
 #endif

 #if HIGGS_DIRECTSOUND
  #include "native/higgs_DirectSound_windows.cpp"
 #endif

 #if HIGGS_USE_WINRT_MIDI && (HIGGS_MSVC || HIGGS_CLANG)
  /* If you cannot find any of the header files below then you are probably
     attempting to use the Windows 10 Bluetooth Low Energy API. For this to work you
     need to install version 10.0.14393.0 of the Windows Standalone SDK and you may
     need to add the path to the WinRT headers to your build system. This path should
     have the form "C:\Program Files (x86)\Windows Kits\10\Include\10.0.14393.0\winrt".

     Also please note that Microsoft's Bluetooth MIDI stack has multiple issues, so
     this API is EXPERIMENTAL - use at your own risk!
  */
  #include <windows.devices.h>
  #include <windows.devices.midi.h>
  #include <windows.devices.enumeration.h>

  HIGGS_BEGIN_IGNORE_WARNINGS_MSVC (4265)
  #include <wrl/event.h>
  HIGGS_END_IGNORE_WARNINGS_MSVC

  HIGGS_BEGIN_IGNORE_WARNINGS_MSVC (4467)
  #include <robuffer.h>
  HIGGS_END_IGNORE_WARNINGS_MSVC
 #endif

 #if HIGGS_ASIO
  #if HIGGS_ASIO_USE_EXTERNAL_SDK
   #include <iasiodrv.h>
  #else
   #include <higgs_audio_devices/native/asio/iasiodrv.h>
  #endif
  #include "native/higgs_ASIO_windows.cpp"
 #endif

//==============================================================================
#elif HIGGS_LINUX || HIGGS_BSD
 #if HIGGS_ALSA
  /* Got an include error here? If so, you've either not got ALSA installed, or you've
     not got your paths set up correctly to find its header files.

     The package you need to install to get ASLA support is "libasound2-dev".

     If you don't have the ALSA library and don't want to build HIGGS with audio support,
     just set the HIGGS_ALSA flag to 0.
  */
  HIGGS_BEGIN_IGNORE_WARNINGS_GCC_LIKE ("-Wzero-length-array")
  #include <alsa/asoundlib.h>
  HIGGS_END_IGNORE_WARNINGS_GCC_LIKE
  #include "native/higgs_ALSA_weak_linux.h"
  #include "native/higgs_ALSA_linux.cpp"
 #endif
 #undef SIZEOF

//==============================================================================
#elif HIGGS_ANDROID

 // Currently we're just using this for enum values
 #include <amidi/AMidi.h>

namespace higgs
{
    using RealtimeThreadFactory = pthread_t (*) (void* (*) (void*), void*);
    RealtimeThreadFactory getAndroidRealtimeThreadFactory();
} // namespace higgs

 #include "native/higgs_Audio_android.cpp"

 #if HIGGS_USE_ANDROID_OPENSLES || HIGGS_USE_ANDROID_OBOE
  #include "native/higgs_HighPerformanceAudioHelpers_android.h"

  #if HIGGS_USE_ANDROID_OPENSLES
   #include <SLES/OpenSLES.h>
   #include <SLES/OpenSLES_Android.h>
   #include <SLES/OpenSLES_AndroidConfiguration.h>
   #include "native/higgs_OpenSL_android.cpp"
  #endif

  #if HIGGS_USE_ANDROID_OBOE
   #if HIGGS_USE_ANDROID_OPENSLES
    #error "Oboe cannot be enabled at the same time as openSL! Please disable HIGGS_USE_ANDROID_OPENSLES"
   #endif

   HIGGS_BEGIN_IGNORE_WARNINGS_GCC_LIKE ("-Wunused-parameter",
                                        "-Wzero-as-null-pointer-constant",
                                        "-Winconsistent-missing-destructor-override",
                                        "-Wshadow-field-in-constructor",
                                        "-Wshadow-field",
                                        "-Wsign-conversion",
                                        "-Wswitch-enum",
                                        "-Wshorten-64-to-32")
   #include <oboe/Oboe.h>
   HIGGS_END_IGNORE_WARNINGS_GCC_LIKE

   #include "native/higgs_Oboe_android.cpp"
  #endif
 #else
// No audio library, so no way to create realtime threads.
  namespace higgs
  {
      RealtimeThreadFactory getAndroidRealtimeThreadFactory() { return nullptr; }
  }
 #endif

#endif

#if (HIGGS_LINUX || HIGGS_BSD || HIGGS_MAC || HIGGS_WINDOWS) && HIGGS_JACK
 /* Got an include error here? If so, you've either not got jack-audio-connection-kit
    installed, or you've not got your paths set up correctly to find its header files.

    Linux: The package you need to install to get JACK support is libjack-dev.

    macOS: The package you need to install to get JACK support is jack, which you can
    install using Homebrew.

    Windows: The package you need to install to get JACK support is available from the
    JACK Audio website. Download and run the installer for Windows.

    If you don't have the jack-audio-connection-kit library and don't want to build
    HIGGS with low latency audio support, just set the HIGGS_JACK flag to 0.
 */
 #include <jack/jack.h>
 #include "native/higgs_JackAudio.cpp"
#endif

#if ! HIGGS_SYSTEMAUDIOVOL_IMPLEMENTED
namespace higgs
{
    // None of these methods are available. (On Windows you might need to enable WASAPI for this)
    float HIGGS_CALLTYPE SystemAudioVolume::getGain()         { jassertfalse; return 0.0f; }
    bool  HIGGS_CALLTYPE SystemAudioVolume::setGain (float)   { jassertfalse; return false; }
    bool  HIGGS_CALLTYPE SystemAudioVolume::isMuted()         { jassertfalse; return false; }
    bool  HIGGS_CALLTYPE SystemAudioVolume::setMuted (bool)   { jassertfalse; return false; }
}
#endif

#include "audio_io/higgs_AudioDeviceManager.cpp"
#include "audio_io/higgs_AudioIODevice.cpp"
#include "audio_io/higgs_AudioIODeviceType.cpp"
#include "midi_io/higgs_MidiMessageCollector.cpp"
#include "sources/higgs_AudioSourcePlayer.cpp"
#include "sources/higgs_AudioTransportSource.cpp"

#if HIGGS_LINUX || HIGGS_BSD
 #include "native/higgs_Midi_linux.cpp"
#elif HIGGS_ANDROID
 #include "native/higgs_Midi_android.cpp"
#elif HIGGS_MAC || HIGGS_IOS
 #include "native/higgs_CoreMidi_mac.mm"
#elif HIGGS_WINDOWS
 #if HIGGS_USE_WINDOWS_MIDI_SERVICES
  static_assert (HIGGS_CXX20_IS_AVAILABLE, "Make sure C++20 is enabled to use Windows MIDI Services");

  HIGGS_BEGIN_IGNORE_WARNINGS_MSVC (4265)
  #include <winrt/Windows.Foundation.h>
  #include <winrt/Windows.Foundation.Collections.h>
  #include <winrt/Windows.Devices.Enumeration.h>

  #include <WindowsMidiServicesAppSdkComExtensions.h>
  #include <winrt/Windows.Devices.Midi2.h>
  #include <winrt/Windows.Devices.Midi2.Enumeration.h>
  #include <winrt/Windows.Devices.Midi2.Transports.Virtual.h>
  HIGGS_END_IGNORE_WARNINGS_MSVC
 #endif

 #include "native/higgs_Midi_windows.cpp"
#endif

#if HIGGS_UNIT_TESTS
 #include "midi_io/higgs_ScheduledEventThread_test.cpp"
#endif
