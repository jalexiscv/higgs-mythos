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


/*******************************************************************************
 The block below describes the properties of this module, and is read by
 the MythosStudio to automatically generate project code that uses it.
 For details about the syntax and how to create or use a module, see the
 Higgs Mythos Module Format.md file.


 BEGIN_HIGGS_MODULE_DECLARATION

  ID:                 higgs_core
  vendor:             higgs
  version:            9.0.5
  name:               Higgs Mythos core classes
  description:        The essential set of basic Higgs Mythos classes, as required by all the other Higgs Mythos modules. Includes text, container, memory, threading and i/o functionality.
  website:            https://github.com/jalexiscv/higgs-mythos
  license:            AGPLv3
  minimumCppStandard: 17

  dependencies:
  OSXFrameworks:      Cocoa Foundation IOKit Security
  iOSFrameworks:      Foundation
  linuxLibs:          rt dl pthread

 END_HIGGS_MODULE_DECLARATION

*******************************************************************************/


#pragma once
#define HIGGS_CORE_H_INCLUDED

//==============================================================================
#ifdef _MSC_VER
 #pragma warning (push)
 // Disable warnings for long class names, padding, and undefined preprocessor definitions.
 #pragma warning (disable: 4251 4786 4668 4820)
 #ifdef __INTEL_COMPILER
  #pragma warning (disable: 1125)
 #endif
#endif

#include "system/higgs_TargetPlatform.h"

//==============================================================================
/** Config: HIGGS_FORCE_DEBUG

    Normally, HIGGS_DEBUG is set to 1 or 0 based on compiler and project settings,
    but if you define this value, you can override this to force it to be true or false.
*/
#ifndef HIGGS_FORCE_DEBUG
 //#define HIGGS_FORCE_DEBUG 0
#endif

//==============================================================================
/** Config: HIGGS_LOG_ASSERTIONS

    If this flag is enabled, the jassert and jassertfalse macros will always use Logger::writeToLog()
    to write a message when an assertion happens.

    Enabling it will also leave this turned on in release builds. When it's disabled,
    however, the jassert and jassertfalse macros will not be compiled in a
    release build.

    @see jassert, jassertfalse, Logger
*/
#ifndef HIGGS_LOG_ASSERTIONS
 #if HIGGS_ANDROID
  #define HIGGS_LOG_ASSERTIONS 1
 #else
  #define HIGGS_LOG_ASSERTIONS 0
 #endif
#endif

//==============================================================================
/** Config: HIGGS_CHECK_MEMORY_LEAKS

    Enables a memory-leak check for certain objects when the app terminates. See the LeakedObjectDetector
    class and the HIGGS_LEAK_DETECTOR macro for more details about enabling leak checking for specific classes.
*/
#if HIGGS_DEBUG && ! defined (HIGGS_CHECK_MEMORY_LEAKS)
 #define HIGGS_CHECK_MEMORY_LEAKS 1
#endif

//==============================================================================
/** Config: HIGGS_DONT_AUTOLINK_TO_WIN32_LIBRARIES

    In a Windows build, this can be used to stop the required system libs being
    automatically added to the link stage.
*/
#ifndef HIGGS_DONT_AUTOLINK_TO_WIN32_LIBRARIES
 #define HIGGS_DONT_AUTOLINK_TO_WIN32_LIBRARIES 0
#endif

/** Config: HIGGS_INCLUDE_ZLIB_CODE
    This can be used to disable Higgs's embedded 3rd-party zlib code.
    You might need to tweak this if you're linking to an external zlib library in your app,
    but for normal apps, this option should be left alone.

    If you disable this, you might also want to set a value for HIGGS_ZLIB_INCLUDE_PATH, to
    specify the path where your zlib headers live.
*/
#ifndef HIGGS_INCLUDE_ZLIB_CODE
 #define HIGGS_INCLUDE_ZLIB_CODE 1
#endif

#ifndef HIGGS_ZLIB_INCLUDE_PATH
 #define HIGGS_ZLIB_INCLUDE_PATH <zlib.h>
#endif

/** Config: HIGGS_USE_CURL
    Enables http/https support via libcurl (Linux only). Enabling this will add an additional
    run-time dynamic dependency to libcurl.

    If you disable this then https/ssl support will not be available on Linux.
*/
#ifndef HIGGS_USE_CURL
 #define HIGGS_USE_CURL 1
