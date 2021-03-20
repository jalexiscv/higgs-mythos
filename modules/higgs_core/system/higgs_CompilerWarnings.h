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

#include "higgs_TargetPlatform.h"

/** @cond */
/** Return the Nth argument. By passing a variadic pack followed by N other
    parameters, we can select one of those N parameter based on the length of
    the parameter pack.
*/
#define HIGGS_NTH_ARG_(_00, _01, _02, _03, _04, _05, _06, _07, _08, _09,        \
                      _10, _11, _12, _13, _14, _15, _16, _17, _18, _19,        \
                      _20, _21, _22, _23, _24, _25, _26, _27, _28, _29,        \
                      _30, _31, _32, _33, _34, _35, _36, _37, _38, _39,        \
                      _40, _41, _42, _43, _44, _45, _46, _47, _48, _49,        \
                      _50, _51, _52, _53, _54, _55, _56, _57, _58, _59, N, ...)\
    N

#define HIGGS_EACH_00_(FN)
#define HIGGS_EACH_01_(FN, X)      FN(X)
#define HIGGS_EACH_02_(FN, X, ...) FN(X) HIGGS_EACH_01_(FN, __VA_ARGS__)
#define HIGGS_EACH_03_(FN, X, ...) FN(X) HIGGS_EACH_02_(FN, __VA_ARGS__)
#define HIGGS_EACH_04_(FN, X, ...) FN(X) HIGGS_EACH_03_(FN, __VA_ARGS__)
#define HIGGS_EACH_05_(FN, X, ...) FN(X) HIGGS_EACH_04_(FN, __VA_ARGS__)
#define HIGGS_EACH_06_(FN, X, ...) FN(X) HIGGS_EACH_05_(FN, __VA_ARGS__)
#define HIGGS_EACH_07_(FN, X, ...) FN(X) HIGGS_EACH_06_(FN, __VA_ARGS__)
#define HIGGS_EACH_08_(FN, X, ...) FN(X) HIGGS_EACH_07_(FN, __VA_ARGS__)
#define HIGGS_EACH_09_(FN, X, ...) FN(X) HIGGS_EACH_08_(FN, __VA_ARGS__)
#define HIGGS_EACH_10_(FN, X, ...) FN(X) HIGGS_EACH_09_(FN, __VA_ARGS__)
#define HIGGS_EACH_11_(FN, X, ...) FN(X) HIGGS_EACH_10_(FN, __VA_ARGS__)
#define HIGGS_EACH_12_(FN, X, ...) FN(X) HIGGS_EACH_11_(FN, __VA_ARGS__)
#define HIGGS_EACH_13_(FN, X, ...) FN(X) HIGGS_EACH_12_(FN, __VA_ARGS__)
#define HIGGS_EACH_14_(FN, X, ...) FN(X) HIGGS_EACH_13_(FN, __VA_ARGS__)
#define HIGGS_EACH_15_(FN, X, ...) FN(X) HIGGS_EACH_14_(FN, __VA_ARGS__)
#define HIGGS_EACH_16_(FN, X, ...) FN(X) HIGGS_EACH_15_(FN, __VA_ARGS__)
#define HIGGS_EACH_17_(FN, X, ...) FN(X) HIGGS_EACH_16_(FN, __VA_ARGS__)
#define HIGGS_EACH_18_(FN, X, ...) FN(X) HIGGS_EACH_17_(FN, __VA_ARGS__)
#define HIGGS_EACH_19_(FN, X, ...) FN(X) HIGGS_EACH_18_(FN, __VA_ARGS__)
#define HIGGS_EACH_20_(FN, X, ...) FN(X) HIGGS_EACH_19_(FN, __VA_ARGS__)
#define HIGGS_EACH_21_(FN, X, ...) FN(X) HIGGS_EACH_20_(FN, __VA_ARGS__)
#define HIGGS_EACH_22_(FN, X, ...) FN(X) HIGGS_EACH_21_(FN, __VA_ARGS__)
#define HIGGS_EACH_23_(FN, X, ...) FN(X) HIGGS_EACH_22_(FN, __VA_ARGS__)
#define HIGGS_EACH_24_(FN, X, ...) FN(X) HIGGS_EACH_23_(FN, __VA_ARGS__)
#define HIGGS_EACH_25_(FN, X, ...) FN(X) HIGGS_EACH_24_(FN, __VA_ARGS__)
#define HIGGS_EACH_26_(FN, X, ...) FN(X) HIGGS_EACH_25_(FN, __VA_ARGS__)
#define HIGGS_EACH_27_(FN, X, ...) FN(X) HIGGS_EACH_26_(FN, __VA_ARGS__)
#define HIGGS_EACH_28_(FN, X, ...) FN(X) HIGGS_EACH_27_(FN, __VA_ARGS__)
#define HIGGS_EACH_29_(FN, X, ...) FN(X) HIGGS_EACH_28_(FN, __VA_ARGS__)
#define HIGGS_EACH_30_(FN, X, ...) FN(X) HIGGS_EACH_29_(FN, __VA_ARGS__)
#define HIGGS_EACH_31_(FN, X, ...) FN(X) HIGGS_EACH_30_(FN, __VA_ARGS__)
#define HIGGS_EACH_32_(FN, X, ...) FN(X) HIGGS_EACH_31_(FN, __VA_ARGS__)
#define HIGGS_EACH_33_(FN, X, ...) FN(X) HIGGS_EACH_32_(FN, __VA_ARGS__)
#define HIGGS_EACH_34_(FN, X, ...) FN(X) HIGGS_EACH_33_(FN, __VA_ARGS__)
#define HIGGS_EACH_35_(FN, X, ...) FN(X) HIGGS_EACH_34_(FN, __VA_ARGS__)
#define HIGGS_EACH_36_(FN, X, ...) FN(X) HIGGS_EACH_35_(FN, __VA_ARGS__)
#define HIGGS_EACH_37_(FN, X, ...) FN(X) HIGGS_EACH_36_(FN, __VA_ARGS__)
#define HIGGS_EACH_38_(FN, X, ...) FN(X) HIGGS_EACH_37_(FN, __VA_ARGS__)
#define HIGGS_EACH_39_(FN, X, ...) FN(X) HIGGS_EACH_38_(FN, __VA_ARGS__)
#define HIGGS_EACH_40_(FN, X, ...) FN(X) HIGGS_EACH_39_(FN, __VA_ARGS__)
#define HIGGS_EACH_41_(FN, X, ...) FN(X) HIGGS_EACH_40_(FN, __VA_ARGS__)
#define HIGGS_EACH_42_(FN, X, ...) FN(X) HIGGS_EACH_41_(FN, __VA_ARGS__)
#define HIGGS_EACH_43_(FN, X, ...) FN(X) HIGGS_EACH_42_(FN, __VA_ARGS__)
#define HIGGS_EACH_44_(FN, X, ...) FN(X) HIGGS_EACH_43_(FN, __VA_ARGS__)
#define HIGGS_EACH_45_(FN, X, ...) FN(X) HIGGS_EACH_44_(FN, __VA_ARGS__)
#define HIGGS_EACH_46_(FN, X, ...) FN(X) HIGGS_EACH_45_(FN, __VA_ARGS__)
#define HIGGS_EACH_47_(FN, X, ...) FN(X) HIGGS_EACH_46_(FN, __VA_ARGS__)
#define HIGGS_EACH_48_(FN, X, ...) FN(X) HIGGS_EACH_47_(FN, __VA_ARGS__)
#define HIGGS_EACH_49_(FN, X, ...) FN(X) HIGGS_EACH_48_(FN, __VA_ARGS__)
#define HIGGS_EACH_50_(FN, X, ...) FN(X) HIGGS_EACH_49_(FN, __VA_ARGS__)
#define HIGGS_EACH_51_(FN, X, ...) FN(X) HIGGS_EACH_50_(FN, __VA_ARGS__)
#define HIGGS_EACH_52_(FN, X, ...) FN(X) HIGGS_EACH_51_(FN, __VA_ARGS__)
#define HIGGS_EACH_53_(FN, X, ...) FN(X) HIGGS_EACH_52_(FN, __VA_ARGS__)
#define HIGGS_EACH_54_(FN, X, ...) FN(X) HIGGS_EACH_53_(FN, __VA_ARGS__)
#define HIGGS_EACH_55_(FN, X, ...) FN(X) HIGGS_EACH_54_(FN, __VA_ARGS__)
#define HIGGS_EACH_56_(FN, X, ...) FN(X) HIGGS_EACH_55_(FN, __VA_ARGS__)
#define HIGGS_EACH_57_(FN, X, ...) FN(X) HIGGS_EACH_56_(FN, __VA_ARGS__)
#define HIGGS_EACH_58_(FN, X, ...) FN(X) HIGGS_EACH_57_(FN, __VA_ARGS__)
#define HIGGS_EACH_59_(FN, X, ...) FN(X) HIGGS_EACH_58_(FN, __VA_ARGS__)

