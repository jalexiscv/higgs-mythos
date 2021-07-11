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

#if ! defined (HIGGS_INCLUDE_JPEGLIB_CODE) || HIGGS_INCLUDE_JPEGLIB_CODE

HIGGS_BEGIN_IGNORE_WARNINGS_GCC_LIKE ("-Wsign-conversion",
                                     "-Wimplicit-int-conversion",
                                     "-Wunused-parameter",
                                     "-Wswitch-enum",
                                     "-Wimplicit-fallthrough")

HIGGS_BEGIN_IGNORE_WARNINGS_MSVC (4100 4996 6385 28182)

#include "higgs_graphics/image_formats/jpglib/jcdctmgr.c"
#include "higgs_graphics/image_formats/jpglib/jddctmgr.c"
#include "higgs_graphics/image_formats/jpglib/jidctflt.c"

#undef CONST_BITS
#undef DEQUANTIZE
#include "higgs_graphics/image_formats/jpglib/jidctfst.c"

#undef CONST_BITS
#undef DEQUANTIZE
#undef FIX_1_847759065
#undef MULTIPLY
#include "higgs_graphics/image_formats/jpglib/jidctint.c"

#undef FIX
#include "higgs_graphics/image_formats/jpglib/jdmerge.c"

#undef FIX
#include "higgs_graphics/image_formats/jpglib/jccolor.c"

#else
// To get around 'empty translation unit' errors and 'has no symbols' warnings emitted by libtool
void higgs_graphicsLibjpg3Placeholder (void);
void higgs_graphicsLibjpg3Placeholder (void) {}
#endif
