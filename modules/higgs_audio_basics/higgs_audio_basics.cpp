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

#ifdef HIGGS_AUDIO_BASICS_H_INCLUDED
 /* When you add this cpp file to your project, you mustn't include it in a file where you've
    already included any other headers - just put it inside a file on its own, possibly with your config
    flags preceding it, but don't include anything else. That also includes avoiding any automatic prefix
    header files that the compiler may be using.
 */
 #error "Incorrect use of HIGGS cpp file"
#endif

#include "higgs_audio_basics.h"

#if HIGGS_USE_SSE_INTRINSICS
 #include <emmintrin.h>
#endif

#if HIGGS_MAC || HIGGS_IOS
 #ifndef HIGGS_USE_VDSP_FRAMEWORK
  #define HIGGS_USE_VDSP_FRAMEWORK 1
 #endif

 #if HIGGS_USE_VDSP_FRAMEWORK
  #include <Accelerate/Accelerate.h>
 #endif

 #include "native/higgs_AudioWorkgroup_mac.h"

#elif HIGGS_USE_VDSP_FRAMEWORK
 #undef HIGGS_USE_VDSP_FRAMEWORK
#endif

#if HIGGS_USE_ARM_NEON
 #include <arm_neon.h>
#endif

#include "buffers/higgs_AudioDataConverters.cpp"
#include "buffers/higgs_FloatVectorOperations.cpp"
#include "buffers/higgs_AudioChannelSet.cpp"
#include "buffers/higgs_AudioProcessLoadMeasurer.cpp"
#include "utilities/higgs_IIRFilter.cpp"
#include "utilities/higgs_LagrangeInterpolator.cpp"
#include "utilities/higgs_WindowedSincInterpolator.cpp"
#include "utilities/higgs_Interpolators.cpp"
#include "utilities/higgs_SmoothedValue.cpp"
#include "midi/higgs_MidiBuffer.cpp"
#include "midi/higgs_MidiFile.cpp"
#include "midi/higgs_MidiKeyboardState.cpp"
#include "midi/higgs_MidiMessage.cpp"
#include "midi/higgs_MidiMessageSequence.cpp"
#include "midi/higgs_MidiRPN.cpp"
#include "mpe/higgs_MPEValue.cpp"
#include "mpe/higgs_MPENote.cpp"
#include "mpe/higgs_MPEZoneLayout.cpp"
#include "mpe/higgs_MPEInstrument.cpp"
#include "mpe/higgs_MPEMessages.cpp"
#include "mpe/higgs_MPESynthesiserBase.cpp"
#include "mpe/higgs_MPESynthesiserVoice.cpp"
#include "mpe/higgs_MPESynthesiser.cpp"
#include "mpe/higgs_MPEUtils.cpp"
#include "sources/higgs_BufferingAudioSource.cpp"
#include "sources/higgs_ChannelRemappingAudioSource.cpp"
#include "sources/higgs_IIRFilterAudioSource.cpp"
#include "sources/higgs_MemoryAudioSource.cpp"
#include "sources/higgs_MixerAudioSource.cpp"
#include "sources/higgs_ResamplingAudioSource.cpp"
#include "sources/higgs_ReverbAudioSource.cpp"
#include "sources/higgs_ToneGeneratorAudioSource.cpp"
#include "sources/higgs_PositionableAudioSource.cpp"
#include "synthesisers/higgs_Synthesiser.cpp"
#include "audio_play_head/higgs_AudioPlayHead.cpp"
#include "utilities/higgs_AudioWorkgroup.cpp"

#include "midi/ump/higgs_UMPDeviceInfo.cpp"
#include "midi/ump/higgs_UMPFactory.cpp"
#include "midi/ump/higgs_UMPUtils.cpp"
#include "midi/ump/higgs_UMPView.cpp"
#include "midi/ump/higgs_UMPSysEx7.cpp"
#include "midi/ump/higgs_UMPMidi1ToMidi2DefaultTranslator.cpp"
#include "midi/ump/higgs_UMPIterator.cpp"
#include "midi/ump/higgs_UMPStringUtils.cpp"

#if HIGGS_UNIT_TESTS
 #include "utilities/higgs_ADSR_test.cpp"
 #include "midi/higgs_MidiDataConcatenator_test.cpp"
 #include "midi/ump/higgs_UMP_test.cpp"
#endif
