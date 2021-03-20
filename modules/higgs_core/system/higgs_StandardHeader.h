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

//==============================================================================
/** Current HIGGS version number.

    See also SystemStats::getHIGGSVersion() for a string version.
*/
#define HIGGS_MAJOR_VERSION      9
#define HIGGS_MINOR_VERSION      0
#define HIGGS_BUILDNUMBER        5

/** Current HIGGS version number.

    Bits 16 to 32 = major version.
    Bits 8 to 16 = minor version.
    Bits 0 to 8 = point release.

    See also SystemStats::getHIGGSVersion() for a string version.
*/
#define HIGGS_VERSION   ((HIGGS_MAJOR_VERSION << 16) + (HIGGS_MINOR_VERSION << 8) + HIGGS_BUILDNUMBER)

/** @cond */
#define HIGGS_VERSION_ID \
    [[maybe_unused]] volatile auto higgsVersionId = "higgs_version_" HIGGS_STRINGIFY(HIGGS_MAJOR_VERSION) "_" HIGGS_STRINGIFY(HIGGS_MINOR_VERSION) "_" HIGGS_STRINGIFY(HIGGS_BUILDNUMBER);
/** @endcond */

//==============================================================================
#include "higgs_CompilerSupport.h"
#include "higgs_CompilerWarnings.h"

//==============================================================================
#include <algorithm>
#include <array>
#include <atomic>
#include <cmath>
#include <condition_variable>
#include <cstddef>

// There's a template specialisation to std::wstring_convert in
// wstring_convert.h that triggers a warning on Xcode 26.4.1 when used by the
// VST3 SDK. It cannot be suppressed at the call site. This file is included
// in <locale>, and would transitively be included by <functional>.
#if defined (__apple_build_version__)
HIGGS_BEGIN_IGNORE_WARNINGS_GCC_LIKE ("-Wdeprecated-declarations")
#include <locale>
HIGGS_END_IGNORE_WARNINGS_GCC_LIKE
#endif

#include <functional>
#include <future>
#include <iomanip>
#include <iostream>
#include <limits>
#include <list>
#include <map>
#include <memory>
#include <mutex>
#include <numeric>
#include <optional>
#include <queue>
#include <set>
#include <sstream>
#include <string_view>
#include <thread>
#include <type_traits>
#include <typeindex>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <variant>
#include <vector>

//==============================================================================
#include "higgs_PlatformDefs.h"

//==============================================================================
// Now we'll include some common OS headers.
HIGGS_BEGIN_IGNORE_WARNINGS_MSVC (4514 4245 4100)

#if HIGGS_MSVC
 #include <intrin.h>
#endif


#if HIGGS_MAC || HIGGS_IOS
 #include <libkern/OSAtomic.h>
 #include <libkern/OSByteOrder.h>
 #include <xlocale.h>
 #include <signal.h>
#endif

#if HIGGS_LINUX || HIGGS_BSD
 #include <cstring>
 #include <signal.h>

 #if __INTEL_COMPILER
  #if __ia64__
   #include <ia64intrin.h>
  #else
   #include <ia32intrin.h>
  #endif
 #endif
#endif

#if HIGGS_MSVC && HIGGS_DEBUG
 #include <crtdbg.h>
#endif

HIGGS_END_IGNORE_WARNINGS_MSVC

#if HIGGS_ANDROID
 #include <cstring>
 #include <byteswap.h>
#endif

// undef symbols that are sometimes set by misguided 3rd-party headers
#undef TYPE_BOOL
#undef max
#undef min
#undef major
#undef minor
#undef KeyPress

//==============================================================================
// DLL building settings on Windows
#if HIGGS_MSVC
 #ifdef HIGGS_DLL_BUILD
  #define HIGGS_API __declspec (dllexport)
  #pragma warning (disable: 4251)
 #elif defined (HIGGS_DLL)
  #define HIGGS_API __declspec (dllimport)
  #pragma warning (disable: 4251)
 #endif
 #ifdef __INTEL_COMPILER
  #pragma warning (disable: 1125) // (virtual override warning)
 #endif
#elif defined (HIGGS_DLL) || defined (HIGGS_DLL_BUILD)
 #define HIGGS_API __attribute__ ((visibility ("default")))
#endif

//==============================================================================
#ifndef HIGGS_API
 /** This macro is added to all HIGGS public class declarations. */
 #define HIGGS_API
#endif

#if HIGGS_MSVC && HIGGS_DLL_BUILD
 #define HIGGS_PUBLIC_IN_DLL_BUILD(declaration)  public: declaration; private:
#else
 #define HIGGS_PUBLIC_IN_DLL_BUILD(declaration)  declaration;
#endif

/** This macro is added to all HIGGS public function declarations. */
#define HIGGS_PUBLIC_FUNCTION        HIGGS_API HIGGS_CALLTYPE

/** @cond */
#define HIGGS_NAMESPACE higgs  // This old macro is deprecated: you should just use the higgs namespace directly.
/** @endcond */
