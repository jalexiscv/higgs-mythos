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

#ifdef HIGGS_CORE_H_INCLUDED
 /* When you add this cpp file to your project, you mustn't include it in a file where you've
    already included any other headers - just put it inside a file on its own, possibly with your config
    flags preceding it, but don't include anything else. That also includes avoiding any automatic prefix
    header files that the compiler may be using.
 */
 #error "Incorrect use of HIGGS cpp file"
#endif

#define HIGGS_CORE_INCLUDE_OBJC_HELPERS 1
#define HIGGS_CORE_INCLUDE_COM_SMART_PTR 1
#define HIGGS_CORE_INCLUDE_NATIVE_HEADERS 1
#define HIGGS_CORE_INCLUDE_JNI_HELPERS 1

#include "higgs_core.h"

#include <cctype>
#include <cstdarg>
#include <locale>
#include <thread>

#if ! (HIGGS_ANDROID || HIGGS_BSD)
 #include <sys/timeb.h>
 #include <cwctype>
#endif

#if HIGGS_WINDOWS
 #include <ctime>

 HIGGS_BEGIN_IGNORE_WARNINGS_MSVC (4091)
 #include <dbghelp.h>
 HIGGS_END_IGNORE_WARNINGS_MSVC

 #if ! HIGGS_DONT_AUTOLINK_TO_WIN32_LIBRARIES
  #pragma comment (lib, "DbgHelp.lib")
 #endif

#else
 #if HIGGS_LINUX || HIGGS_BSD || HIGGS_ANDROID
  #include <sys/types.h>
  #include <sys/socket.h>

  #if __has_include (<errno.h>)
   #include <errno.h>
  #else
   #include <sys/errno.h>
  #endif

  #include <unistd.h>
  #include <netinet/in.h>
 #endif

 #if HIGGS_WASM
  #include <stdio.h>
  #include <sys/types.h>
  #include <sys/socket.h>
  #include <errno.h>
  #include <unistd.h>
  #include <netinet/in.h>
  #include <sys/stat.h>
 #endif

 #if HIGGS_LINUX || HIGGS_BSD
  #include <stdio.h>
  #include <langinfo.h>
  #include <ifaddrs.h>
  #include <sys/resource.h>
  #include <sys/utsname.h>

  #if HIGGS_USE_CURL
   #include <curl/curl.h>
  #endif
 #endif

 #include <pwd.h>
 #include <fcntl.h>
 #include <netdb.h>
 #include <arpa/inet.h>
 #include <netinet/tcp.h>
 #include <sys/time.h>
 #include <net/if.h>
 #include <sys/ioctl.h>

 #if ! (HIGGS_ANDROID || HIGGS_WASM) && __has_include (<execinfo.h>)
  #include <execinfo.h>
 #endif
#endif

#if HIGGS_MAC || HIGGS_IOS
 #include <xlocale.h>
 #include <mach/mach.h>
#endif

#if HIGGS_ANDROID
 #include <ifaddrs.h>
 #include <android/log.h>
#endif

#undef check

//==============================================================================
#include "containers/higgs_AbstractFifo.cpp"
#include "containers/higgs_ArrayBase.cpp"
#include "containers/higgs_NamedValueSet.cpp"
#include "containers/higgs_OwnedArray.cpp"
#include "containers/higgs_PropertySet.cpp"
#include "containers/higgs_ReferenceCountedArray.cpp"
#include "containers/higgs_SparseSet.cpp"
#include "files/higgs_DirectoryIterator.cpp"
#include "files/higgs_RangedDirectoryIterator.cpp"
#include "files/higgs_File.cpp"
#include "files/higgs_FileInputStream.cpp"
#include "files/higgs_FileOutputStream.cpp"
#include "files/higgs_FileSearchPath.cpp"
#include "files/higgs_TemporaryFile.cpp"
#include "logging/higgs_FileLogger.cpp"
#include "logging/higgs_Logger.cpp"
#include "maths/higgs_BigInteger.cpp"
#include "maths/higgs_Expression.cpp"
#include "maths/higgs_Random.cpp"
#include "memory/higgs_MemoryBlock.cpp"
#include "memory/higgs_AllocationHooks.cpp"
#include "misc/higgs_RuntimePermissions.cpp"
#include "misc/higgs_Result.cpp"
#include "misc/higgs_Uuid.cpp"
#include "misc/higgs_ConsoleApplication.cpp"
#include "misc/higgs_ScopeGuard.cpp"
#include "network/higgs_MACAddress.cpp"

