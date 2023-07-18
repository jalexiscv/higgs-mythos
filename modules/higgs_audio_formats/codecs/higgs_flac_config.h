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

#ifndef HIGGS_USE_FLAC
 #define HIGGS_USE_FLAC 1
#endif

#include <higgs_core/system/higgs_CompilerWarnings.h>

HIGGS_BEGIN_IGNORE_WARNINGS_GCC_LIKE ("-Wempty-translation-unit")

#if HIGGS_USE_FLAC

#if HIGGS_INCLUDE_FLAC_CODE || ! defined (HIGGS_INCLUDE_FLAC_CODE)

 #define PACKAGE_VERSION "1.5.0"

 #define FLAC__NO_DLL 1
 #define FLAC__HAS_OGG 0

 HIGGS_BEGIN_IGNORE_WARNINGS_MSVC (4267 4127 4244 4996 4100 4701 4702 4706 4013 4133 4206 4312 4505 4365 4005 4334 181 111 6340 6308 6297 6001 6320 4245 6239 6326 6237 6031)
 #if ! HIGGS_MSVC
  #define HAVE_LROUND 1
 #endif

 #if HIGGS_MAC
  #define FLAC__SYS_DARWIN 1
 #endif

 #ifndef SIZE_MAX
  #define SIZE_MAX 0xffffffff
 #endif

 HIGGS_BEGIN_IGNORE_WARNINGS_GCC_LIKE ("-Wconversion",
                                      "-Wdeprecated-register",
                                      "-Wfloat-equal",
                                      "-Wimplicit-fallthrough",
                                      "-Wlanguage-extension-token",
                                      "-Wredundant-decls",
                                      "-Wshadow",
                                      "-Wsign-conversion",
                                      "-Wswitch-default",
                                      "-Wswitch-enum",
                                      "-Wzero-as-null-pointer-constant",
                                      "-Wstatic-in-inline",
                                      "-Wconditional-uninitialized",
                                      "-Wunused-parameter",
                                      "-Wunused-variable",
                                      "-Wcast-align",
                                      "-Wpedantic")

 #if HIGGS_INTEL
  #if HIGGS_32BIT
   #define FLAC__CPU_IA32 1
  #endif
  #if HIGGS_64BIT
   #define FLAC__CPU_X86_64 1
  #endif
  #define FLAC__HAS_X86INTRIN 1
 #endif

 #if HIGGS_ARM && HIGGS_64BIT
  #define FLAC__CPU_ARM64 1

  #if HIGGS_USE_ARM_NEON
    #define FLAC__HAS_NEONINTRIN 1
    #define FLAC__HAS_A64NEONINTRIN 1
  #endif
 #endif

 #undef  DEBUG  // (some flac code dumps debug trace if the app defines this macro)

 #ifndef NDEBUG
  #define NDEBUG // (some flac code prints cpu info if this isn't defined)
 #endif

#endif

#endif
