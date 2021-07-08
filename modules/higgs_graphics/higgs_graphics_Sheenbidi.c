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

HIGGS_BEGIN_IGNORE_WARNINGS_GCC_LIKE ("-Wsign-conversion",
                                     "-Wcast-align",
                                     "-Wunused-variable",
                                     "-Wunused-function",
                                     "-Wstringop-overflow",
                                     "-Wimplicit-fallthrough")

HIGGS_BEGIN_IGNORE_WARNINGS_MSVC (4189 4706 6011)

//==============================================================================

#define SB_CONFIG_UNITY 1

#include <higgs_graphics/unicode/sheenbidi/Source/SheenBidi.c>

HIGGS_END_IGNORE_WARNINGS_MSVC
HIGGS_END_IGNORE_WARNINGS_GCC_LIKE
