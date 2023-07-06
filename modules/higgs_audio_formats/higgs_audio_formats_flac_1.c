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

 #include "higgs_audio_formats/codecs/flac/libFLAC/stream_encoder.c"
 #include "higgs_audio_formats/codecs/flac/libFLAC/stream_encoder_intrin_avx2.c"
 #include "higgs_audio_formats/codecs/flac/libFLAC/stream_encoder_intrin_sse2.c"
 #include "higgs_audio_formats/codecs/flac/libFLAC/stream_encoder_intrin_ssse3.c"
 #include "higgs_audio_formats/codecs/flac/libFLAC/fixed_intrin_avx2.c"
 #include "higgs_audio_formats/codecs/flac/libFLAC/fixed_intrin_sse2.c"
 #include "higgs_audio_formats/codecs/flac/libFLAC/fixed_intrin_sse42.c"
 #include "higgs_audio_formats/codecs/flac/libFLAC/fixed_intrin_ssse3.c"

 #ifdef _WIN32
  #include "higgs_audio_formats/codecs/flac/win_utf8_io/win_utf8_io.c"
 #endif
#else
 // To get around 'empty translation unit' errors and 'has no symbols' warnings emitted by libtool
 void higgs_audioFormatsFlac1Placeholder (void);
 void higgs_audioFormatsFlac1Placeholder (void) {}
#endif
