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
                                     "-Wcast-align",
                                     "-Wimplicit-fallthrough")

HIGGS_BEGIN_IGNORE_WARNINGS_MSVC (4100 4127 4996 6001 6011 6326 6385 6386 6387 28183)

#include "higgs_graphics/image_formats/jpglib/jcapimin.c"
#include "higgs_graphics/image_formats/jpglib/jcapistd.c"
#include "higgs_graphics/image_formats/jpglib/jccoefct.c"
#include "higgs_graphics/image_formats/jpglib/jcinit.c"
#include "higgs_graphics/image_formats/jpglib/jcmainct.c"
#include "higgs_graphics/image_formats/jpglib/jcmarker.c"
#include "higgs_graphics/image_formats/jpglib/jcmaster.c"
#include "higgs_graphics/image_formats/jpglib/jcomapi.c"
#include "higgs_graphics/image_formats/jpglib/jcparam.c"
#include "higgs_graphics/image_formats/jpglib/jcprepct.c"
#include "higgs_graphics/image_formats/jpglib/jcsample.c"
#include "higgs_graphics/image_formats/jpglib/jmemmgr.c"
#include "higgs_graphics/image_formats/jpglib/jmemnobs.c"
#include "higgs_graphics/image_formats/jpglib/jquant2.c"
#include "higgs_graphics/image_formats/jpglib/jutils.c"

#define initial_setup dedup_initial_setup
#define per_scan_setup dedup_per_scan_setup
#include "higgs_graphics/image_formats/jpglib/jdinput.c"
#undef initial_setup
#undef per_scan_setup

#define emit_byte dedup_emit_byte
#include "higgs_graphics/image_formats/jpglib/jcarith.c"
#undef emit_byte

#define encode_mcu_DC_first dedup_encode_mcu_DC_first
#define encode_mcu_AC_first dedup_encode_mcu_AC_first
#define encode_mcu_DC_refine dedup_encode_mcu_DC_refine
#define encode_mcu_AC_refine dedup_encode_mcu_AC_refine
#include "higgs_graphics/image_formats/jpglib/jchuff.c"
#undef encode_mcu_DC_first
#undef encode_mcu_AC_first
#undef encode_mcu_DC_refine
#undef encode_mcu_AC_refine

#include "higgs_graphics/image_formats/jpglib/jaricom.c"

#undef CONST_BITS
#include "higgs_graphics/image_formats/jpglib/jfdctint.c"
#include "higgs_graphics/image_formats/jpglib/jfdctflt.c"

#undef CONST_BITS
#undef FIX_0_541196100
#undef MULTIPLY
#include "higgs_graphics/image_formats/jpglib/jfdctfst.c"

#define savable_state dedup_savable_state
#define huff_entropy_ptr dedup_huff_entryopy_ptr
#define finish_pass_huff dedup_finish_pass_huff
#include "higgs_graphics/image_formats/jpglib/jdhuff.c"
#undef savable_state
#undef huff_entropy_ptr
#undef finish_pass_huff

#else
// To get around 'empty translation unit' errors and 'has no symbols' warnings emitted by libtool
void higgs_graphicsLibjpg1Placeholder (void);
void higgs_graphicsLibjpg1Placeholder (void) {}
#endif
