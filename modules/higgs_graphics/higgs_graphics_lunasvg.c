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

#define HIGGS_PLUTOVG_BUILD
#define HIGGS_PLUTOVG_BUILD_STATIC

HIGGS_BEGIN_IGNORE_WARNINGS_MSVC (4018 4100 4244 4267 4324 4389 4996 6011 6308 6387 6385 6386 6262 28182)
HIGGS_BEGIN_IGNORE_WARNINGS_GCC_LIKE ("-Wcast-align",
                                     "-Wfloat-conversion",
                                     "-Wfloat-equal",
                                     "-Wimplicit-int-conversion",
                                     "-Wmissing-prototypes",
                                     "-Wshorten-64-to-32",
                                     "-Wsign-compare",
                                     "-Wsign-conversion",
                                     "-Wswitch-enum",
                                     "-Wunused-function",
                                     "-Wunused-parameter",
                                     "-Wcomma",
                                     "-Wimplicit-int-float-conversion",
                                     "-Wunused-but-set-variable",
                                     "-Wdeprecated-declarations",
                                     "-Wimplicit-fallthrough")

#include <higgs_graphics/drawables/lunasvg/plutovg/source/plutovg-blend.c>
#include <higgs_graphics/drawables/lunasvg/plutovg/source/plutovg-canvas.c>
#include <higgs_graphics/drawables/lunasvg/plutovg/source/plutovg-font.c>
#include <higgs_graphics/drawables/lunasvg/plutovg/source/plutovg-ft-math.c>
#include <higgs_graphics/drawables/lunasvg/plutovg/source/plutovg-ft-raster.c>
#include <higgs_graphics/drawables/lunasvg/plutovg/source/plutovg-ft-stroker.c>
#include <higgs_graphics/drawables/lunasvg/plutovg/source/plutovg-matrix.c>
#include <higgs_graphics/drawables/lunasvg/plutovg/source/plutovg-paint.c>
#include <higgs_graphics/drawables/lunasvg/plutovg/source/plutovg-path.c>
#include <higgs_graphics/drawables/lunasvg/plutovg/source/plutovg-rasterize.c>
#include <higgs_graphics/drawables/lunasvg/plutovg/source/plutovg-surface.c>

HIGGS_END_IGNORE_WARNINGS_GCC_LIKE
HIGGS_END_IGNORE_WARNINGS_MSVC

#undef HIGGS_PLUTOVG_BUILD_STATIC
#undef HIGGS_PLUTOVG_BUILD
