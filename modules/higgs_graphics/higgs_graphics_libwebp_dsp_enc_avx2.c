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

// This translation unit contains only AVX2 kernels that libwebp invokes
// through its cpuid-guarded dispatch, so it is safe to compile it with
// elevated instruction-set options

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

#if HIGGS_INTEL
 #if HIGGS_CLANG
  #pragma clang attribute push (__attribute__ ((target ("avx2"))), apply_to=function)
 #elif HIGGS_GCC
  #pragma GCC push_options
  #pragma GCC target ("avx2")
 #endif
#endif

#include "higgs_graphics/image_formats/libwebp/src/dsp/lossless_enc_avx2.c"

#if HIGGS_INTEL
 #if HIGGS_CLANG
  #pragma clang attribute pop
 #elif HIGGS_GCC
  #pragma GCC pop_options
 #endif
#endif

HIGGS_END_IGNORE_WARNINGS_GCC_LIKE
HIGGS_END_IGNORE_WARNINGS_MSVC

#else
 // To get around 'empty translation unit' errors
 enum { placeholder = 0 };
#endif