/** Apply the macro FN to each of the other arguments. */
#define HIGGS_EACH(FN, ...)                                                     \
    HIGGS_NTH_ARG_(, __VA_ARGS__,                                               \
                  HIGGS_EACH_59_,                                               \
                  HIGGS_EACH_58_,                                               \
                  HIGGS_EACH_57_,                                               \
                  HIGGS_EACH_56_,                                               \
                  HIGGS_EACH_55_,                                               \
                  HIGGS_EACH_54_,                                               \
                  HIGGS_EACH_53_,                                               \
                  HIGGS_EACH_52_,                                               \
                  HIGGS_EACH_51_,                                               \
                  HIGGS_EACH_50_,                                               \
                  HIGGS_EACH_49_,                                               \
                  HIGGS_EACH_48_,                                               \
                  HIGGS_EACH_47_,                                               \
                  HIGGS_EACH_46_,                                               \
                  HIGGS_EACH_45_,                                               \
                  HIGGS_EACH_44_,                                               \
                  HIGGS_EACH_43_,                                               \
                  HIGGS_EACH_42_,                                               \
                  HIGGS_EACH_41_,                                               \
                  HIGGS_EACH_40_,                                               \
                  HIGGS_EACH_39_,                                               \
                  HIGGS_EACH_38_,                                               \
                  HIGGS_EACH_37_,                                               \
                  HIGGS_EACH_36_,                                               \
                  HIGGS_EACH_35_,                                               \
                  HIGGS_EACH_34_,                                               \
                  HIGGS_EACH_33_,                                               \
                  HIGGS_EACH_32_,                                               \
                  HIGGS_EACH_31_,                                               \
                  HIGGS_EACH_30_,                                               \
                  HIGGS_EACH_29_,                                               \
                  HIGGS_EACH_28_,                                               \
                  HIGGS_EACH_27_,                                               \
                  HIGGS_EACH_26_,                                               \
                  HIGGS_EACH_25_,                                               \
                  HIGGS_EACH_24_,                                               \
                  HIGGS_EACH_23_,                                               \
                  HIGGS_EACH_22_,                                               \
                  HIGGS_EACH_21_,                                               \
                  HIGGS_EACH_20_,                                               \
                  HIGGS_EACH_19_,                                               \
                  HIGGS_EACH_18_,                                               \
                  HIGGS_EACH_17_,                                               \
                  HIGGS_EACH_16_,                                               \
                  HIGGS_EACH_15_,                                               \
                  HIGGS_EACH_14_,                                               \
                  HIGGS_EACH_13_,                                               \
                  HIGGS_EACH_12_,                                               \
                  HIGGS_EACH_11_,                                               \
                  HIGGS_EACH_10_,                                               \
                  HIGGS_EACH_09_,                                               \
                  HIGGS_EACH_08_,                                               \
                  HIGGS_EACH_07_,                                               \
                  HIGGS_EACH_06_,                                               \
                  HIGGS_EACH_05_,                                               \
                  HIGGS_EACH_04_,                                               \
                  HIGGS_EACH_03_,                                               \
                  HIGGS_EACH_02_,                                               \
                  HIGGS_EACH_01_,                                               \
                  HIGGS_EACH_00_)                                               \
    (FN, __VA_ARGS__)

