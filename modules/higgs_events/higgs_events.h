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

  ID:                 higgs_events
  vendor:             higgs
  version:            9.0.5
  name:               Higgs Mythos message and event handling classes
  description:        Classes for running an application's main event loop and sending/receiving messages, timers, etc.
  website:            https://github.com/jalexiscv/higgs-mythos
  license:            AGPLv3
  minimumCppStandard: 17

  dependencies:       higgs_core

 END_HIGGS_MODULE_DECLARATION

*******************************************************************************/


#pragma once
#define HIGGS_EVENTS_H_INCLUDED

#include <higgs_core/higgs_core.h>

//==============================================================================
/** Config: HIGGS_EXECUTE_APP_SUSPEND_ON_BACKGROUND_TASK
    Will execute your application's suspend method on an iOS background task, giving
    you extra time to save your applications state.
*/
#ifndef HIGGS_EXECUTE_APP_SUSPEND_ON_BACKGROUND_TASK
 #define HIGGS_EXECUTE_APP_SUSPEND_ON_BACKGROUND_TASK 0
#endif

#if HIGGS_WINDOWS && HIGGS_EVENTS_INCLUDE_WINRT_WRAPPER
 // If this header file is missing then you are probably attempting to use WinRT
 // functionality without the WinRT libraries installed on your system. Try installing
 // the latest Windows Standalone SDK and maybe also adding the path to the WinRT
 // headers to your build system. This path should have the form
 // "C:\Program Files (x86)\Windows Kits\10\Include\10.0.14393.0\winrt".
 #include <inspectable.h>
 #include <hstring.h>
#endif

#include "messages/higgs_MessageManager.h"
#include "messages/higgs_Message.h"
#include "messages/higgs_MessageListener.h"
#include "messages/higgs_CallbackMessage.h"
#include "messages/higgs_DeletedAtShutdown.h"
#include "messages/higgs_NotificationType.h"
#include "messages/higgs_ApplicationBase.h"
#include "messages/higgs_Initialisation.h"
#include "messages/higgs_MountedVolumeListChangeDetector.h"
#include "broadcasters/higgs_ActionBroadcaster.h"
#include "broadcasters/higgs_ActionListener.h"
#include "broadcasters/higgs_AsyncUpdater.h"
#include "broadcasters/higgs_LockingAsyncUpdater.h"
#include "broadcasters/higgs_ChangeListener.h"
#include "broadcasters/higgs_ChangeBroadcaster.h"
#include "timers/higgs_Timer.h"
#include "timers/higgs_TimedCallback.h"
#include "timers/higgs_MultiTimer.h"
#include "interprocess/higgs_ChildProcessManager.h"
#include "interprocess/higgs_InterprocessConnection.h"
#include "interprocess/higgs_InterprocessConnectionServer.h"
#include "interprocess/higgs_ConnectedChildProcess.h"
#include "interprocess/higgs_NetworkServiceDiscovery.h"
#include "native/higgs_ScopedLowPowerModeDisabler.h"

#if HIGGS_LINUX || HIGGS_BSD
 #include "native/higgs_EventLoop_linux.h"
#endif

#if HIGGS_WINDOWS
 #if HIGGS_EVENTS_INCLUDE_WIN32_MESSAGE_WINDOW
  #include "native/higgs_HiddenMessageWindow_windows.h"
 #endif
 #if HIGGS_EVENTS_INCLUDE_WINRT_WRAPPER
  #include "native/higgs_WinRTWrapper_windows.h"
 #endif
#endif