#if ! HIGGS_WINDOWS
 #include "native/higgs_SharedCode_posix.h"
 #include "native/higgs_NamedPipe_posix.cpp"
#else
 #include "native/higgs_Files_windows.cpp"
#endif

#include "zip/higgs_zlib.h"
#include "network/higgs_NamedPipe.cpp"
#include "network/higgs_Socket.cpp"
#include "network/higgs_IPAddress.cpp"
#include "streams/higgs_BufferedInputStream.cpp"
#include "streams/higgs_FileInputSource.cpp"
#include "streams/higgs_InputStream.cpp"
#include "streams/higgs_MemoryInputStream.cpp"
#include "streams/higgs_MemoryOutputStream.cpp"
#include "streams/higgs_SubregionStream.cpp"
#include "system/higgs_SystemStats.cpp"
#include "text/higgs_CharacterFunctions.cpp"
#include "text/higgs_Identifier.cpp"
#include "text/higgs_LocalisedStrings.cpp"
#include "text/higgs_String.cpp"
#include "streams/higgs_OutputStream.cpp"
#include "text/higgs_StringArray.cpp"
#include "text/higgs_StringPairArray.cpp"
#include "text/higgs_StringPool.cpp"
#include "text/higgs_TextDiff.cpp"
#include "text/higgs_Base64.cpp"
#include "threads/higgs_ReadWriteLock.cpp"
#include "threads/higgs_Thread.cpp"
#include "threads/higgs_ThreadPool.cpp"
#include "threads/higgs_ThreadPoolJob.cpp"
#include "threads/higgs_TimeSliceThread.cpp"
#include "time/higgs_PerformanceCounter.cpp"
#include "time/higgs_RelativeTime.cpp"
#include "time/higgs_Time.cpp"
#include "unit_tests/higgs_UnitTest.cpp"
#include "containers/higgs_Variant.cpp"
#include "json/higgs_JSON.cpp"
#include "json/higgs_JSONUtils.cpp"
#include "containers/higgs_DynamicObject.cpp"
#include "xml/higgs_XmlDocument.cpp"
#include "xml/higgs_XmlElement.cpp"
#include "zip/higgs_GZIPDecompressorInputStream.cpp"
#include "zip/higgs_GZIPCompressorOutputStream.cpp"
#include "zip/higgs_ZipFile.cpp"
#include "files/higgs_FileFilter.cpp"
#include "files/higgs_WildcardFileFilter.cpp"
#include "native/higgs_ThreadPriorities_native.h"
#include "native/higgs_PlatformTimerListener.h"

//==============================================================================
#if ! HIGGS_WINDOWS && (! HIGGS_ANDROID || __ANDROID_API__ >= 24)
 #include "native/higgs_IPAddress_posix.h"
#endif

//==============================================================================
#if HIGGS_MAC || HIGGS_IOS
 #include "native/higgs_Files_mac.mm"
 #include "native/higgs_Network_mac.mm"
 #include "native/higgs_Strings_mac.mm"
 #include "native/higgs_SharedCode_intel.h"
 #include "native/higgs_SystemStats_mac.mm"
 #include "native/higgs_Threads_mac.mm"
 #include "native/higgs_PlatformTimer_generic.cpp"
 #include "native/higgs_Process_mac.mm"

//==============================================================================
#elif HIGGS_WINDOWS
 #include "native/higgs_Network_windows.cpp"
 #include "native/higgs_Registry_windows.cpp"
 #include "native/higgs_SystemStats_windows.cpp"
 #include "native/higgs_Threads_windows.cpp"
 #include "native/higgs_PlatformTimer_generic.cpp"
 #include "native/higgs_PlatformTimer_windows.cpp"

