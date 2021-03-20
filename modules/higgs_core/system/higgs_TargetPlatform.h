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
/*  This file figures out which platform is being built, and defines some macros
    that the rest of the code can use for OS-specific compilation.

    Macros that will be set here are:

    - One of HIGGS_WINDOWS, HIGGS_MAC HIGGS_LINUX, HIGGS_IOS, HIGGS_ANDROID, etc.
    - Either HIGGS_32BIT or HIGGS_64BIT, depending on the architecture.
    - Either HIGGS_LITTLE_ENDIAN or HIGGS_BIG_ENDIAN.
    - Either HIGGS_INTEL or HIGGS_ARM
    - Either HIGGS_GCC or HIGGS_CLANG or HIGGS_MSVC
*/

//==============================================================================
#ifdef HIGGS_APP_CONFIG_HEADER
 #include HIGGS_APP_CONFIG_HEADER
#elif ! defined (HIGGS_GLOBAL_MODULE_SETTINGS_INCLUDED)
 /*
    Most projects will contain a global header file containing various settings that
    should be applied to all the code in your project. If you use the mythosstudio, it'll
    set up a global header file for you automatically, but if you're doing things manually,
    you may want to set the HIGGS_APP_CONFIG_HEADER macro with the name of a file to include,
    or just include one before all the module cpp files, in which you set
    HIGGS_GLOBAL_MODULE_SETTINGS_INCLUDED=1 to silence this error.
    (Or if you don't need a global header, then you can just define HIGGS_GLOBAL_MODULE_SETTINGS_INCLUDED
    globally to avoid this error).

    Note for people who hit this error when trying to compile a HIGGS project created by
    a pre-v4.2 version of the Intromythos/MythosStudio, it's very easy to fix: just re-save
    your project with the latest version of the MythosStudio, and it'll magically fix this!
 */
 #error "No global header file was included!"
#endif

//==============================================================================
#if defined (_WIN32) || defined (_WIN64)
  #define       HIGGS_WINDOWS 1
#elif defined (HIGGS_ANDROID)
  #undef        HIGGS_ANDROID
  #define       HIGGS_ANDROID 1
#elif defined (__FreeBSD__) || defined (__OpenBSD__)
  #define       HIGGS_BSD 1
#elif defined (LINUX) || defined (__linux__)
  #define       HIGGS_LINUX 1
#elif defined (__APPLE_CPP__) || defined (__APPLE_CC__)
  #define CF_EXCLUDE_CSTD_HEADERS 1
  #include <TargetConditionals.h> // (needed to find out what platform we're using)
  #include <AvailabilityMacros.h>

  #if TARGET_OS_IPHONE || TARGET_IPHONE_SIMULATOR
    #define     HIGGS_IPHONE 1
    #define     HIGGS_IOS 1
  #else
    #define     HIGGS_MAC 1
  #endif
#elif defined (__wasm__)
  #define       HIGGS_WASM 1
#else
  #error "Unknown platform!"
#endif

//==============================================================================
#if HIGGS_WINDOWS
  #ifdef _MSC_VER
    #ifdef _WIN64
      #define HIGGS_64BIT 1
    #else
      #define HIGGS_32BIT 1
    #endif
  #endif

  #ifdef _DEBUG
    #define HIGGS_DEBUG 1
  #endif

  #ifdef __MINGW32__
    #error "MinGW is not supported. Please use an alternative compiler."
  #endif

  /** If defined, this indicates that the processor is little-endian. */
  #define HIGGS_LITTLE_ENDIAN 1

  #if defined (_M_ARM) || defined (_M_ARM64) || defined (_M_ARM64EC) || defined (__arm__) || defined (__aarch64__)
    #define HIGGS_ARM 1
  #else
    #define HIGGS_INTEL 1
  #endif
#endif

//==============================================================================
#if HIGGS_MAC || HIGGS_IOS

// Expands to true if the API of the specified version is available at build time, false otherwise
#define HIGGS_MAC_API_VERSION_CAN_BE_BUILT(major, minor) \
    ((major) * 10000 + (minor) * 100 <= __MAC_OS_X_VERSION_MAX_ALLOWED)

// Expands to true if the API of the specified version is available at build time, false otherwise
#define HIGGS_IOS_API_VERSION_CAN_BE_BUILT(major, minor) \
    ((major) * 10000 + (minor) * 100 <= __IPHONE_OS_VERSION_MAX_ALLOWED)

