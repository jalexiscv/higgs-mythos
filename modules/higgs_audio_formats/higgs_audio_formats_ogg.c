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

#include <higgs_core/system/higgs_CompilerWarnings.h>

#ifndef HIGGS_USE_OGGVORBIS
 #define HIGGS_USE_OGGVORBIS 1
#endif

#ifndef HIGGS_USE_OPUS
 #define HIGGS_USE_OPUS 1
#endif

#if (HIGGS_USE_OGGVORBIS && (HIGGS_INCLUDE_OGGVORBIS_CODE || ! defined (HIGGS_INCLUDE_OGGVORBIS_CODE))) \
 || (HIGGS_USE_OPUS && (HIGGS_INCLUDE_OPUS_CODE || ! defined (HIGGS_INCLUDE_OPUS_CODE)))

HIGGS_BEGIN_IGNORE_WARNINGS_GCC_LIKE ("-Wconversion",
                                     "-Wsign-conversion")
HIGGS_BEGIN_IGNORE_WARNINGS_MSVC (4267 4127 4244 4100 4701 4702 4013 4133 4206 4305 4189 4706 4995 4365 4456 4457 4459 6297 6011 6001 6308 6255 6386 6385 6246 6387 6263 6262 28182)
HIGGS_BEGIN_NO_SANITIZE ("undefined")

#include "higgs_audio_formats/codecs/ogg/src/bitwise.c"
#include "higgs_audio_formats/codecs/ogg/src/framing.c"

HIGGS_END_NO_SANITIZE
HIGGS_END_IGNORE_WARNINGS_MSVC
HIGGS_END_IGNORE_WARNINGS_GCC_LIKE

#else
// To get around 'empty translation unit' errors and 'has no symbols' warnings emitted by libtool
void higgs_audioFormatsOggPlaceholder (void);
void higgs_audioFormatsOggPlaceholder (void) {}
#endif