//==============================================================================
#elif HIGGS_LINUX
 #include "native/higgs_CommonFile_linux.cpp"
 #include "native/higgs_Files_linux.cpp"
 #include "native/higgs_Network_linux.cpp"
 #if HIGGS_USE_CURL
  #include "native/higgs_Network_curl.cpp"
 #endif
 #include "native/higgs_SystemStats_linux.cpp"
 #include "native/higgs_Threads_linux.cpp"
 #include "native/higgs_PlatformTimer_generic.cpp"

//==============================================================================
#elif HIGGS_BSD
 #include "native/higgs_CommonFile_linux.cpp"
 #include "native/higgs_Files_linux.cpp"
 #include "native/higgs_Network_linux.cpp"
 #if HIGGS_USE_CURL
  #include "native/higgs_Network_curl.cpp"
 #endif
 #include "native/higgs_SharedCode_intel.h"
 #include "native/higgs_SystemStats_linux.cpp"
 #include "native/higgs_Threads_linux.cpp"
 #include "native/higgs_PlatformTimer_generic.cpp"

//==============================================================================
#elif HIGGS_ANDROID
 #include "native/higgs_CommonFile_linux.cpp"
 #include "native/higgs_JNIHelpers_android.cpp"
 #include "native/higgs_Files_android.cpp"
 #include "native/higgs_Misc_android.cpp"
 #include "native/higgs_Network_android.cpp"
 #include "native/higgs_SystemStats_android.cpp"
 #include "native/higgs_Threads_android.cpp"
 #include "native/higgs_RuntimePermissions_android.cpp"
 #include "native/higgs_PlatformTimer_generic.cpp"

//==============================================================================
#elif HIGGS_WASM
 #include "native/higgs_SystemStats_wasm.cpp"
 #include "native/higgs_PlatformTimer_generic.cpp"
#endif

#include "files/higgs_common_MimeTypes.h"
#include "files/higgs_common_MimeTypes.cpp"
#include "native/higgs_AndroidDocument_android.cpp"
#include "threads/higgs_HighResolutionTimer.cpp"
#include "threads/higgs_WaitableEvent.cpp"
#include "network/higgs_URL.cpp"

#if ! HIGGS_WASM
 #include "threads/higgs_ChildProcess.cpp"
 #include "network/higgs_WebInputStream.cpp"
 #include "streams/higgs_URLInputSource.cpp"
#endif

//==============================================================================
#if HIGGS_UNIT_TESTS
 #include "containers/higgs_HashMap_test.cpp"
 #include "containers/higgs_Optional_test.cpp"
 #include "containers/higgs_Enumerate_test.cpp"
 #include "containers/higgs_ListenerList_test.cpp"
 #include "maths/higgs_MathsFunctions_test.cpp"
 #include "misc/higgs_EnumHelpers_test.cpp"
 #include "misc/higgs_OrderedContainerHelpers_test.cpp"
 #include "containers/higgs_FixedSizeFunction_test.cpp"
 #include "json/higgs_JSONSerialisation_test.cpp"
 #include "memory/higgs_SharedResourcePointer_test.cpp"
 #include "text/higgs_CharPointer_UTF8_test.cpp"
 #include "text/higgs_CharPointer_UTF16_test.cpp"
 #include "text/higgs_CharPointer_UTF32_test.cpp"
 #if HIGGS_MAC || HIGGS_IOS
  #include "native/higgs_ObjCHelpers_mac_test.mm"
 #endif
#endif

//==============================================================================
namespace higgs
{
/*
    As the very long class names here try to explain, the purpose of this code is to cause
    a linker error if not all of your compile units are consistent in the options that they
    enable before including HIGGS headers. The reason this is important is that if you have
    two cpp files, and one includes the higgs headers with debug enabled, and the other doesn't,
    then each will be generating code with different memory layouts for the classes, and
    you'll get subtle and hard-to-track-down memory corruption bugs!
*/
#if HIGGS_DEBUG
 this_will_fail_to_link_if_some_of_your_compile_units_are_built_in_debug_mode
    ::this_will_fail_to_link_if_some_of_your_compile_units_are_built_in_debug_mode() noexcept {}
#else
 this_will_fail_to_link_if_some_of_your_compile_units_are_built_in_release_mode
    ::this_will_fail_to_link_if_some_of_your_compile_units_are_built_in_release_mode() noexcept {}
#endif
}
