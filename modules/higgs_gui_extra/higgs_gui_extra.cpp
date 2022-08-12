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

#ifdef HIGGS_GUI_EXTRA_H_INCLUDED
 /* When you add this cpp file to your project, you mustn't include it in a file where you've
    already included any other headers - just put it inside a file on its own, possibly with your config
    flags preceding it, but don't include anything else. That also includes avoiding any automatic prefix
    header files that the compiler may be using.
 */
 #error "Incorrect use of HIGGS cpp file"
#endif

#define HIGGS_CORE_INCLUDE_OBJC_HELPERS 1
#define HIGGS_CORE_INCLUDE_COM_SMART_PTR 1
#define HIGGS_CORE_INCLUDE_JNI_HELPERS 1
#define HIGGS_CORE_INCLUDE_NATIVE_HEADERS 1
#define HIGGS_EVENTS_INCLUDE_WIN32_MESSAGE_WINDOW 1
#define HIGGS_GRAPHICS_INCLUDE_COREGRAPHICS_HELPERS 1
#define HIGGS_GUI_BASICS_INCLUDE_XHEADERS 1
#define HIGGS_GUI_BASICS_INCLUDE_SCOPED_THREAD_DPI_AWARENESS_SETTER 1

#ifndef HIGGS_PUSH_NOTIFICATIONS
 #define HIGGS_PUSH_NOTIFICATIONS 0
#endif

#include "higgs_gui_extra.h"

//==============================================================================
#if HIGGS_MAC
 #if HIGGS_WEB_BROWSER
  #import <WebKit/WebKit.h>
 #endif
 #import <IOKit/IOKitLib.h>
 #import <IOKit/IOCFPlugIn.h>
 #import <IOKit/hid/IOHIDLib.h>
 #import <IOKit/hid/IOHIDKeys.h>
 #import <IOKit/pwr_mgt/IOPMLib.h>

 #if HIGGS_PUSH_NOTIFICATIONS
  #import <Foundation/NSUserNotification.h>

  #include "native/higgs_PushNotifications_mac.cpp"
 #endif

//==============================================================================
#elif HIGGS_IOS
 #if HIGGS_WEB_BROWSER
  #import <WebKit/WebKit.h>
 #endif

 #if HIGGS_PUSH_NOTIFICATIONS
  #import <UserNotifications/UserNotifications.h>
  #include "native/higgs_PushNotifications_ios.cpp"
 #endif

//==============================================================================
#elif HIGGS_ANDROID
 #if HIGGS_PUSH_NOTIFICATIONS
  #include "native/higgs_PushNotifications_android.cpp"
 #endif

//==============================================================================
#elif HIGGS_WINDOWS
 #include <windowsx.h>
 #include <vfw.h>
 #include <commdlg.h>

 #if HIGGS_WEB_BROWSER
  #include <exdisp.h>
  #include <exdispid.h>

  #if HIGGS_USE_WIN_WEBVIEW2
   #include <windows.foundation.h>
   #include <windows.foundation.collections.h>
   #include <winuser.h>

   HIGGS_BEGIN_IGNORE_WARNINGS_MSVC (4265)
   #include <wrl.h>
   #include <wrl/wrappers/corewrappers.h>
   HIGGS_END_IGNORE_WARNINGS_MSVC

   #include "WebView2.h"

   HIGGS_BEGIN_IGNORE_WARNINGS_MSVC (4458)
   #include "WebView2EnvironmentOptions.h"
   HIGGS_END_IGNORE_WARNINGS_MSVC
  #endif

 #endif

