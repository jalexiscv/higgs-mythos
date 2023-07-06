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

#include "higgs_audio_formats/codecs/opus/higgs_opus_config.h"
#include <higgs_core/system/higgs_CompilerWarnings.h>

#if HIGGS_USE_OPUS && (HIGGS_INCLUDE_OPUS_CODE || ! defined (HIGGS_INCLUDE_OPUS_CODE))

HIGGS_BEGIN_IGNORE_WARNINGS_GCC_LIKE ("-Wbitwise-op-parentheses",
                                     "-Wconditional-uninitialized",
                                     "-Wconversion",
                                     "-Wfloat-equal",
                                     "-Wlanguage-extension-token",
                                     "-Wlogical-op-parentheses",
                                     "-Wmacro-redefined",
                                     "-Wmissing-prototypes",
                                     "-Wnull-pointer-subtraction",
                                     "-Wshadow",
                                     "-Wshift-op-parentheses",
                                     "-Wsign-compare",
                                     "-Wsign-conversion")
HIGGS_BEGIN_IGNORE_WARNINGS_MSVC (4267 4127 4244 4100 4701 4702 4013 4133 4206 4305 4189 4706 4995 4365 4456 4457 4459 6297 6011 6001 6308 6255 6386 6385 6246 6387 6263 6262 28182 4232 4245 4018 6326)

#include "higgs_audio_formats/codecs/opus/opusfile/src/info.c"
#include "higgs_audio_formats/codecs/opus/opusfile/src/internal.c"
#include "higgs_audio_formats/codecs/opus/opusfile/src/opusfile.c"
#include "higgs_audio_formats/codecs/opus/opusfile/src/stream.c"
#include "higgs_audio_formats/codecs/opus/opusfile/src/http.c"

#include "higgs_audio_formats/codecs/opus/libopusenc/package_version.h"

// libopusenc's bundled speex resampler
#define OUTSIDE_SPEEX 1
#define RANDOM_PREFIX libopusenc

// ogg_packer.c is a mini-fork of libogg's page writer: its static
// ogg_page_checksum_set conflicts with the declaration in ogg.h, and its
// shift_buffer with the one in opusenc.c
#define ogg_page_checksum_set libopusenc_ogg_page_checksum_set
#define shift_buffer libopusenc_shift_buffer
#include "higgs_audio_formats/codecs/opus/libopusenc/src/ogg_packer.c"
#undef ogg_page_checksum_set
#undef shift_buffer
#include "higgs_audio_formats/codecs/opus/libopusenc/src/opus_header.c"
#include "higgs_audio_formats/codecs/opus/libopusenc/src/opusenc.c"
#include "higgs_audio_formats/codecs/opus/libopusenc/src/picture.c"
#include "higgs_audio_formats/codecs/opus/libopusenc/src/resample.c"
#include "higgs_audio_formats/codecs/opus/libopusenc/src/unicode_support.c"

HIGGS_END_IGNORE_WARNINGS_MSVC
HIGGS_END_IGNORE_WARNINGS_GCC_LIKE

#else
// To get around 'empty translation unit' errors and 'has no symbols' warnings emitted by libtool
void higgs_audioFormatsOpusfilePlaceholder (void);
void higgs_audioFormatsOpusfilePlaceholder (void) {}
#endif