/** Concatenate two tokens to form a new token. */
#define HIGGS_CONCAT_(a, b) a##b
#define HIGGS_CONCAT(a, b) HIGGS_CONCAT_(a, b)

/** Quote the argument, turning it into a string. */
#define HIGGS_TO_STRING(x) #x

/** @endcond */
#if HIGGS_CLANG || HIGGS_GCC
    #define HIGGS_IGNORE_GCC_IMPL_(compiler, warning)
    #define HIGGS_IGNORE_GCC_IMPL_0(compiler, warning)
    #define HIGGS_IGNORE_GCC_IMPL_1(compiler, warning)                          \
        _Pragma(HIGGS_TO_STRING(compiler diagnostic ignored warning))

    /** If 'warning' is recognised by this compiler, ignore it. */
    #if defined (__has_warning)
        #define HIGGS_IGNORE_GCC_LIKE(compiler, warning)                        \
            HIGGS_CONCAT(HIGGS_IGNORE_GCC_IMPL_, __has_warning(warning))(compiler, warning)
    #else
        #define HIGGS_IGNORE_GCC_LIKE(compiler, warning)                        \
            HIGGS_IGNORE_GCC_IMPL_1(compiler, warning)
    #endif

    /** Ignore GCC/clang-specific warnings. */
    #define HIGGS_IGNORE_GCC(warning)   HIGGS_IGNORE_GCC_LIKE(GCC, warning)
    #define HIGGS_IGNORE_clang(warning) HIGGS_IGNORE_GCC_LIKE(clang, warning)

    #define HIGGS_IGNORE_WARNINGS_GCC_LIKE(compiler, ...)                       \
        _Pragma(HIGGS_TO_STRING(compiler diagnostic push))                      \
        HIGGS_EACH(HIGGS_CONCAT(HIGGS_IGNORE_, compiler), __VA_ARGS__)

    /** Push a new warning scope, and then ignore each warning for either clang
        or gcc. If the compiler doesn't support __has_warning, we add -Wpragmas
        as the first disabled warning because otherwise we might get complaints
        about unknown warning options.
    */
    #if defined (__has_warning)
        #define HIGGS_PUSH_WARNINGS_GCC_LIKE(compiler, ...)                     \
            HIGGS_IGNORE_WARNINGS_GCC_LIKE(compiler, __VA_ARGS__)
    #else
        #define HIGGS_PUSH_WARNINGS_GCC_LIKE(compiler, ...)                     \
            HIGGS_IGNORE_WARNINGS_GCC_LIKE(compiler, "-Wpragmas", __VA_ARGS__)
    #endif

    /** Pop the current warning scope. */
    #define HIGGS_POP_WARNINGS_GCC_LIKE(compiler)                               \
        _Pragma(HIGGS_TO_STRING(compiler diagnostic pop))

    /** Push/pop warnings on compilers with gcc-like warning flags.
        These macros expand to nothing on other compilers (like MSVC).
    */
    #if HIGGS_CLANG
        #define HIGGS_BEGIN_IGNORE_WARNINGS_GCC_LIKE(...) HIGGS_PUSH_WARNINGS_GCC_LIKE(clang, __VA_ARGS__)
        #define HIGGS_END_IGNORE_WARNINGS_GCC_LIKE HIGGS_POP_WARNINGS_GCC_LIKE(clang)
    #else
        #define HIGGS_BEGIN_IGNORE_WARNINGS_GCC_LIKE(...) HIGGS_PUSH_WARNINGS_GCC_LIKE(GCC, __VA_ARGS__)
        #define HIGGS_END_IGNORE_WARNINGS_GCC_LIKE HIGGS_POP_WARNINGS_GCC_LIKE(GCC)
    #endif