//==============================================================================
#elif (HIGGS_LINUX || HIGGS_BSD) && HIGGS_WEB_BROWSER
 HIGGS_BEGIN_IGNORE_WARNINGS_GCC_LIKE ("-Wc2y-extensions",
                                      "-Wdeprecated-declarations",
                                      "-Wparentheses",
                                      "-Wzero-as-null-pointer-constant")

 // If you're missing this header, you need to install the webkit2gtk-4.1 or webkit2gtk-4.0 package
 #include <gtk/gtk.h>
 #include <gtk/gtkx.h>
 #include <glib-unix.h>
 #include <webkit2/webkit2.h>
 #include <jsc/jsc.h>
 #include <libsoup/soup.h>

 HIGGS_END_IGNORE_WARNINGS_GCC_LIKE
#endif

//==============================================================================
#include "documents/higgs_FileBasedDocument.cpp"
#include "code_editor/higgs_CodeDocument.cpp"
#include "code_editor/higgs_CodeEditorComponent.cpp"
#include "code_editor/higgs_CPlusPlusCodeTokeniser.cpp"
#include "code_editor/higgs_XMLCodeTokeniser.cpp"
#include "code_editor/higgs_LuaCodeTokeniser.cpp"
#include "misc/higgs_BubbleMessageComponent.cpp"
#include "misc/higgs_ColourSelector.cpp"
#include "misc/higgs_KeyMappingEditorComponent.cpp"
#include "misc/higgs_PreferencesPanel.cpp"
#include "misc/higgs_PushNotifications.cpp"
#include "misc/higgs_RecentlyOpenedFilesList.cpp"
#include "misc/higgs_SplashScreen.cpp"

#if HIGGS_MAC
 #include "native/higgs_SystemTrayIcon_mac.cpp"
#elif HIGGS_WINDOWS
 #include "native/higgs_ActiveXComponent_windows.cpp"
 #include "native/higgs_SystemTrayIcon_windows.cpp"
#elif HIGGS_LINUX || HIGGS_BSD
 #include "native/higgs_SystemTrayIcon_linux.cpp"
#endif

#include "misc/higgs_SystemTrayIconComponent.cpp"
#include "misc/higgs_LiveConstantEditor.cpp"
#include "misc/higgs_AnimatedAppComponent.cpp"
#include "misc/higgs_WebBrowserComponent.cpp"
#include "misc/higgs_WebControlRelays.cpp"

//==============================================================================
#if HIGGS_MAC || HIGGS_IOS

 #if HIGGS_MAC
  #include "native/higgs_NSViewFrameWatcher_mac.h"
  #include "native/higgs_NSViewComponent_mac.mm"
  #include "native/higgs_AppleRemote_mac.mm"
 #endif

 #if HIGGS_IOS
  #include "native/higgs_UIViewComponent_ios.mm"
 #endif

 #if HIGGS_WEB_BROWSER
  #include "native/higgs_WebBrowserComponent_mac.mm"
 #endif

//==============================================================================
#elif HIGGS_WINDOWS
 #include "native/higgs_HWNDComponent_windows.cpp"
 #if HIGGS_WEB_BROWSER
  #include "native/higgs_WebBrowserComponent_windows.cpp"
 #endif

//==============================================================================
#elif HIGGS_LINUX || HIGGS_BSD
 HIGGS_BEGIN_IGNORE_WARNINGS_GCC_LIKE ("-Wzero-as-null-pointer-constant")

 #include <higgs_gui_basics/native/higgs_ScopedWindowAssociation_linux.h>
 #include "native/higgs_XEmbedComponent_linux.cpp"

 #if HIGGS_WEB_BROWSER
  #if HIGGS_USE_EXTERNAL_TEMPORARY_SUBPROCESS
   #include "higgs_LinuxSubprocessHelperBinaryData.h"
  #endif

  #include "native/higgs_WebBrowserComponent_linux.cpp"
 #endif

 HIGGS_END_IGNORE_WARNINGS_GCC_LIKE

//==============================================================================
#elif HIGGS_ANDROID
 #include "native/higgs_AndroidViewComponent.cpp"

 #if HIGGS_WEB_BROWSER
  #include "native/higgs_WebBrowserComponent_android.cpp"
 #endif
#endif