#endif

/** Config: HIGGS_LOAD_CURL_SYMBOLS_LAZILY
    If enabled, HIGGS will load libcurl lazily when required (for example, when WebInputStream
    is used). Enabling this flag may also help with library dependency errors as linking
    libcurl at compile-time may instruct the linker to hard depend on a specific version
    of libcurl. It's also useful if you want to limit the amount of HIGGS dependencies and
    you are not using WebInputStream or the URL classes.
*/
#ifndef HIGGS_LOAD_CURL_SYMBOLS_LAZILY
 #define HIGGS_LOAD_CURL_SYMBOLS_LAZILY 0
#endif

/** Config: HIGGS_CATCH_UNHANDLED_EXCEPTIONS
    If enabled, this will add some exception-catching code to forward unhandled exceptions
    to your HIGGSApplicationBase::unhandledException() callback.
*/
#ifndef HIGGS_CATCH_UNHANDLED_EXCEPTIONS
 #define HIGGS_CATCH_UNHANDLED_EXCEPTIONS 0
#endif

/** Config: HIGGS_ALLOW_STATIC_NULL_VARIABLES
    If disabled, this will turn off dangerous static globals like String::empty, var::null, etc
    which can cause nasty order-of-initialisation problems if they are referenced during static
    constructor code.
*/
#ifndef HIGGS_ALLOW_STATIC_NULL_VARIABLES
 #define HIGGS_ALLOW_STATIC_NULL_VARIABLES 0
#endif

/** Config: HIGGS_STRICT_REFCOUNTEDPOINTER
    If enabled, this will make the ReferenceCountedObjectPtr class stricter about allowing
    itself to be cast directly to a raw pointer. By default this is disabled, for compatibility
    with old code, but if possible, you should always enable it to improve code safety!
*/
#ifndef HIGGS_STRICT_REFCOUNTEDPOINTER
 #define HIGGS_STRICT_REFCOUNTEDPOINTER 0
#endif

/** Config: HIGGS_ENABLE_ALLOCATION_HOOKS
    If enabled, this will add global allocation functions with built-in assertions, which may
    help when debugging allocations in unit tests.
*/
#ifndef HIGGS_ENABLE_ALLOCATION_HOOKS
 #define HIGGS_ENABLE_ALLOCATION_HOOKS 0
#endif

#ifndef HIGGS_STRING_UTF_TYPE
 #define HIGGS_STRING_UTF_TYPE 8
#endif

//==============================================================================
//==============================================================================

#if HIGGS_CORE_INCLUDE_NATIVE_HEADERS
 #include "native/higgs_BasicNativeHeaders.h"
#endif

#if HIGGS_WINDOWS
 #undef small
#endif

#include "system/higgs_StandardHeader.h"

namespace higgs
{
    class StringRef;
    class MemoryBlock;
    class File;
    class InputStream;
    class OutputStream;
    class DynamicObject;
    class FileInputStream;
    class FileOutputStream;
    class XmlElement;
    class NamedValue;

    extern HIGGS_API bool HIGGS_CALLTYPE higgs_isRunningUnderDebugger() noexcept;
    extern HIGGS_API void HIGGS_CALLTYPE logAssertion (const char* file, int line) noexcept;

    namespace detail
    {
        class QuickJSWrapper;
    }
}

#include "misc/higgs_EnumHelpers.h"
#include "misc/higgs_OrderedContainerHelpers.h"
#include "memory/higgs_Memory.h"
#include "misc/higgs_Functional.h"
#include "containers/higgs_Span.h"
#include "maths/higgs_MathsFunctions.h"
#include "memory/higgs_ByteOrder.h"
#include "memory/higgs_Atomic.h"
#include "text/higgs_CharacterFunctions.h"

HIGGS_BEGIN_IGNORE_WARNINGS_MSVC (4514 4996)

#include "text/higgs_CharPointer_UTF8.h"
#include "text/higgs_CharPointer_UTF16.h"
#include "text/higgs_CharPointer_UTF32.h"
#include "text/higgs_CharPointer_ASCII.h"

HIGGS_END_IGNORE_WARNINGS_MSVC