#else
    #define HIGGS_BEGIN_IGNORE_WARNINGS_GCC_LIKE(...)
    #define HIGGS_END_IGNORE_WARNINGS_GCC_LIKE
#endif

/** Push/pop warnings on MSVC. These macros expand to nothing on other
    compilers (like clang and gcc).
*/
#if HIGGS_MSVC
    #define HIGGS_IGNORE_MSVC(warnings) __pragma(warning(disable:warnings))
    #define HIGGS_BEGIN_IGNORE_WARNINGS_LEVEL_MSVC(level, warnings)              \
        __pragma(warning(push, level)) HIGGS_IGNORE_MSVC(warnings)
    #define HIGGS_BEGIN_IGNORE_WARNINGS_MSVC(warnings)                           \
        __pragma(warning(push)) HIGGS_IGNORE_MSVC(warnings)
    #define HIGGS_END_IGNORE_WARNINGS_MSVC __pragma(warning(pop))
#else
    #define HIGGS_IGNORE_MSVC(warnings)
    #define HIGGS_BEGIN_IGNORE_WARNINGS_LEVEL_MSVC(level, warnings)
    #define HIGGS_BEGIN_IGNORE_WARNINGS_MSVC(warnings)
    #define HIGGS_END_IGNORE_WARNINGS_MSVC
#endif

#if HIGGS_MAC || HIGGS_IOS
    #define HIGGS_SANITIZER_ATTRIBUTE_MINIMUM_CLANG_VERSION 11
#else
    #define HIGGS_SANITIZER_ATTRIBUTE_MINIMUM_CLANG_VERSION 9
#endif

#define HIGGS_BEGIN_IGNORE_DEPRECATION_WARNINGS                                  \
    HIGGS_BEGIN_IGNORE_WARNINGS_GCC_LIKE ("-Wdeprecated-declarations")           \
    HIGGS_BEGIN_IGNORE_WARNINGS_MSVC (4996)

#define HIGGS_END_IGNORE_DEPRECATION_WARNINGS                                    \
    HIGGS_END_IGNORE_WARNINGS_MSVC                                               \
    HIGGS_END_IGNORE_WARNINGS_GCC_LIKE

/** Disable sanitizers for a range of functions.

    This functionality doesn't seem to exist on GCC yet, so at the moment this only works for clang.
*/
#if HIGGS_CLANG && __clang_major__ >= HIGGS_SANITIZER_ATTRIBUTE_MINIMUM_CLANG_VERSION
    #define HIGGS_BEGIN_NO_SANITIZE(warnings)                                    \
        _Pragma (HIGGS_TO_STRING (clang attribute push (__attribute__ ((no_sanitize (warnings))), apply_to=function)))
    #define HIGGS_END_NO_SANITIZE _Pragma (HIGGS_TO_STRING (clang attribute pop))
#else
    #define HIGGS_BEGIN_NO_SANITIZE(warnings)
    #define HIGGS_END_NO_SANITIZE
#endif

#undef HIGGS_SANITIZER_ATTRIBUTE_MINIMUM_CLANG_VERSION
