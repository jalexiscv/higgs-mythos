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

#ifdef HIGGS_EVENTS_H_INCLUDED
 /* When you add this cpp file to your project, you mustn't include it in a file where you've
    already included any other headers - just put it inside a file on its own, possibly with your config
    flags preceding it, but don't include anything else. That also includes avoiding any automatic prefix
    header files that the compiler may be using.
 */
 #error "Incorrect use of HIGGS cpp file"
#endif

#define HIGGS_CORE_INCLUDE_OBJC_HELPERS 1
#define HIGGS_CORE_INCLUDE_JNI_HELPERS 1
#define HIGGS_CORE_INCLUDE_NATIVE_HEADERS 1
#define HIGGS_CORE_INCLUDE_COM_SMART_PTR 1
#define HIGGS_EVENTS_INCLUDE_WIN32_MESSAGE_WINDOW 1

#if HIGGS_USE_WINRT_MIDI
 #define HIGGS_EVENTS_INCLUDE_WINRT_WRAPPER 1
#endif

#include "higgs_events.h"

//==============================================================================
#if HIGGS_MAC
 #import <IOKit/IOKitLib.h>
 #import <IOKit/IOCFPlugIn.h>
 #import <IOKit/hid/IOHIDLib.h>
 #import <IOKit/hid/IOHIDKeys.h>
 #import <IOKit/pwr_mgt/IOPMLib.h>

#elif HIGGS_LINUX || HIGGS_BSD
 #include <unistd.h>
#elif HIGGS_WINDOWS
 #include <shellscalingapi.h>

 #if ! HIGGS_DONT_AUTOLINK_TO_WIN32_LIBRARIES
  #pragma comment(lib, "shcore.lib")
 #endif
#endif

//==============================================================================
#include "messages/higgs_ApplicationBase.cpp"
#include "messages/higgs_DeletedAtShutdown.cpp"
#include "messages/higgs_MessageListener.cpp"
#include "messages/higgs_MessageManager.cpp"
#include "broadcasters/higgs_ActionBroadcaster.cpp"
#include "broadcasters/higgs_AsyncUpdater.cpp"
#include "broadcasters/higgs_LockingAsyncUpdater.cpp"
#include "broadcasters/higgs_ChangeBroadcaster.cpp"
#include "timers/higgs_MultiTimer.cpp"
#include "timers/higgs_Timer.cpp"
#include "interprocess/higgs_ChildProcessManager.cpp"
#include "interprocess/higgs_InterprocessConnection.cpp"
#include "interprocess/higgs_InterprocessConnectionServer.cpp"
#include "interprocess/higgs_ConnectedChildProcess.cpp"
#include "interprocess/higgs_NetworkServiceDiscovery.cpp"
#include "native/higgs_ScopedLowPowerModeDisabler.cpp"

//==============================================================================
#if HIGGS_MAC || HIGGS_IOS

 #include "native/higgs_MessageQueue_mac.h"

 #if HIGGS_MAC
  #include "native/higgs_MessageManager_mac.mm"
 #else
  #include "native/higgs_MessageManager_ios.mm"
 #endif

#elif HIGGS_WINDOWS
 #include "native/higgs_RunningInUnity.h"
 #include "native/higgs_Messaging_windows.cpp"
 #include "native/higgs_HiddenMessageWindow_windows.cpp"
 #if HIGGS_EVENTS_INCLUDE_WINRT_WRAPPER
  #include "native/higgs_WinRTWrapper_windows.cpp"
 #endif

#elif HIGGS_LINUX || HIGGS_BSD
 #include "native/higgs_EventLoopInternal_linux.h"
 #include "native/higgs_Messaging_linux.cpp"

#elif HIGGS_ANDROID
 #include "native/higgs_Messaging_android.cpp"

#endif
