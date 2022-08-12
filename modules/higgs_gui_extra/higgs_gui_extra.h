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

  ID:                     higgs_gui_extra
  vendor:                 higgs
  version:                9.0.5
  name:                   Higgs Mythos extended GUI classes
  description:            Miscellaneous GUI classes for specialised tasks.
  website:                https://github.com/jalexiscv/higgs-mythos
  license:                AGPLv3
  minimumCppStandard:     17

  dependencies:           higgs_gui_basics
  OSXFrameworks:          WebKit
  iOSFrameworks:          WebKit
  WeakiOSFrameworks:      UserNotifications
  WeakMacOSFrameworks:    UserNotifications

 END_HIGGS_MODULE_DECLARATION

*******************************************************************************/


#pragma once
#define HIGGS_GUI_EXTRA_H_INCLUDED

#include <higgs_gui_basics/higgs_gui_basics.h>

//==============================================================================
/** Config: HIGGS_WEB_BROWSER
    This lets you disable the WebBrowserComponent class.
    If you're not using any embedded web-pages, turning this off may reduce your code size.
*/
#ifndef HIGGS_WEB_BROWSER
 #define HIGGS_WEB_BROWSER 1
#endif

/** Config: HIGGS_USE_WIN_WEBVIEW2_WITH_STATIC_LINKING
    Enables the use of the Microsoft Edge (Chromium) WebView2 browser on Windows.

    If using the MythosStudio, the Microsoft.Web.WebView2 package will be added to the
    project solution if this flag is enabled. If you are building using CMake you
    will need to manually add the package via the NuGet package manager.

    Using this flag requires statically linking against WebView2LoaderStatic.lib,
    which at this time is only available through the NuGet package, but is missing
    in VCPKG.

    In addition to enabling this macro, you will need to use the
    WebBrowserComponent::Options::Backend::webview2 option when instantiating the
    WebBrowserComponent.
*/
#ifndef HIGGS_USE_WIN_WEBVIEW2_WITH_STATIC_LINKING
 #define HIGGS_USE_WIN_WEBVIEW2_WITH_STATIC_LINKING 0
#else
 #define HIGGS_USE_WIN_WEBVIEW2 1
#endif

/** Config: HIGGS_USE_WIN_WEBVIEW2
    Enables the use of the Microsoft Edge (Chromium) WebView2 browser on Windows.

    If using the MythosStudio, the Microsoft.Web.WebView2 package will be added to the
    project solution if this flag is enabled. If you are building using CMake you
    will need to manually add the package via the Visual Studio package manager.

    If the WITH_STATIC_LINKING variant of this flag is also set, you must statically
    link against WebView2LoaderStatic.lib, otherwise dynamic loading will be used.

    See more about dynamic linking in the documentation of
    WebBrowserComponent::Options::WinWebView2::withDLLLocation().

    In addition to enabling this macro, you will need to use the
    WebBrowserComponent::Options::Backend::webview2 option when instantiating the
    WebBrowserComponent.
*/
#ifndef HIGGS_USE_WIN_WEBVIEW2
 #define HIGGS_USE_WIN_WEBVIEW2 0
#endif

/** Config: HIGGS_ENABLE_LIVE_CONSTANT_EDITOR
    This lets you turn on the HIGGS_ENABLE_LIVE_CONSTANT_EDITOR support (desktop only). By default
    this will be enabled for debug builds and disabled for release builds. See the documentation
    for that macro for more details.
*/
#ifndef HIGGS_ENABLE_LIVE_CONSTANT_EDITOR
 #if HIGGS_DEBUG && ! (HIGGS_IOS || HIGGS_ANDROID)
  #define HIGGS_ENABLE_LIVE_CONSTANT_EDITOR 1
 #endif
#endif

//==============================================================================
#include "documents/higgs_FileBasedDocument.h"
#include "code_editor/higgs_CodeDocument.h"
#include "code_editor/higgs_CodeEditorComponent.h"
#include "code_editor/higgs_CodeTokeniser.h"
#include "code_editor/higgs_CPlusPlusCodeTokeniser.h"
#include "code_editor/higgs_CPlusPlusCodeTokeniserFunctions.h"
#include "code_editor/higgs_XMLCodeTokeniser.h"
#include "code_editor/higgs_LuaCodeTokeniser.h"
#include "embedding/higgs_ActiveXControlComponent.h"
#include "embedding/higgs_AndroidViewComponent.h"
#include "embedding/higgs_NSViewComponent.h"
#include "embedding/higgs_UIViewComponent.h"
#include "embedding/higgs_XEmbedComponent.h"
#include "embedding/higgs_HWNDComponent.h"
#include "misc/higgs_AppleRemote.h"
#include "misc/higgs_BubbleMessageComponent.h"
#include "misc/higgs_ColourSelector.h"
#include "misc/higgs_KeyMappingEditorComponent.h"
#include "misc/higgs_PreferencesPanel.h"
#include "misc/higgs_PushNotifications.h"
#include "misc/higgs_RecentlyOpenedFilesList.h"
#include "misc/higgs_SplashScreen.h"
#include "misc/higgs_SystemTrayIconComponent.h"
#include "misc/higgs_WebBrowserComponent.h"
#include "misc/higgs_LiveConstantEditor.h"
#include "misc/higgs_AnimatedAppComponent.h"
#include "detail/higgs_WebControlRelayEvents.h"
#include "misc/higgs_WebControlRelays.h"
#include "misc/higgs_WebControlParameterIndexReceiver.h"
