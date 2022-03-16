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

#ifdef HIGGS_GUI_BASICS_H_INCLUDED
 /* When you add this cpp file to your project, you mustn't include it in a file where you've
    already included any other headers - just put it inside a file on its own, possibly with your config
    flags preceding it, but don't include anything else. That also includes avoiding any automatic prefix
    header files that the compiler may be using.
 */
 #error "Incorrect use of HIGGS cpp file"
#endif

#define NS_FORMAT_FUNCTION(F,A) // To avoid spurious warnings from GCC

#define HIGGS_CORE_INCLUDE_OBJC_HELPERS 1
#define HIGGS_CORE_INCLUDE_COM_SMART_PTR 1
#define HIGGS_CORE_INCLUDE_JNI_HELPERS 1
#define HIGGS_CORE_INCLUDE_NATIVE_HEADERS 1
#define HIGGS_EVENTS_INCLUDE_WIN32_MESSAGE_WINDOW 1
#define HIGGS_GRAPHICS_INCLUDE_COREGRAPHICS_HELPERS 1
#define HIGGS_GUI_BASICS_INCLUDE_XHEADERS 1
#define HIGGS_GUI_BASICS_INCLUDE_SCOPED_THREAD_DPI_AWARENESS_SETTER 1

#include "higgs_gui_basics.h"

#include <cctype>

//==============================================================================
#ifdef HIGGS_DISPLAY_SPLASH_SCREEN
 HIGGS_COMPILER_WARNING ("This version of HIGGS does not use the splash screen, the flag HIGGS_DISPLAY_SPLASH_SCREEN is ignored")
#endif

#ifdef HIGGS_USE_DARK_SPLASH_SCREEN
 HIGGS_COMPILER_WARNING ("This version of HIGGS does not use the splash screen, the flag HIGGS_USE_DARK_SPLASH_SCREEN is ignored")
#endif

//==============================================================================
#if HIGGS_MAC
 #import <IOKit/pwr_mgt/IOPMLib.h>
 #import <MetalKit/MetalKit.h>

 #if HIGGS_MAC_API_VERSION_MIN_REQUIRED_AT_LEAST (14, 4)
  #import <ScreenCaptureKit/ScreenCaptureKit.h>
 #endif

#elif HIGGS_IOS
 #import <UserNotifications/UserNotifications.h>
 #import <UniformTypeIdentifiers/UniformTypeIdentifiers.h>
 #import <MetalKit/MetalKit.h>
 #import <UIKit/UIActivityViewController.h>

//==============================================================================
#elif HIGGS_WINDOWS
 #include <commctrl.h>
 #include <commdlg.h>
 #include <d2d1_3.h>
 #include <d3d11_2.h>
 #include <dxgi1_3.h>
 #include <sapi.h>
 #include <vfw.h>
 #include <windowsx.h>
 #include <dwmapi.h>
 #include <dwrite_3.h>
 #include <dcomp.h>
 #include <shellscalingapi.h>

 #if HIGGS_ETW_TRACELOGGING
  #include <TraceLoggingProvider.h>
  #include <evntrace.h>
 #endif

 #include <uiautomation.h>

 #undef new

 #if HIGGS_WEB_BROWSER
  #include <exdisp.h>
  #include <exdispid.h>
 #endif

 #if ! HIGGS_DONT_AUTOLINK_TO_WIN32_LIBRARIES
  #pragma comment(lib, "vfw32.lib")
  #pragma comment(lib, "imm32.lib")
  #pragma comment(lib, "comctl32.lib")
  #pragma comment(lib, "dwmapi.lib")
  #pragma comment(lib, "shcore.lib")

  // Link a newer version of the side-by-side comctl32 dll.
  // Required to enable the newer native message box and visual styles on vista and above.
  #pragma comment(linker,                             \
          "\"/MANIFESTDEPENDENCY:type='Win32' "       \
          "name='Microsoft.Windows.Common-Controls' " \
          "version='6.0.0.0' "                        \
          "processorArchitecture='*' "                \
          "publicKeyToken='6595b64144ccf1df' "        \
          "language='*'\""                            \
      )
  #if HIGGS_OPENGL
   #pragma comment(lib, "OpenGL32.Lib")
   #pragma comment(lib, "GlU32.Lib")
  #endif

 #endif
