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

namespace higgs
{

//==============================================================================
/*  This file defines miscellaneous macros for debugging, assertions, etc.
*/

//==============================================================================
#ifdef HIGGS_FORCE_DEBUG
 #undef HIGGS_DEBUG

 #if HIGGS_FORCE_DEBUG
  #define HIGGS_DEBUG 1
 #endif
#endif

/** This macro defines the C calling convention used as the standard for HIGGS calls. */
#if HIGGS_WINDOWS
 #define HIGGS_CALLTYPE   __stdcall
 #define HIGGS_CDECL      __cdecl
#else
 #define HIGGS_CALLTYPE
 #define HIGGS_CDECL
#endif

//==============================================================================
// Debugging and assertion macros

#ifndef HIGGS_LOG_CURRENT_ASSERTION
 #if HIGGS_LOG_ASSERTIONS || HIGGS_DEBUG
  #define HIGGS_LOG_CURRENT_ASSERTION    higgs::logAssertion (__FILE__, __LINE__);
 #else
  #define HIGGS_LOG_CURRENT_ASSERTION
 #endif
#endif

//==============================================================================
#if HIGGS_IOS || HIGGS_LINUX || HIGGS_BSD
  /** This will try to break into the debugger if the app is currently being debugged.
      If called by an app that's not being debugged, the behaviour isn't defined - it may
      crash or not, depending on the platform.
      @see jassert()
  */
  #define HIGGS_BREAK_IN_DEBUGGER        { ::kill (0, SIGTRAP); }
#elif HIGGS_MSVC
  #ifndef __INTEL_COMPILER
    #pragma intrinsic (__debugbreak)
  #endif
  #define HIGGS_BREAK_IN_DEBUGGER        { __debugbreak(); }
#elif HIGGS_INTEL && (HIGGS_GCC || HIGGS_CLANG || HIGGS_MAC)
  #if HIGGS_NO_INLINE_ASM
    #define HIGGS_BREAK_IN_DEBUGGER      { }
  #else
    #define HIGGS_BREAK_IN_DEBUGGER      { asm ("int $3"); }
  #endif
#elif HIGGS_ANDROID
  #define HIGGS_BREAK_IN_DEBUGGER        { __builtin_trap(); }
#elif HIGGS_ARM && HIGGS_CLANG
  #define HIGGS_BREAK_IN_DEBUGGER        { __builtin_debugtrap(); }
#else
  #define HIGGS_BREAK_IN_DEBUGGER        { __asm int 3 }
#endif

#if HIGGS_CLANG && defined (__has_feature) && ! defined (HIGGS_ANALYZER_NORETURN)
 #if __has_feature (attribute_analyzer_noreturn)
  inline void __attribute__ ((analyzer_noreturn)) higgs_assert_noreturn() {}
  #define HIGGS_ANALYZER_NORETURN higgs::higgs_assert_noreturn();
 #endif
#endif

#ifndef HIGGS_ANALYZER_NORETURN
 #define HIGGS_ANALYZER_NORETURN
#endif

/** Used to silence Wimplicit-fallthrough on Clang and GCC where available
    as there are a few places in the codebase where we need to do this
    deliberately and want to ignore the warning.
*/
#if HIGGS_CLANG
 #if __has_cpp_attribute(clang::fallthrough)
  #define HIGGS_FALLTHROUGH [[clang::fallthrough]];
 #else
  #define HIGGS_FALLTHROUGH
 #endif
#elif HIGGS_GCC
 #if __GNUC__ >= 7
  #define HIGGS_FALLTHROUGH [[gnu::fallthrough]];
 #else
  #define HIGGS_FALLTHROUGH
 #endif
#else
 #define HIGGS_FALLTHROUGH
#endif

//==============================================================================
#if HIGGS_MSVC
 /** @cond */
 #define HIGGS_BLOCK_WITH_FORCED_SEMICOLON(x) \
   __pragma(warning(push)) \
   __pragma(warning(disable:4127)) \
   do { x } while (false) \
   __pragma(warning(pop))
 /** @endcond */
#else
 /** This is the good old C++ trick for creating a macro that forces the user to put
    a semicolon after it when they use it.
 */
 #define HIGGS_BLOCK_WITH_FORCED_SEMICOLON(x) do { x } while (false)
#endif

//==============================================================================
#if (HIGGS_DEBUG && ! HIGGS_DISABLE_ASSERTIONS) || DOXYGEN
  /** Writes a string to the standard error stream.
      Note that as well as a single string, you can use this to write multiple items
      as a stream, e.g.
      @code
        DBG ("foo = " << foo << "bar = " << bar);
      @endcode
      The macro is only enabled in a debug build, so be careful not to use it with expressions
      that have important side-effects!
      @see Logger::outputDebugString
  */
  #define DBG(textToWrite)              HIGGS_BLOCK_WITH_FORCED_SEMICOLON (higgs::String tempDbgBuf; tempDbgBuf << textToWrite; higgs::Logger::outputDebugString (tempDbgBuf);)

