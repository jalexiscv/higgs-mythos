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

#include "higgs_audio_formats/codecs/higgs_flac_config.h"

#if HIGGS_USE_FLAC && (HIGGS_INCLUDE_FLAC_CODE || ! defined (HIGGS_INCLUDE_FLAC_CODE))
 HIGGS_BEGIN_IGNORE_WARNINGS_GCC_LIKE ("-Wdeprecated-declarations")

 #include "higgs_audio_formats/codecs/flac/all.h"
 #include "higgs_audio_formats/codecs/flac/libFLAC/bitmath.c"
 #include "higgs_audio_formats/codecs/flac/libFLAC/bitreader.c"
 #include "higgs_audio_formats/codecs/flac/libFLAC/bitwriter.c"
 #include "higgs_audio_formats/codecs/flac/libFLAC/cpu.c"
 #include "higgs_audio_formats/codecs/flac/libFLAC/crc.c"
 #include "higgs_audio_formats/codecs/flac/libFLAC/fixed.c"
 #include "higgs_audio_formats/codecs/flac/libFLAC/float.c"
 #include "higgs_audio_formats/codecs/flac/libFLAC/format.c"
 #include "higgs_audio_formats/codecs/flac/libFLAC/lpc_flac.c"
 #include "higgs_audio_formats/codecs/flac/libFLAC/lpc_intrin_avx2.c"
 #include "higgs_audio_formats/codecs/flac/libFLAC/lpc_intrin_fma.c"
 #include "higgs_audio_formats/codecs/flac/libFLAC/lpc_intrin_neon.c"
 #include "higgs_audio_formats/codecs/flac/libFLAC/lpc_intrin_sse2.c"
 #include "higgs_audio_formats/codecs/flac/libFLAC/lpc_intrin_sse41.c"
 #include "higgs_audio_formats/codecs/flac/libFLAC/md5.c"
 #include "higgs_audio_formats/codecs/flac/libFLAC/memory.c"
 #include "higgs_audio_formats/codecs/flac/libFLAC/stream_decoder.c"
 #include "higgs_audio_formats/codecs/flac/libFLAC/stream_encoder_framing.c"
 #include "higgs_audio_formats/codecs/flac/libFLAC/window_flac.c"
#else
 // To get around 'empty translation unit' errors and 'has no symbols' warnings emitted by libtool
 void higgs_audioFormatsFlac2Placeholder (void);
 void higgs_audioFormatsFlac2Placeholder (void) {}
#endif
