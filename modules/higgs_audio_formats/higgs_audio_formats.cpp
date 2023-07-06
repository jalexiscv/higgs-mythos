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

#ifdef HIGGS_AUDIO_FORMATS_H_INCLUDED
 /* When you add this cpp file to your project, you mustn't include it in a file where you've
    already included any other headers - just put it inside a file on its own, possibly with your config
    flags preceding it, but don't include anything else. That also includes avoiding any automatic prefix
    header files that the compiler may be using.
 */
 #error "Incorrect use of HIGGS cpp file"
#endif

#define HIGGS_CORE_INCLUDE_COM_SMART_PTR 1
#define HIGGS_CORE_INCLUDE_JNI_HELPERS 1
#define HIGGS_CORE_INCLUDE_NATIVE_HEADERS 1

#include "higgs_audio_formats.h"

//==============================================================================
#if HIGGS_MAC
 #include <AudioToolbox/AudioToolbox.h>

#elif HIGGS_IOS
 #import <AudioToolbox/AudioToolbox.h>
 #import <AVFoundation/AVFoundation.h>

//==============================================================================
#elif HIGGS_WINDOWS && HIGGS_USE_WINDOWS_MEDIA_FORMAT
 #include <wmsdk.h>
#endif

//==============================================================================
#include "format/higgs_AudioFormat.cpp"
#include "format/higgs_AudioFormatManager.cpp"
#include "format/higgs_AudioFormatReader.cpp"
#include "format/higgs_AudioFormatReaderSource.cpp"
#include "format/higgs_AudioFormatWriter.cpp"
#include "format/higgs_AudioSubsectionReader.cpp"
#include "format/higgs_BufferingAudioFormatReader.cpp"
#include "sampler/higgs_Sampler.cpp"
#include "codecs/higgs_AiffAudioFormat.cpp"
#include "codecs/higgs_CoreAudioFormat.cpp"
#include "codecs/higgs_FlacAudioFormat.cpp"
#include "codecs/higgs_MP3AudioFormat.cpp"
#include "codecs/higgs_OggVorbisAudioFormat.cpp"
#include "codecs/higgs_OpusAudioFormat.cpp"
#include "codecs/higgs_WavAudioFormat.cpp"
#include "codecs/higgs_LAMEEncoderAudioFormat.cpp"

#if HiggsPlugin_Enable_ARA
 #include <higgs_audio_processors_headless/utilities/ARA/higgs_ARADocumentControllerCommon.h>
 #include "format/higgs_ARAAudioReaders.cpp"
#endif

#if HIGGS_WINDOWS && HIGGS_USE_WINDOWS_MEDIA_FORMAT
 #include "codecs/higgs_WindowsMediaAudioFormat.cpp"
#endif