  //==============================================================================
  /** This will always cause an assertion failure.
      It is only compiled in a debug build, (unless HIGGS_LOG_ASSERTIONS is enabled for your build).
      @see jassert
      @hiderefby
  */
  #define jassertfalse                  HIGGS_BLOCK_WITH_FORCED_SEMICOLON (HIGGS_LOG_CURRENT_ASSERTION; if (higgs::higgs_isRunningUnderDebugger()) HIGGS_BREAK_IN_DEBUGGER; HIGGS_ANALYZER_NORETURN)

  //==============================================================================
  /** Platform-independent assertion macro.

      This macro gets turned into a no-op when you're building with debugging turned off, so be
      careful that the expression you pass to it doesn't perform any actions that are vital for the
      correct behaviour of your program!
      @see jassertfalse
      @hiderefby
  */
  #define jassert(expression)           HIGGS_BLOCK_WITH_FORCED_SEMICOLON (if (! (expression)) jassertfalse;)

  /** Platform-independent assertion macro which suppresses ignored-variable
      warnings in all build modes. You should probably use a plain jassert()
      and [[maybe_unused]] by default.
  */
  #define jassertquiet(expression)      HIGGS_BLOCK_WITH_FORCED_SEMICOLON (if (! (expression)) jassertfalse;)

  #define HIGGS_ASSERTIONS_ENABLED       1

#else
  //==============================================================================
  // If debugging is disabled, these dummy debug and assertion macros are used.

  #define DBG(textToWrite)
  #define jassertfalse                  HIGGS_BLOCK_WITH_FORCED_SEMICOLON (HIGGS_LOG_CURRENT_ASSERTION;)
  #define HIGGS_ASSERTIONS_ENABLED       0

