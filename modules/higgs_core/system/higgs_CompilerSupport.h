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

/*
   This file provides flags for compiler features that aren't supported on all platforms.
*/

//==============================================================================
// GCC
#if HIGGS_GCC

 #if (__GNUC__ * 100 + __GNUC_MINOR__) < 700
  #error "HIGGS requires GCC 7.0 or later"
 #endif

 #ifndef HIGGS_EXCEPTIONS_DISABLED
  #if ! __EXCEPTIONS
   #define HIGGS_EXCEPTIONS_DISABLED 1
  #endif
 #endif

 #define HIGGS_CXX17_IS_AVAILABLE (__cplusplus >= 201703L)
 #define HIGGS_CXX20_IS_AVAILABLE (__cplusplus >= 202002L)

#endif

//==============================================================================
// Clang
#if HIGGS_CLANG

 #if (__clang_major__ < 6)
  #error "HIGGS requires Clang 6 or later"
 #endif

 #ifndef HIGGS_COMPILER_SUPPORTS_ARC
  #define HIGGS_COMPILER_SUPPORTS_ARC 1
 #endif

 #ifndef HIGGS_EXCEPTIONS_DISABLED
  #if ! __has_feature (cxx_exceptions)
   #define HIGGS_EXCEPTIONS_DISABLED 1
  #endif
 #endif

 #if ! defined (HIGGS_SILENCE_XCODE_15_LINKER_WARNING)                          \
     && defined (__apple_build_version__)                                      \
     && __apple_build_version__ >= 15000000                                    \
     && __apple_build_version__ <  15000100

  // Due to known issues, the linker in Xcode 15.0 may produce broken binaries.
  #error Please upgrade to Xcode 15.1 or higher
 #endif

 #define HIGGS_CXX17_IS_AVAILABLE (__cplusplus >= 201703L)
 #define HIGGS_CXX20_IS_AVAILABLE (__cplusplus >= 202002L)

#endif

//==============================================================================
// MSVC
#if HIGGS_MSVC

 #if _MSC_FULL_VER < 191025017  // VS2017
   #error "HIGGS requires Visual Studio 2017 or later"
 #endif

 #ifndef HIGGS_EXCEPTIONS_DISABLED
  #if ! _CPPUNWIND
   #define HIGGS_EXCEPTIONS_DISABLED 1
  #endif
 #endif

  #define HIGGS_CXX17_IS_AVAILABLE (_MSVC_LANG >= 201703L)
  #define HIGGS_CXX20_IS_AVAILABLE (_MSVC_LANG >= 202002L)
#endif

//==============================================================================
#if ! HIGGS_CXX17_IS_AVAILABLE
 #error "HIGGS requires C++17 or later"
#endif

//==============================================================================
// These are old flags that are now supported on all compatible build targets
/** @cond */
#define HIGGS_CXX14_IS_AVAILABLE 1
#define HIGGS_COMPILER_SUPPORTS_OVERRIDE_AND_FINAL 1
#define HIGGS_COMPILER_SUPPORTS_VARIADIC_TEMPLATES 1
#define HIGGS_COMPILER_SUPPORTS_INITIALIZER_LISTS 1
#define HIGGS_COMPILER_SUPPORTS_NOEXCEPT 1
#define HIGGS_DELETED_FUNCTION = delete
#define HIGGS_CONSTEXPR constexpr
#define HIGGS_NODISCARD [[nodiscard]]
/** @endcond */