#include "text/higgs_String.h"
#include "text/higgs_StringRef.h"
#include "logging/higgs_Logger.h"
#include "memory/higgs_LeakedObjectDetector.h"
#include "memory/higgs_ContainerDeletePolicy.h"
#include "memory/higgs_HeapBlock.h"
#include "memory/higgs_MemoryBlock.h"
#include "memory/higgs_CopyableHeapBlock.h"
#include "memory/higgs_ReferenceCountedObject.h"
#include "memory/higgs_ScopedPointer.h"
#include "memory/higgs_OptionalScopedPointer.h"
#include "containers/higgs_Optional.h"
#include "containers/higgs_Enumerate.h"
#include "containers/higgs_ScopedValueSetter.h"
#include "memory/higgs_Singleton.h"
#include "memory/higgs_WeakReference.h"
#include "threads/higgs_ScopedLock.h"
#include "threads/higgs_CriticalSection.h"
#include "maths/higgs_Range.h"
#include "maths/higgs_NormalisableRange.h"
#include "maths/higgs_StatisticsAccumulator.h"
#include "containers/higgs_ElementComparator.h"
#include "containers/higgs_ArrayAllocationBase.h"
#include "containers/higgs_ArrayBase.h"
#include "containers/higgs_Array.h"
#include "containers/higgs_LinkedListPointer.h"
#include "misc/higgs_ScopeGuard.h"
#include "containers/higgs_ListenerList.h"
#include "containers/higgs_OwnedArray.h"
#include "containers/higgs_ReferenceCountedArray.h"
#include "containers/higgs_SortedSet.h"
#include "containers/higgs_SparseSet.h"
#include "containers/higgs_AbstractFifo.h"
#include "containers/higgs_SingleThreadedAbstractFifo.h"
#include "text/higgs_NewLine.h"
#include "text/higgs_StringPool.h"
#include "text/higgs_Identifier.h"
#include "text/higgs_StringArray.h"
#include "system/higgs_SystemStats.h"
#include "memory/higgs_HeavyweightLeakedObjectDetector.h"
#include "text/higgs_StringPairArray.h"
#include "text/higgs_TextDiff.h"
#include "text/higgs_LocalisedStrings.h"
#include "text/higgs_Base64.h"
#include "misc/higgs_Result.h"
#include "misc/higgs_Uuid.h"
#include "misc/higgs_ConsoleApplication.h"
#include "containers/higgs_Variant.h"
#include "containers/higgs_NamedValueSet.h"
#include "json/higgs_JSON.h"
#include "containers/higgs_DynamicObject.h"
#include "containers/higgs_HashMap.h"
#include "containers/higgs_FixedSizeFunction.h"
#include "time/higgs_TimeUnits.h"
#include "time/higgs_RelativeTime.h"
#include "time/higgs_Time.h"
#include "streams/higgs_InputStream.h"
#include "streams/higgs_OutputStream.h"
#include "streams/higgs_BufferedInputStream.h"
#include "streams/higgs_MemoryInputStream.h"
#include "streams/higgs_MemoryOutputStream.h"
#include "streams/higgs_SubregionStream.h"
#include "streams/higgs_InputSource.h"
#include "files/higgs_File.h"
#include "files/higgs_DirectoryIterator.h"
#include "files/higgs_RangedDirectoryIterator.h"
#include "detail/higgs_NativeFileHandle.h"
#include "files/higgs_FileInputStream.h"
#include "files/higgs_FileOutputStream.h"
#include "files/higgs_FileSearchPath.h"
#include "files/higgs_MemoryMappedFile.h"
#include "files/higgs_TemporaryFile.h"
#include "files/higgs_FileFilter.h"
#include "files/higgs_WildcardFileFilter.h"
#include "streams/higgs_FileInputSource.h"
#include "logging/higgs_FileLogger.h"
#include "json/higgs_JSONUtils.h"
#include "serialisation/higgs_Serialisation.h"
#include "json/higgs_JSONSerialisation.h"
#include "maths/higgs_BigInteger.h"
#include "maths/higgs_Expression.h"
#include "maths/higgs_Random.h"
#include "misc/higgs_RuntimePermissions.h"
#include "misc/higgs_WindowsRegistry.h"
#include "threads/higgs_ChildProcess.h"
#include "threads/higgs_DynamicLibrary.h"
#include "threads/higgs_InterProcessLock.h"
#include "threads/higgs_Process.h"
#include "threads/higgs_SpinLock.h"
#include "threads/higgs_WaitableEvent.h"
#include "threads/higgs_Thread.h"
#include "threads/higgs_HighResolutionTimer.h"
#include "threads/higgs_ThreadLocalValue.h"
#include "threads/higgs_ThreadPoolJob.h"
#include "detail/higgs_ThreadPoolLambdaJob.h"
#include "threads/higgs_ThreadPool.h"
#include "threads/higgs_TimeSliceThread.h"
#include "threads/higgs_ReadWriteLock.h"
#include "threads/higgs_ScopedReadLock.h"
#include "threads/higgs_ScopedWriteLock.h"
#include "network/higgs_IPAddress.h"
#include "network/higgs_MACAddress.h"
#include "network/higgs_NamedPipe.h"
#include "network/higgs_Socket.h"
#include "network/higgs_URL.h"
#include "network/higgs_WebInputStream.h"
#include "streams/higgs_URLInputSource.h"
#include "time/higgs_PerformanceCounter.h"
#include "time/higgs_TimedDiagnostic.h"
#include "unit_tests/higgs_UnitTest.h"
#include "xml/higgs_XmlDocument.h"
#include "xml/higgs_XmlElement.h"
#include "zip/higgs_GZIPCompressorOutputStream.h"
#include "zip/higgs_GZIPDecompressorInputStream.h"
#include "zip/higgs_ZipFile.h"
#include "containers/higgs_PropertySet.h"
#include "memory/higgs_SharedResourcePointer.h"
#include "memory/higgs_AllocationHooks.h"
#include "memory/higgs_Reservoir.h"
#include "files/higgs_AndroidDocument.h"
#include "streams/higgs_AndroidDocumentInputSource.h"
#include "misc/higgs_OptionsHelpers.h"

