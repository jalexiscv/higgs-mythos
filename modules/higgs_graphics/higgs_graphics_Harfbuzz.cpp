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

HIGGS_BEGIN_IGNORE_WARNINGS_MSVC (4100 4127 4189 4244 4245 4265 4267 4309 4310 4312 4456 4457 4458 4459 4701 4702 4706 6001 6011 6239 6244 6246 6262 6297 6313 6319 6326 6336 6385 6386 28251 28199 6237 4146)

HIGGS_BEGIN_IGNORE_WARNINGS_GCC_LIKE ("-Wcast-function-type",
                                     "-Wsign-conversion",
                                     "-Wzero-as-null-pointer-constant",
                                     "-Wformat-pedantic",
                                     "-Wextra-semi",
                                     "-Wc++98-compat-extra-semi",
                                     "-Wshadow-field",
                                     "-Wfloat-equal",
                                     "-Wformat",
                                     "-Wpedantic",
                                     "-Wmicrosoft-exception-spec",
                                     "-Wmicrosoft-cast",
                                     "-Wconditional-uninitialized",
                                     "-Wexpansion-to-defined",
                                     "-Wunsafe-loop-optimizations",
                                     "-Wformat-overflow",
                                     "-Woverflow",
                                     "-Wnontrivial-memcall",
                                     "-Wimplicit-fallthrough",
                                     "-Wdeprecated-copy-with-user-provided-dtor")

HIGGS_BEGIN_IGNORE_DEPRECATION_WARNINGS

#define HAVE_ATEXIT 1

#if HIGGS_LINUX || HIGGS_BSD
 #ifndef HIGGS_USE_FREETYPE
  #define HIGGS_USE_FREETYPE 1
 #endif
#endif

#if HIGGS_USE_FREETYPE
 #define HAVE_FREETYPE 1
#endif

#if HIGGS_WINDOWS
 #define HAVE_DIRECTWRITE 1
 #define _CRT_SECURE_NO_WARNINGS 1
#elif HIGGS_MAC || HIGGS_IOS
 #define HAVE_CORETEXT 1
#endif

// HB enables some warnings in its header, but we would rather build cleanly
#define HB_NO_PRAGMA_GCC_DIAGNOSTIC_WARNING
#define HB_NO_PRAGMA_GCC_DIAGNOSTIC_ERROR

// This is a hack, because harfbuzz headers define hb_has_builtin to
// expand to a macro that includes a 'defines' expression, which triggers
// -Wexpansion-to-defined on gcc 7. There's no way to turn that warning off
// locally, so we sidestep it.
#if ! defined(__has_builtin) && defined(__GNUC__) && __GNUC__ >= 5
 #define __has_builtin(x) 1
#endif

#include <utility>
#include <higgs_graphics/fonts/harfbuzz/hb.hh>
#include <higgs_graphics/fonts/harfbuzz/harfbuzz.cc>

#undef HAVE_DIRECTWRITE
#undef HAVE_FREETYPE
#undef HAVE_CORETEXT

HIGGS_END_IGNORE_DEPRECATION_WARNINGS
HIGGS_END_IGNORE_WARNINGS_GCC_LIKE
HIGGS_END_IGNORE_WARNINGS_MSVC
