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

  ID:                 higgs_audio_formats
  vendor:             higgs
  version:            9.0.5
  name:               Higgs Mythos audio file format codecs
  description:        Classes for reading and writing various audio file formats.
  website:            https://github.com/jalexiscv/higgs-mythos
  license:            AGPLv3
  minimumCppStandard: 17

  dependencies:       higgs_audio_basics
  OSXFrameworks:      CoreAudio CoreMIDI QuartzCore AudioToolbox
  iOSFrameworks:      AudioToolbox QuartzCore

 END_HIGGS_MODULE_DECLARATION

*******************************************************************************/


#pragma once
#define HIGGS_AUDIO_FORMATS_H_INCLUDED

#include <higgs_audio_basics/higgs_audio_basics.h>

//==============================================================================
/** Config: HIGGS_USE_FLAC
    Enables the FLAC audio codec classes (available on all platforms).
    If your app doesn't need to read FLAC files, you might want to disable this to
    reduce the size of your codebase and build time.
*/
#ifndef HIGGS_USE_FLAC
 #define HIGGS_USE_FLAC 1
#endif

/** Config: HIGGS_INCLUDE_FLAC_CODE
    This can be used to disable Higgs's embedded 3rd-party flaclib code.
    You might need to tweak this if you're linking to an external flaclib library in your app,
    but for normal apps, this option should be left alone.

    If you disable this, you might also want to set a value for HIGGS_FLAC_INCLUDE_PATH, to
    specify the path where your flaclib headers live.

    This config option only has an effect when HIGGS_USE_FLAC is enabled.
*/
#ifndef HIGGS_INCLUDE_FLAC_CODE
 #define HIGGS_INCLUDE_FLAC_CODE 1
#endif

#ifndef HIGGS_FLAC_INCLUDE_PATH
 #define HIGGS_FLAC_INCLUDE_PATH <FLAC/all.h>
#endif

/** Config: HIGGS_USE_OGGVORBIS
    Enables the Ogg-Vorbis audio codec classes (available on all platforms).
    If your app doesn't need to read Ogg-Vorbis files, you might want to disable this to
    reduce the size of your codebase and build time.
*/
#ifndef HIGGS_USE_OGGVORBIS
 #define HIGGS_USE_OGGVORBIS 1
#endif

/** Config: HIGGS_USE_OPUS
    Enables the Opus audio codec classes (available on all platforms).
    If your app doesn't need to read or write Opus files, you might want to disable this to
    reduce the size of your codebase and build time.
*/
#ifndef HIGGS_USE_OPUS
 #define HIGGS_USE_OPUS 1
#endif

/** Config: HIGGS_INCLUDE_OPUS_CODE
    This can be used to disable Higgs's embedded 3rd-party opus, opusfile and libopusenc code.
    You might need to tweak this if you're linking to external copies of these libraries in
    your app, but for normal apps, this option should be left alone.

    The embedded libogg is compiled when this option or HIGGS_INCLUDE_OGGVORBIS_CODE is
    enabled. If only one of them is, it may collide with the libogg that the external
    library depends on.

    This config option only has an effect with HIGGS_USE_OPUS is enabled.
*/
#ifndef HIGGS_INCLUDE_OPUS_CODE
 #define HIGGS_INCLUDE_OPUS_CODE 1
#endif

/** Config: HIGGS_USE_MP3AUDIOFORMAT
    Enables the software-based MP3AudioFormat class.
    If your app doesn't need to read MP3 files, you might want to disable this to
    reduce the size of your codebase and build time.
*/
#ifndef HIGGS_USE_MP3AUDIOFORMAT
 #define HIGGS_USE_MP3AUDIOFORMAT 1
#endif

/** Config: HIGGS_USE_LAME_AUDIO_FORMAT
    Enables the LameEncoderAudioFormat class.
*/
#ifndef HIGGS_USE_LAME_AUDIO_FORMAT
 #define HIGGS_USE_LAME_AUDIO_FORMAT 0
#endif

/** Config: HIGGS_USE_WINDOWS_MEDIA_FORMAT
    Enables the Windows Media SDK codecs.
*/
#ifndef HIGGS_USE_WINDOWS_MEDIA_FORMAT
 #define HIGGS_USE_WINDOWS_MEDIA_FORMAT 1
#endif

#if ! HIGGS_WINDOWS
 #undef HIGGS_USE_WINDOWS_MEDIA_FORMAT
 #define HIGGS_USE_WINDOWS_MEDIA_FORMAT 0
#endif

//==============================================================================
#include "format/higgs_AudioFormatReader.h"
#include "format/higgs_AudioFormatWriterOptions.h"
#include "format/higgs_AudioFormatWriter.h"
#include "format/higgs_MemoryMappedAudioFormatReader.h"
#include "format/higgs_AudioFormat.h"
#include "format/higgs_AudioFormatManager.h"
#include "format/higgs_AudioFormatReaderSource.h"
#include "format/higgs_AudioSubsectionReader.h"
#include "format/higgs_BufferingAudioFormatReader.h"
#include "codecs/higgs_AiffAudioFormat.h"
#include "codecs/higgs_CoreAudioFormat.h"
#include "codecs/higgs_FlacAudioFormat.h"
#include "codecs/higgs_LAMEEncoderAudioFormat.h"
#include "codecs/higgs_MP3AudioFormat.h"
#include "codecs/higgs_OggVorbisAudioFormat.h"
#include "codecs/higgs_OpusAudioFormat.h"
#include "codecs/higgs_WavAudioFormat.h"
#include "codecs/higgs_WindowsMediaAudioFormat.h"
#include "sampler/higgs_Sampler.h"

#if HiggsPlugin_Enable_ARA
 #include <higgs_audio_processors/higgs_audio_processors.h>

 #include "format/higgs_ARAAudioReaders.h"
#endif