  #if HIGGS_LOG_ASSERTIONS
   #define jassert(expression)          HIGGS_BLOCK_WITH_FORCED_SEMICOLON (if (! (expression)) jassertfalse;)
   #define jassertquiet(expression)     HIGGS_BLOCK_WITH_FORCED_SEMICOLON (if (! (expression)) jassertfalse;)
  #else
   #define jassert(expression)          HIGGS_BLOCK_WITH_FORCED_SEMICOLON ( ; )
   #define jassertquiet(expression)     HIGGS_BLOCK_WITH_FORCED_SEMICOLON (if (false) (void) (expression);)
  #endif

#endif

#define HIGGS_ASSERTIONS_ENABLED_OR_LOGGED   HIGGS_ASSERTIONS_ENABLED || HIGGS_LOG_ASSERTIONS

//==============================================================================
/** @cond */
#define HIGGS_JOIN_MACRO_HELPER(a, b) a ## b
#define HIGGS_STRINGIFY_MACRO_HELPER(a) #a
/** @endcond */

/** A good old-fashioned C macro concatenation helper.
    This combines two items (which may themselves be macros) into a single string,
    avoiding the pitfalls of the ## macro operator.
*/
#define HIGGS_JOIN_MACRO(item1, item2)  HIGGS_JOIN_MACRO_HELPER (item1, item2)

/** A handy C macro for stringifying any symbol, rather than just a macro parameter. */
#define HIGGS_STRINGIFY(item)  HIGGS_STRINGIFY_MACRO_HELPER (item)

//==============================================================================
/** This is a shorthand macro for deleting a class's copy constructor and
    copy assignment operator.

    For example, instead of
    @code
    class MyClass
    {
        etc.

    private:
        MyClass (const MyClass&);
        MyClass& operator= (const MyClass&);
    };@endcode

    ...you can just write:

    @code
    class MyClass
    {
        etc.

    private:
        HIGGS_DECLARE_NON_COPYABLE (MyClass)
    };@endcode
*/
#define HIGGS_DECLARE_NON_COPYABLE(className) \
    className (const className&) = delete;\
    className& operator= (const className&) = delete;

/** This is a shorthand macro for deleting a class's move constructor and
    move assignment operator.
*/
#define HIGGS_DECLARE_NON_MOVEABLE(className) \
    className (className&&) = delete;\
    className& operator= (className&&) = delete;

/** This is a shorthand way of writing both a HIGGS_DECLARE_NON_COPYABLE and
    HIGGS_LEAK_DETECTOR macro for a class.
*/
#define HIGGS_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(className) \
    HIGGS_DECLARE_NON_COPYABLE(className) \
    HIGGS_LEAK_DETECTOR(className)

/** This macro can be added to class definitions to disable the use of new/delete to
    allocate the object on the heap, forcing it to only be used as a stack or member variable.
*/
#define HIGGS_PREVENT_HEAP_ALLOCATION \
   private: \
    static void* operator new (size_t) = delete; \
    static void operator delete (void*) = delete;

//==============================================================================
#if HIGGS_MSVC
 /** @cond */
 #define HIGGS_COMPILER_WARNING(msg) __pragma (message (__FILE__ "(" HIGGS_STRINGIFY (__LINE__) ") : Warning: " msg))
 /** @endcond */
#else

 /** This macro allows you to emit a custom compiler warning message.
     Very handy for marking bits of code as "to-do" items, or for shaming
     code written by your co-workers in a way that's hard to ignore.

     GCC and Clang provide the \#warning directive, but MSVC doesn't, so this
     macro is a cross-compiler way to get the same functionality as \#warning.

     Unlike the \#warning directive in GCC and Clang this macro requires the
     argument to passed as a quoted string.
 */
 #define HIGGS_COMPILER_WARNING(msg)  _Pragma (HIGGS_STRINGIFY (message (msg)))
#endif


//==============================================================================
#if HIGGS_DEBUG || DOXYGEN
  /** A platform-independent way of forcing an inline function.
      Use the syntax: @code
      forcedinline void myfunction (int x)
      @endcode
  */
  #define forcedinline  inline
#else
  #if HIGGS_MSVC
   #define forcedinline       __forceinline
  #else
   #define forcedinline       inline __attribute__ ((always_inline))
  #endif
#endif

#if HIGGS_MSVC || DOXYGEN
  /** This can be placed before a stack or member variable declaration to tell the compiler
      to align it to the specified number of bytes. */
  #define HIGGS_ALIGN(bytes)   __declspec (align (bytes))
#else
  #define HIGGS_ALIGN(bytes)   __attribute__ ((aligned (bytes)))
#endif

//==============================================================================
#if HIGGS_ANDROID
 /** @cond */
 #define HIGGS_MODAL_LOOPS_PERMITTED 0
 /** @endcond */
#elif ! defined (HIGGS_MODAL_LOOPS_PERMITTED)
 /** Some operating environments don't provide a modal loop mechanism, so this flag can be
     used to disable any functions that try to run a modal loop. */
 #define HIGGS_MODAL_LOOPS_PERMITTED 0
#endif

//==============================================================================
/** @cond */
#if HIGGS_GCC || HIGGS_CLANG
 #define HIGGS_PACKED __attribute__ ((packed))
#else
 #define HIGGS_PACKED
#endif
/** @endcond */

//==============================================================================
#if HIGGS_GCC || DOXYGEN
 /** This can be appended to a function declaration to tell gcc to disable associative
     math optimisations which break some floating point algorithms. */
 #define HIGGS_NO_ASSOCIATIVE_MATH_OPTIMISATIONS   __attribute__ ((__optimize__ ("no-associative-math")))
#else
 #define HIGGS_NO_ASSOCIATIVE_MATH_OPTIMISATIONS
#endif

} // namespace higgs