#include "detail/higgs_CallbackListenerList.h"
#include "detail/higgs_LruCache.h"
#include "detail/higgs_IncrementRef.h"

#if HIGGS_CORE_INCLUDE_OBJC_HELPERS && (HIGGS_MAC || HIGGS_IOS)
 #include "native/higgs_CFHelpers_mac.h"
 #include "native/higgs_ObjCHelpers_mac.h"
#endif

#if HIGGS_CORE_INCLUDE_COM_SMART_PTR && HIGGS_WINDOWS
 #include "native/higgs_ComSmartPtr_windows.h"
#endif

#if HIGGS_CORE_INCLUDE_JNI_HELPERS && HIGGS_ANDROID
 #include <jni.h>
 #include "native/higgs_JNIHelpers_android.h"
#endif

#if HIGGS_UNIT_TESTS
 #include "unit_tests/higgs_UnitTestCategories.h"
#endif

/** @cond */
namespace higgs
{
 /*
    As the very long class names here try to explain, the purpose of this code is to cause
    a linker error if not all of your compile units are consistent in the options that they
    enable before including HIGGS headers. The reason this is important is that if you have
    two cpp files, and one includes the higgs headers with debug enabled, and another does so
    without that, then each will be generating code with different class layouts, and you'll
    get subtle and hard-to-track-down memory corruption!
 */
 #if HIGGS_DEBUG
  struct HIGGS_API this_will_fail_to_link_if_some_of_your_compile_units_are_built_in_debug_mode
  { this_will_fail_to_link_if_some_of_your_compile_units_are_built_in_debug_mode() noexcept; };
  static this_will_fail_to_link_if_some_of_your_compile_units_are_built_in_debug_mode compileUnitMismatchSentinel;
 #else
  struct HIGGS_API this_will_fail_to_link_if_some_of_your_compile_units_are_built_in_release_mode
  { this_will_fail_to_link_if_some_of_your_compile_units_are_built_in_release_mode() noexcept; };
  static this_will_fail_to_link_if_some_of_your_compile_units_are_built_in_release_mode compileUnitMismatchSentinel;
 #endif
}
/** @endcond */

HIGGS_END_IGNORE_WARNINGS_MSVC

// In DLL builds, need to disable this warnings for other modules
#if defined (HIGGS_DLL_BUILD) || defined (HIGGS_DLL)
 HIGGS_IGNORE_MSVC (4251)
#endif