// Expands to true if the deployment target is greater or equal to the specified version, false otherwise
#define HIGGS_MAC_API_VERSION_MIN_REQUIRED_AT_LEAST(major, minor) \
    ((major) * 10000 + (minor) * 100 <= __MAC_OS_X_VERSION_MIN_REQUIRED)

// Expands to true if the deployment target is greater or equal to the specified version, false otherwise
#define HIGGS_IOS_API_VERSION_MIN_REQUIRED_AT_LEAST(major, minor) \
    ((major) * 10000 + (minor) * 100 <= __IPHONE_OS_VERSION_MIN_REQUIRED)

#if defined (DEBUG) || defined (_DEBUG) || ! (defined (NDEBUG) || defined (_NDEBUG))
    #define HIGGS_DEBUG 1
  #endif

  #if ! (defined (DEBUG) || defined (_DEBUG) || defined (NDEBUG) || defined (_NDEBUG))
    #warning "Neither NDEBUG or DEBUG has been defined - you should set one of these to make it clear whether this is a release build,"
  #endif

  #ifdef __LITTLE_ENDIAN__
    #define HIGGS_LITTLE_ENDIAN 1
  #else
    #define HIGGS_BIG_ENDIAN 1
  #endif

  #ifdef __LP64__
    #define HIGGS_64BIT 1
  #else
    #define HIGGS_32BIT 1
  #endif

  #if defined (__ppc__) || defined (__ppc64__)
    #error "PowerPC is no longer supported by HIGGS!"
  #elif defined (__arm__) || defined (__arm64__)
    #define HIGGS_ARM 1
  #else
    #define HIGGS_INTEL 1
  #endif

  #if HIGGS_MAC
    #if ! HIGGS_MAC_API_VERSION_CAN_BE_BUILT (11, 1)
      #error "The macOS 11.1 SDK (Xcode 12.4+) is required to build HIGGS apps. You can create apps that run on macOS 10.11+ by changing the deployment target."
    #elif ! HIGGS_MAC_API_VERSION_MIN_REQUIRED_AT_LEAST (10, 11)
      #error "Building for OSX 10.10 and earlier is no longer supported!"
    #endif
  #endif
#endif

//==============================================================================
#if HIGGS_LINUX || HIGGS_ANDROID || HIGGS_BSD

  #ifdef _DEBUG
    #define HIGGS_DEBUG 1
  #endif

  // Allow override for big-endian Linux platforms
  #if defined (__LITTLE_ENDIAN__) || ! defined (HIGGS_BIG_ENDIAN)
    #define HIGGS_LITTLE_ENDIAN 1
    #undef HIGGS_BIG_ENDIAN
  #else
    #undef HIGGS_LITTLE_ENDIAN
    #define HIGGS_BIG_ENDIAN 1
  #endif

  #if defined (__LP64__) || defined (_LP64) || defined (__arm64__) || defined (__aarch64__)
    #define HIGGS_64BIT 1
  #else
    #define HIGGS_32BIT 1
  #endif

  #if defined (__arm__) || defined (__arm64__) || defined (__aarch64__)
    #define HIGGS_ARM 1
  #elif __MMX__ || __SSE__ || __amd64__
    #define HIGGS_INTEL 1
  #endif
#endif

//==============================================================================
// Compiler type macros.

#if defined (__clang__)
  #define HIGGS_CLANG 1

#elif defined (__GNUC__)
  #define HIGGS_GCC 1

#elif defined (_MSC_VER)
  #define HIGGS_MSVC 1

#else
  #error unknown compiler
#endif

//==============================================================================
// Instruction set capabilities

#ifndef HIGGS_USE_SSE_INTRINSICS
 #define HIGGS_USE_SSE_INTRINSICS 1
#endif

#if ! HIGGS_INTEL
 #undef HIGGS_USE_SSE_INTRINSICS
#endif

#ifndef HIGGS_USE_ARM_NEON
 #if (__ARM_NEON || __ARM_NEON__ || _M_ARM64 || _M_ARM64EC) && ! HIGGS_USE_VDSP_FRAMEWORK
  #define HIGGS_USE_ARM_NEON 1
 #endif
#endif

#if TARGET_IPHONE_SIMULATOR && ! (TARGET_CPU_ARM || TARGET_CPU_ARM64)
 #ifdef HIGGS_USE_ARM_NEON
  #undef HIGGS_USE_ARM_NEON
 #endif
 #define HIGGS_USE_ARM_NEON 0
#endif
