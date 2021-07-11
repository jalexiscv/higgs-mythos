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

#if HIGGS_INCLUDE_PNGLIB_CODE || ! defined (HIGGS_INCLUDE_PNGLIB_CODE)

HIGGS_BEGIN_IGNORE_WARNINGS_GCC_LIKE ("-Wsign-conversion",
                                     "-Wfloat-equal",
                                     "-Wshadow",
                                     "-Wshorten-64-to-32",
                                     "-Wdeprecated-declarations",
                                     "-Wredundant-decls",
                                     "-Wimplicit-fallthrough")

HIGGS_BEGIN_IGNORE_WARNINGS_MSVC (4146 4996)

#include "higgs_graphics/image_formats/pnglib/png.c"
#include "higgs_graphics/image_formats/pnglib/pngerror.c"
#include "higgs_graphics/image_formats/pnglib/pngget.c"
#include "higgs_graphics/image_formats/pnglib/pngmem.c"
#include "higgs_graphics/image_formats/pnglib/pngread.c"
#include "higgs_graphics/image_formats/pnglib/pngpread.c"
#include "higgs_graphics/image_formats/pnglib/pngrio.c"
#include "higgs_graphics/image_formats/pnglib/pngrtran.c"

#define png_pass_start png_pass_start_2
#define png_pass_inc png_pass_inc_2
#define png_pass_ystart png_pass_ystart_2
#define png_pass_yinc png_pass_yinc_2

#include "higgs_graphics/image_formats/pnglib/pngrutil.c"

#undef png_pass_start
#undef png_pass_inc
#undef png_pass_ystart
#undef png_pass_yinc

#include "higgs_graphics/image_formats/pnglib/pngset.c"
#include "higgs_graphics/image_formats/pnglib/pngtrans.c"
#include "higgs_graphics/image_formats/pnglib/pngwio.c"
#include "higgs_graphics/image_formats/pnglib/pngwrite.c"
#include "higgs_graphics/image_formats/pnglib/pngwtran.c"

#define png_pass_start png_pass_start_3
#define png_pass_inc png_pass_inc_3
#define png_pass_ystart png_pass_ystart_3
#define png_pass_yinc png_pass_yinc_3

#include "higgs_graphics/image_formats/pnglib/pngwutil.c"

#undef png_pass_start
#undef png_pass_inc
#undef png_pass_ystart
#undef png_pass_yinc

#include "higgs_graphics/image_formats/pnglib/arm/arm_init.c"
#include "higgs_graphics/image_formats/pnglib/arm/filter_neon_intrinsics.c"
#include "higgs_graphics/image_formats/pnglib/arm/palette_neon_intrinsics.c"

#else
// To get around 'empty translation unit' errors and 'has no symbols' warnings emitted by libtool
void higgs_graphicsLibpngPlaceholder (void);
void higgs_graphicsLibpngPlaceholder (void) {}
#endif
