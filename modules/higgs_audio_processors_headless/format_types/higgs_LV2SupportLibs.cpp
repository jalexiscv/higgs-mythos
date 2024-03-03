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

#include "higgs_lv2_config.h"

HIGGS_BEGIN_IGNORE_WARNINGS_GCC_LIKE ("-Wc99-extensions",
                                     "-Wcast-align",
                                     "-Wconversion",
                                     "-Wextra-semi",
                                     "-Wfloat-conversion",
                                     "-Wfloat-equal",
                                     "-Wformat-overflow",
                                     "-Wimplicit-float-conversion",
                                     "-Wimplicit-int-conversion",
                                     "-Wmicrosoft-include",
                                     "-Wmissing-field-initializers",
                                     "-Wnullability-extension",
                                     "-Wnullable-to-nonnull-conversion",
                                     "-Wparentheses",
                                     "-Wpedantic",
                                     "-Wredundant-decls",
                                     "-Wshorten-64-to-32",
                                     "-Wsign-conversion",
                                     "-Wswitch-enum",
                                     "-Wunused-parameter",
                                     "-Wzero-as-null-pointer-constant",
                                     "-Wimplicit-fallthrough")
HIGGS_BEGIN_IGNORE_WARNINGS_MSVC (4100 4200 4244 4267 4389 4702 4706 4800 6308 28182 28183 6385 6386 6387 6011 6282 6323 6330 6001 6031)
HIGGS_BEGIN_IGNORE_DEPRECATION_WARNINGS

extern "C"
{

#include <math.h>

#pragma push_macro ("_POSIX_C_SOURCE")
#undef _POSIX_C_SOURCE

#define is_windows_path serd_is_windows_path

#include "serd/src/base64.c"
#include "serd/src/byte_source.c"
#include "serd/src/env.c"
#include "serd/src/n3.c"
#undef TRY

#include "serd/src/node.c"
#include "serd/src/reader.c"
#include "serd/src/string.c"
#include "serd/src/system.c"
#include "serd/src/uri.c"
#include "serd/src/writer.c"

#undef is_windows_path

#include "sord/src/sord.c"
#include "sord/src/syntax.c"

#include "lilv/src/collections.c"
#include "lilv/src/filesystem.c"
#include "lilv/src/instance.c"
#include "lilv/src/lib.c"
#include "lilv/src/node.c"
#include "lilv/src/plugin.c"
#include "lilv/src/pluginclass.c"
#include "lilv/src/port.c"
#include "lilv/src/query.c"
#include "lilv/src/scalepoint.c"
#include "lilv/src/state.c"
#include "lilv/src/ui.c"
#include "lilv/src/util.c"
#include "lilv/src/world.c"
#include "lilv/src/zix/tree.c"

#pragma pop_macro ("_POSIX_C_SOURCE")

#undef NS_RDF
#undef NS_XSD
#undef USTR

#define read_object sratom_read_object
#define read_literal sratom_read_literal

#pragma push_macro ("nil")
#undef nil
#include "sratom/src/sratom.c"
#pragma pop_macro ("nil")

} // extern "C"

HIGGS_END_IGNORE_DEPRECATION_WARNINGS
HIGGS_END_IGNORE_WARNINGS_MSVC
HIGGS_END_IGNORE_WARNINGS_GCC_LIKE
