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
                                     "-Wdeprecated-declarations",
                                     "-Wcomma",
                                     "-Wimplicit-fallthrough")

HIGGS_BEGIN_IGNORE_WARNINGS_MSVC (4100 4996 6385 6386 6387)

#include "higgs_graphics/image_formats/jpglib/jdapimin.c"
#include "higgs_graphics/image_formats/jpglib/jdapistd.c"
#include "higgs_graphics/image_formats/jpglib/jdarith.c"
#include "higgs_graphics/image_formats/jpglib/jdcoefct.c"
#include "higgs_graphics/image_formats/jpglib/jdcolor.c"
#include "higgs_graphics/image_formats/jpglib/jdmainct.c"
#include "higgs_graphics/image_formats/jpglib/jdmarker.c"
#include "higgs_graphics/image_formats/jpglib/jdmaster.c"
#include "higgs_graphics/image_formats/jpglib/jdpostct.c"
#include "higgs_graphics/image_formats/jpglib/jquant1.c"
#include "higgs_graphics/image_formats/jpglib/jdsample.c"

#else
// To get around 'empty translation unit' errors and 'has no symbols' warnings emitted by libtool
void higgs_graphicsLibjpg2Placeholder (void);
void higgs_graphicsLibjpg2Placeholder (void) {}
#endif
