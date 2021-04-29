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

// The purpose of this internal header, included by higgs_core and higgs_graphics,
// is to apply consistent preprocessor definitions to zlib.h.

#pragma once

#define ZLIB_CONST 1
#define HAVE_MEMCPY 1

#if ! defined (HIGGS_INCLUDE_ZLIB_CODE) || HIGGS_INCLUDE_ZLIB_CODE
 #include "higgs_core/zip/zlib/zlib.h"
#else
 #include HIGGS_ZLIB_INCLUDE_PATH
#endif