#endif

//==============================================================================
#include <higgs_graphics/native/higgs_EventTracing.h>

#include "detail/higgs_AccessibilityHelpers.h"
#include "detail/higgs_ComponentHelpers.h"
#include "detail/higgs_FocusHelpers.h"
#include "detail/higgs_PointerState.h"
#include "detail/higgs_CustomMouseCursorInfo.h"
#include "detail/higgs_MouseInputSourceImpl.h"
#include "detail/higgs_MouseInputSourceList.h"
#include "detail/higgs_ScopedMessageBoxInterface.h"
#include "detail/higgs_ScopedMessageBoxImpl.h"
#include "detail/higgs_ScopedContentSharerInterface.h"
#include "detail/higgs_ScopedContentSharerImpl.h"
#include "detail/higgs_WindowingHelpers.h"
#include "detail/higgs_AlertWindowHelpers.h"
#include "detail/higgs_StandardCachedComponentImage.h"
#include "detail/higgs_TopLevelWindowManager.h"
#include "detail/higgs_FocusRestorer.h"

//==============================================================================
#if HIGGS_IOS || HIGGS_WINDOWS || HIGGS_LINUX || HIGGS_BSD
 #include "native/higgs_MultiTouchMapper.h"
#endif

#if HIGGS_ANDROID || HIGGS_WINDOWS || HIGGS_IOS || HIGGS_UNIT_TESTS
 #include "native/accessibility/higgs_AccessibilityTextHelpers.h"
#endif

#if HIGGS_MAC || HIGGS_IOS
 #include "native/accessibility/higgs_AccessibilitySharedCode_mac.mm"
 #include "native/higgs_CGMetalLayerRenderer_mac.h"

 #if HIGGS_IOS
  #include "native/higgs_UIViewComponentPeer_ios.mm"
  #include "native/accessibility/higgs_Accessibility_ios.mm"
  #include "native/higgs_WindowUtils_ios.mm"
  #include "native/higgs_Windowing_ios.mm"
  #include "native/higgs_NativeMessageBox_ios.mm"
  #include "native/higgs_NativeModalWrapperComponent_ios.h"
  #include "native/higgs_FileChooser_ios.mm"

  #if HIGGS_CONTENT_SHARING
   #include "native/higgs_ContentSharer_ios.cpp"
  #endif

 #else
  #include "native/accessibility/higgs_Accessibility_mac.mm"
  #include "native/higgs_PerScreenDisplayLinks_mac.h"
  #include "native/higgs_NSViewComponentPeer_mac.mm"
  #include "native/higgs_WindowUtils_mac.mm"
  #include "native/higgs_Windowing_mac.mm"
  #include "native/higgs_NativeMessageBox_mac.mm"
  #include "native/higgs_MainMenu_mac.mm"
  #include "native/higgs_FileChooser_mac.mm"
  #include "detail/higgs_ComponentPeerHelpers.h"
  #include "detail/higgs_ComponentPeerHelpers.cpp"
 #endif

 #include "native/higgs_MouseCursor_mac.mm"

