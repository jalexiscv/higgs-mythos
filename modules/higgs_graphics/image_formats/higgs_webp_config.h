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

#ifndef HIGGS_USE_WEBP
 #define HIGGS_USE_WEBP 1
#endif

#ifndef HIGGS_INCLUDE_WEBPLIB_CODE
 #define HIGGS_INCLUDE_WEBPLIB_CODE 1
#endif

// libwebp includes <intrin.h> on x86 when _MSC_VER is defined. It is included
// here under the same condition so that every libwebp translation unit sees it
// before any SIMD target attribute is applied. Otherwise clang-cl would apply
// the attribute to the declarations in <intrin.h> as well, which conflicts with
// the always_inline definitions in clang's own intrinsic headers.
#if defined (_MSC_VER) && (defined (_M_X64) || defined (_M_IX86))
 #include <intrin.h>
#endif
