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

#if ! defined (HIGGS_INCLUDE_ZLIB_CODE) || HIGGS_INCLUDE_ZLIB_CODE

HIGGS_BEGIN_IGNORE_WARNINGS_GCC_LIKE ("-Wcast-align",
                                     "-Wshorten-64-to-32",
                                     "-Wsign-conversion",
                                     "-Wredundant-decls",
                                     "-Wshadow",
                                     "-Wcomma",
                                     "-Wimplicit-fallthrough")

HIGGS_BEGIN_IGNORE_WARNINGS_MSVC (6385 6326)

#include "higgs_core/zip/zlib/adler32.c"
#include "higgs_core/zip/zlib/compress.c"
#include "higgs_core/zip/zlib/crc32.c"
#include "higgs_core/zip/zlib/deflate.c"
#include "higgs_core/zip/zlib/inffast.c"
#undef GZIP
#include "higgs_core/zip/zlib/inflate.c"
#include "higgs_core/zip/zlib/inftrees.c"
#include "higgs_core/zip/zlib/trees.c"
#include "higgs_core/zip/zlib/zutil.c"

#else
// To get around 'empty translation unit' errors and 'has no symbols' warnings emitted by libtool
void higgs_coreZlibPlaceholder (void);
void higgs_coreZlibPlaceholder (void) {}
#endif
