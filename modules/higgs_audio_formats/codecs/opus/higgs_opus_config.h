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

#include <higgs_core/system/higgs_TargetPlatform.h>

#ifndef HIGGS_USE_OPUS
 #define HIGGS_USE_OPUS 1
#endif

#if HIGGS_USE_OPUS && (HIGGS_INCLUDE_OPUS_CODE || ! defined (HIGGS_INCLUDE_OPUS_CODE))

 // The standard floating-point build of libopus. The SIMD tiers that every
 // CPU of the architecture supports (NEON on 64-bit Arm, SSE and SSE2 on
 // 64-bit x86) are presumed at compile time, so no runtime CPU detection is
 // needed. The higher x86 tiers are not enabled, as their sources require
 // per-file compiler flags.
 #define OPUS_BUILD 1
 #define USE_ALLOCA 1

 // libopus requires that FLOAT_APPROX is defined when building with __FAST_MATH__
 #if defined (__FAST_MATH__) && ! defined (FLOAT_APPROX)
  #define FLOAT_APPROX 1
 #endif

 #if ! HIGGS_MSVC
  #define HAVE_LRINT 1
  #define HAVE_LRINTF 1
 #endif

 #if HIGGS_ARM && HIGGS_64BIT
  #define OPUS_ARM_MAY_HAVE_NEON_INTR 1
  #define OPUS_ARM_PRESUME_NEON_INTR 1
 #endif

 #if HIGGS_INTEL && HIGGS_64BIT
  #define OPUS_X86_MAY_HAVE_SSE 1
  #define OPUS_X86_PRESUME_SSE 1
  #define OPUS_X86_MAY_HAVE_SSE2 1
  #define OPUS_X86_PRESUME_SSE2 1
 #endif

#endif
