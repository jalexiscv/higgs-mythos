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

  ID:                 higgs_audio_basics
  vendor:             higgs
  version:            9.0.5
  name:               Higgs Mythos audio and MIDI data classes
  description:        Classes for audio buffer manipulation, midi message handling, synthesis, etc.
  website:            https://github.com/jalexiscv/higgs-mythos
  license:            AGPLv3
  minimumCppStandard: 17

  dependencies:       higgs_core
  OSXFrameworks:      Accelerate
  iOSFrameworks:      Accelerate

 END_HIGGS_MODULE_DECLARATION

*******************************************************************************/


#pragma once
#define HIGGS_AUDIO_BASICS_H_INCLUDED

#include <higgs_core/higgs_core.h>

//==============================================================================
#undef Complex  // apparently some C libraries actually define these symbols (!)
#undef Factor

//==============================================================================
#include "buffers/higgs_AudioDataConverters.h"
HIGGS_BEGIN_IGNORE_WARNINGS_MSVC (4661)
#include "buffers/higgs_FloatVectorOperations.h"
HIGGS_END_IGNORE_WARNINGS_MSVC
#include "buffers/higgs_AudioSampleBuffer.h"
#include "buffers/higgs_AudioChannelSet.h"
#include "buffers/higgs_AudioProcessLoadMeasurer.h"
#include "utilities/higgs_Decibels.h"
#include "utilities/higgs_IIRFilter.h"
#include "utilities/higgs_GenericInterpolator.h"
#include "utilities/higgs_Interpolators.h"
#include "utilities/higgs_SmoothedValue.h"
#include "utilities/higgs_Reverb.h"
#include "utilities/higgs_ADSR.h"
#include "midi/higgs_MidiMessage.h"
#include "midi/higgs_MidiBuffer.h"
#include "midi/higgs_MidiMessageSequence.h"
#include "midi/higgs_MidiFile.h"
#include "midi/higgs_MidiKeyboardState.h"
#include "midi/higgs_MidiRPN.h"
#include "midi/higgs_MidiDataConcatenator.h"
#include "mpe/higgs_MPEValue.h"
#include "mpe/higgs_MPENote.h"
#include "mpe/higgs_MPEZoneLayout.h"
#include "mpe/higgs_MPEInstrument.h"
#include "mpe/higgs_MPEMessages.h"
#include "mpe/higgs_MPESynthesiserBase.h"
#include "mpe/higgs_MPESynthesiserVoice.h"
#include "mpe/higgs_MPESynthesiser.h"
#include "mpe/higgs_MPEUtils.h"
#include "sources/higgs_AudioSource.h"
#include "sources/higgs_PositionableAudioSource.h"
#include "sources/higgs_BufferingAudioSource.h"
#include "sources/higgs_ChannelRemappingAudioSource.h"
#include "sources/higgs_IIRFilterAudioSource.h"
#include "sources/higgs_MemoryAudioSource.h"
#include "sources/higgs_MixerAudioSource.h"
#include "sources/higgs_ResamplingAudioSource.h"
#include "sources/higgs_ReverbAudioSource.h"
#include "sources/higgs_ToneGeneratorAudioSource.h"
#include "synthesisers/higgs_Synthesiser.h"
#include "audio_play_head/higgs_AudioPlayHead.h"
#include "utilities/higgs_AudioWorkgroup.h"
#include "midi/ump/higgs_UMPBytesOnGroup.h"
#include "midi/ump/higgs_UMPDeviceInfo.h"
#include "midi/ump/higgs_UMP.h"

namespace higgs
{
    namespace ump = universal_midi_packets;
}