#elif HIGGS_WINDOWS
 #include <higgs_core/detail/higgs_FunctionPointerDestructor.h>
 #include <higgs_graphics/native/higgs_Direct2DMetrics_windows.h>
 #include <higgs_graphics/native/higgs_Direct2DGraphicsContext_windows.h>
 #include <higgs_graphics/native/higgs_DirectX_windows.h>
 #include <higgs_graphics/native/higgs_Direct2DPixelDataPage_windows.h>
 #include <higgs_graphics/images/higgs_ImagePixelDataNativeExtensions.h>
 #include <higgs_graphics/native/higgs_Direct2DGraphicsContextImpl_windows.h>
 #include <higgs_graphics/native/higgs_Direct2DImage_windows.h>
 #include <higgs_graphics/native/higgs_Direct2DImageContext_windows.h>

 #include "native/accessibility/higgs_WindowsUIAWrapper_windows.h"
 #include "native/accessibility/higgs_AccessibilityElement_windows.h"
 #include "native/accessibility/higgs_UIAHelpers_windows.h"
 #include "native/accessibility/higgs_UIAProviders_windows.h"
 #include "native/accessibility/higgs_AccessibilityElement_windows.cpp"
 #include "native/accessibility/higgs_Accessibility_windows.cpp"
 #include "native/higgs_Direct2DHwndContext_windows.h"
 #include "native/higgs_Direct2DHwndContext_windows.cpp"
 #include "native/higgs_WindowsHooks_windows.h"
 #include "native/higgs_WindowUtils_windows.cpp"
 #include "native/higgs_VBlank_windows.cpp"
 #include "native/higgs_Windowing_windows.cpp"
 #include "native/higgs_WindowsHooks_windows.cpp"
 #include "native/higgs_NativeMessageBox_windows.cpp"
 #include "native/higgs_DragAndDrop_windows.cpp"
 #include "native/higgs_FileChooser_windows.cpp"

#elif HIGGS_LINUX || HIGGS_BSD
 #include "native/higgs_XSymbols_linux.cpp"
 #include "native/higgs_DragAndDrop_linux.cpp"

 HIGGS_BEGIN_IGNORE_WARNINGS_GCC_LIKE ("-Wzero-as-null-pointer-constant")

 #include "native/higgs_ScopedWindowAssociation_linux.h"
 #include "native/higgs_WindowUtils_linux.cpp"
 #include "native/higgs_Windowing_linux.cpp"
 #include "native/higgs_NativeMessageBox_linux.cpp"
 #include "native/higgs_XWindowSystem_linux.cpp"

 HIGGS_END_IGNORE_WARNINGS_GCC_LIKE

 #include "native/higgs_FileChooser_linux.cpp"

#elif HIGGS_ANDROID

 #include "higgs_core/files/higgs_common_MimeTypes.h"
 #include "native/accessibility/higgs_Accessibility_android.cpp"
 #include "native/higgs_WindowUtils_android.cpp"
 #include "native/higgs_Windowing_android.cpp"
 #include "native/higgs_NativeMessageBox_android.cpp"
 #include "native/higgs_FileChooser_android.cpp"

 #if HIGGS_CONTENT_SHARING
  #include "native/higgs_ContentSharer_android.cpp"
 #endif

#endif

//==============================================================================
// Depends on types defined in platform-specific windowing files
#include "mouse/higgs_MouseCursor.cpp"

#if HIGGS_UNIT_TESTS
 #include "native/accessibility/higgs_AccessibilityTextHelpers_test.cpp"
#endif

//==============================================================================
#include "native/accessibility/higgs_Accessibility.cpp"
#include "accessibility/higgs_AccessibilityHandler.cpp"

#include "application/higgs_Application.cpp"

#include "components/higgs_Component.cpp"
#include "components/higgs_ComponentListener.cpp"
#include "components/higgs_FocusTraverser.cpp"
#include "components/higgs_ModalComponentManager.cpp"

#include "desktop/higgs_Desktop.cpp"
#include "desktop/higgs_Displays.cpp"

#include "detail/higgs_AccessibilityHelpers.cpp"

#include "filebrowser/higgs_ContentSharer.cpp"

#include "keyboard/higgs_KeyboardFocusTraverser.cpp"

#include "mouse/higgs_MouseInputSource.cpp"

#include "native/higgs_ScopedDPIAwarenessDisabler.cpp"

#include "windows/higgs_NativeMessageBox.cpp"
#include "windows/higgs_AlertWindow.cpp"
#include "windows/higgs_ComponentPeer.cpp"
#include "windows/higgs_ScopedMessageBox.cpp"
#include "windows/higgs_TopLevelWindow.cpp"

#include "menus/higgs_PopupMenu.cpp"

#include "misc/higgs_DropShadower.cpp"
