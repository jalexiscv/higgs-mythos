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

#pragma once

#include <higgs_core/system/higgs_PlatformDefs.h>

/** @cond */
#ifndef HIGGS_API
 #define HIGGS_API
#endif
/** @endcond */

#if HIGGS_INTERNAL_HAS_ARA || (HiggsPlugin_Enable_ARA && (HIGGS_MAC || HIGGS_WINDOWS || HIGGS_LINUX))

namespace higgs
{

//==============================================================================
 #if (HIGGS_DEBUG && ! HIGGS_DISABLE_ASSERTIONS) || HIGGS_LOG_ASSERTIONS
  #define ARA_ENABLE_INTERNAL_ASSERTS 1
 #else
  #define ARA_ENABLE_INTERNAL_ASSERTS 0
 #endif // (HIGGS_DEBUG && ! HIGGS_DISABLE_ASSERTIONS) || HIGGS_LOG_ASSERTIONS

//==============================================================================
 #if ARA_ENABLE_INTERNAL_ASSERTS

HIGGS_API void HIGGS_CALLTYPE handleARAAssertion (const char* file, const int line, const char* diagnosis) noexcept;

  #if !defined(ARA_HANDLE_ASSERT)
   #define ARA_HANDLE_ASSERT(file, line, diagnosis)    higgs::handleARAAssertion (file, line, diagnosis)
  #endif

  #if HIGGS_LOG_ASSERTIONS
   #define ARA_ENABLE_DEBUG_OUTPUT 1
  #endif

 #endif

} // namespace higgs

HIGGS_BEGIN_IGNORE_WARNINGS_GCC_LIKE ("-Wgnu-zero-variadic-macro-arguments", "-Wmissing-prototypes")
 #include <ARA_Library/Debug/ARADebug.h>
HIGGS_END_IGNORE_WARNINGS_GCC_LIKE

#endif
