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

#include "higgs_graphics/image_formats/libwebp/src/enc/alpha_enc.c"
#include "higgs_graphics/image_formats/libwebp/src/enc/analysis_enc.c"
#include "higgs_graphics/image_formats/libwebp/src/enc/backward_references_cost_enc.c"
#include "higgs_graphics/image_formats/libwebp/src/enc/backward_references_enc.c"
#include "higgs_graphics/image_formats/libwebp/src/enc/config_enc.c"
#include "higgs_graphics/image_formats/libwebp/src/enc/cost_enc.c"
#include "higgs_graphics/image_formats/libwebp/src/enc/filter_enc.c"
#define GetPSNR higgs_webp_frame_enc_GetPSNR
#include "higgs_graphics/image_formats/libwebp/src/enc/frame_enc.c"
#undef GetPSNR
#include "higgs_graphics/image_formats/libwebp/src/enc/histogram_enc.c"
#include "higgs_graphics/image_formats/libwebp/src/enc/iterator_enc.c"
#include "higgs_graphics/image_formats/libwebp/src/enc/near_lossless_enc.c"
#include "higgs_graphics/image_formats/libwebp/src/enc/picture_csp_enc.c"
#include "higgs_graphics/image_formats/libwebp/src/enc/picture_enc.c"
#define GetPSNR higgs_webp_picture_psnr_enc_GetPSNR
#include "higgs_graphics/image_formats/libwebp/src/enc/picture_psnr_enc.c"
#undef GetPSNR
#include "higgs_graphics/image_formats/libwebp/src/enc/picture_rescale_enc.c"
#include "higgs_graphics/image_formats/libwebp/src/enc/picture_tools_enc.c"
#define NearLossless higgs_webp_predictor_enc_NearLossless
#include "higgs_graphics/image_formats/libwebp/src/enc/predictor_enc.c"
#undef NearLossless
#undef MAX_ALPHA
#define clip higgs_webp_quant_enc_clip
#include "higgs_graphics/image_formats/libwebp/src/enc/quant_enc.c"
#undef clip
#include "higgs_graphics/image_formats/libwebp/src/enc/syntax_enc.c"
#include "higgs_graphics/image_formats/libwebp/src/enc/token_enc.c"
#include "higgs_graphics/image_formats/libwebp/src/enc/tree_enc.c"
#include "higgs_graphics/image_formats/libwebp/src/enc/vp8l_enc.c"
#include "higgs_graphics/image_formats/libwebp/src/enc/webp_enc.c"

HIGGS_END_IGNORE_WARNINGS_GCC_LIKE
HIGGS_END_IGNORE_WARNINGS_MSVC

#else
 // To get around 'empty translation unit' errors
 enum { placeholder = 0 };
#endif
