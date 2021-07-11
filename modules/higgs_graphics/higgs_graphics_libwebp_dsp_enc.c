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

#include <higgs_graphics/image_formats/higgs_webp_config.h>

#if HIGGS_USE_WEBP && HIGGS_INCLUDE_WEBPLIB_CODE

HIGGS_BEGIN_IGNORE_WARNINGS_MSVC (4127 4244 4245 4310 4701 6001 6011 6239 6286 6287 6297 6326 6385 6387)

HIGGS_BEGIN_IGNORE_WARNINGS_GCC_LIKE ("-Wsign-conversion",
                                     "-Wimplicit-int-conversion",
                                     "-Wimplicit-int-float-conversion",
                                     "-Wfloat-conversion",
                                     "-Wcast-align",
                                     "-Wswitch-enum",
                                     "-Wconditional-uninitialized",
                                     "-Wfloat-equal",
                                     "-Wredundant-decls",
                                     "-Wpedantic",
                                     "-Woverflow",
                                     "-Wzero-as-null-pointer-constant")

// The SSE4.1 and AVX2 kernels live in higgs_graphics_libwebp_dsp_enc_sse41.c
// and higgs_graphics_libwebp_dsp_enc_avx2.c, which may be compiled with
// elevated instruction-set options that must not apply to the unguarded code
// in this file. Defining WEBP_HAVE_* here enables the cpuid-guarded dispatch
// to those kernels.
#if (defined(__i386__) || defined(__x86_64__) || defined(_M_IX86) || defined(_M_X64)) \
     && ! defined(_M_ARM64EC)
 #define WEBP_HAVE_SSE41
 #define WEBP_HAVE_AVX2
#endif

#include "higgs_graphics/image_formats/libwebp/src/dsp/cost.c"
#include "higgs_graphics/image_formats/libwebp/src/dsp/cost_neon.c"
#include "higgs_graphics/image_formats/libwebp/src/dsp/cost_sse2.c"
#include "higgs_graphics/image_formats/libwebp/src/dsp/enc.c"
#include "higgs_graphics/image_formats/libwebp/src/dsp/enc_neon.c"
#include "higgs_graphics/image_formats/libwebp/src/dsp/enc_sse2.c"
#include "higgs_graphics/image_formats/libwebp/src/dsp/lossless_enc.c"
#include "higgs_graphics/image_formats/libwebp/src/dsp/lossless_enc_neon.c"
#include "higgs_graphics/image_formats/libwebp/src/dsp/lossless_enc_sse2.c"
#include "higgs_graphics/image_formats/libwebp/src/dsp/ssim.c"
#define kWeight higgs_webp_ssim_sse2_kWeight
#include "higgs_graphics/image_formats/libwebp/src/dsp/ssim_sse2.c"
#undef kWeight

HIGGS_END_IGNORE_WARNINGS_GCC_LIKE
HIGGS_END_IGNORE_WARNINGS_MSVC

#else
 // To get around 'empty translation unit' errors
 enum { placeholder = 0 };
#endif
