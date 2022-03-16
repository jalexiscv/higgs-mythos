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

  ID:                 higgs_gui_basics
  vendor:             higgs
  version:            9.0.5
  name:               Higgs Mythos GUI core classes
  description:        Basic user-interface components and related classes.
  website:            https://github.com/jalexiscv/higgs-mythos
  license:            AGPLv3
  minimumCppStandard: 17

  dependencies:       higgs_graphics higgs_data_structures
  OSXFrameworks:      Cocoa QuartzCore
  WeakOSXFrameworks:  Metal MetalKit
  iOSFrameworks:      CoreServices UIKit
  WeakiOSFrameworks:  Metal MetalKit UniformTypeIdentifiers UserNotifications

 END_HIGGS_MODULE_DECLARATION

*******************************************************************************/


#pragma once
#define HIGGS_GUI_BASICS_H_INCLUDED

#include <higgs_graphics/higgs_graphics.h>
#include <higgs_data_structures/higgs_data_structures.h>

//==============================================================================
/** Config: HIGGS_ENABLE_REPAINT_DEBUGGING
    If this option is turned on, each area of the screen that gets repainted will
    flash in a random colour, so that you can see exactly which bits of your
    components are being drawn.
*/
#ifndef HIGGS_ENABLE_REPAINT_DEBUGGING
 #define HIGGS_ENABLE_REPAINT_DEBUGGING 0
#endif

/** Config: HIGGS_USE_XRANDR
    Enables Xrandr multi-monitor support (Linux only).
    Unless you specifically want to disable this, it's best to leave this option turned on.
    Note that your users do not need to have Xrandr installed for your HIGGS app to run, as
    the availability of Xrandr is queried during runtime.
*/
#ifndef HIGGS_USE_XRANDR
 #define HIGGS_USE_XRANDR 1
#endif

/** Config: HIGGS_USE_XINERAMA
    Enables Xinerama multi-monitor support (Linux only).
    Unless you specifically want to disable this, it's best to leave this option turned on.
    This will be used as a fallback if HIGGS_USE_XRANDR not set or libxrandr cannot be found.
    Note that your users do not need to have Xinerama installed for your HIGGS app to run, as
    the availability of Xinerama is queried during runtime.
*/
#ifndef HIGGS_USE_XINERAMA
 #define HIGGS_USE_XINERAMA 1
#endif

/** Config: HIGGS_USE_XSHM
    Enables X shared memory for faster rendering on Linux. This is best left turned on
    unless you have a good reason to disable it.
*/
#ifndef HIGGS_USE_XSHM
 #define HIGGS_USE_XSHM 1
#endif

/** Config: HIGGS_USE_XRENDER
    Enables XRender to allow semi-transparent windowing on Linux.
*/
#ifndef HIGGS_USE_XRENDER
 #define HIGGS_USE_XRENDER 0
#endif

/** Config: HIGGS_USE_XCURSOR
    Uses XCursor to allow ARGB cursor on Linux. This is best left turned on unless you have
    a good reason to disable it.
*/
#ifndef HIGGS_USE_XCURSOR
 #define HIGGS_USE_XCURSOR 1
#endif

/** Config: HIGGS_USE_XINPUT
    Uses XInput (v2.2+) to allow multitouch on Linux. This is best left turned on unless you
    have a good reason to disable it.
    The availability of XInput v2.2+ is queried at runtime so your users do not need to have
    XInput installed for your HIGGS app to run.
*/
#ifndef HIGGS_USE_XINPUT
 #define HIGGS_USE_XINPUT 1
#endif

/** Config: HIGGS_WIN_PER_MONITOR_DPI_AWARE
    Enables per-monitor DPI awareness on Windows 8.1 and above.
*/
#ifndef HIGGS_WIN_PER_MONITOR_DPI_AWARE
 #define HIGGS_WIN_PER_MONITOR_DPI_AWARE 1
#endif

//==============================================================================
namespace higgs
{
    class Component;
    class LookAndFeel;
    class MouseInputSource;
    class ComponentPeer;
    class MouseEvent;
    struct MouseWheelDetails;
    struct PenDetails;
    class ToggleButton;
    class TextButton;
    class AlertWindow;
    class TextLayout;
    class ScrollBar;
    class ComboBox;
    class Button;
    class FilenameComponent;
    class ResizableWindow;
    class MenuBarComponent;
    class GlyphArrangement;
    class TableHeaderComponent;
    class Toolbar;
    class PopupMenu;
    class ProgressBar;
    class FileBrowserComponent;
    class DirectoryContentsDisplayComponent;
    class FilePreviewComponent;
    class CallOutBox;
    class Drawable;
    class DrawablePath;
    class DrawableComposite;
    class CaretComponent;
    class KeyPressMappingSet;
    class ApplicationCommandManagerListener;
    class DrawableButton;
    class Displays;
    class AccessibilityHandler;
    class KeyboardFocusTraverser;

    class FlexBox;
    class Grid;
    class FocusOutline;

   #if HIGGS_MAC || HIGGS_WINDOWS || HIGGS_LINUX || HIGGS_BSD
    Image createSnapshotOfNativeWindow (void* nativeWindowHandle);
   #endif

    namespace detail
    {
        struct ComponentHelpers;
        class MouseInputSourceImpl;
        class MouseInputSourceList;
        class PointerState;
        class ScopedMessageBoxImpl;
        class ToolbarItemDragAndDropOverlayComponent;
        class TopLevelWindowManager;
    } // namespace detail

} // namespace higgs

#include "mouse/higgs_MouseCursor.h"
#include "mouse/higgs_MouseListener.h"
#include "keyboard/higgs_ModifierKeys.h"
#include "mouse/higgs_MouseInputSource.h"
#include "mouse/higgs_MouseEvent.h"
#include "keyboard/higgs_KeyPress.h"
#include "keyboard/higgs_KeyListener.h"
#include "components/higgs_ComponentTraverser.h"
#include "components/higgs_FocusTraverser.h"
#include "components/higgs_ModalComponentManager.h"
#include "components/higgs_ComponentListener.h"
#include "components/higgs_CachedComponentImage.h"
#include "desktop/higgs_DesktopWindowOptions.h"
#include "components/higgs_Component.h"
#include "layout/higgs_ComponentAnimator.h"
#include "desktop/higgs_Desktop.h"
#include "desktop/higgs_Displays.h"
#include "layout/higgs_ComponentBoundsConstrainer.h"
#include "layout/higgs_BorderedComponentBoundsConstrainer.h"
#include "mouse/higgs_ComponentDragger.h"
#include "mouse/higgs_DragAndDropTarget.h"
#include "mouse/higgs_DragAndDropContainer.h"
#include "mouse/higgs_FileDragAndDropTarget.h"
#include "mouse/higgs_SelectedItemSet.h"
#include "mouse/higgs_MouseInactivityDetector.h"
#include "mouse/higgs_TextDragAndDropTarget.h"
#include "mouse/higgs_TooltipClient.h"
#include "keyboard/higgs_CaretComponent.h"
#include "keyboard/higgs_KeyboardFocusTraverser.h"
#include "keyboard/higgs_SystemClipboard.h"
#include "keyboard/higgs_TextEditorKeyMapper.h"
#include "keyboard/higgs_TextInputTarget.h"
#include "commands/higgs_ApplicationCommandID.h"
#include "commands/higgs_ApplicationCommandInfo.h"
#include "commands/higgs_ApplicationCommandTarget.h"
#include "commands/higgs_ApplicationCommandManager.h"
#include "commands/higgs_KeyPressMappingSet.h"
#include "buttons/higgs_Button.h"
#include "buttons/higgs_ArrowButton.h"
#include "misc/higgs_DrawableComponent.h"
#include "misc/higgs_OwningDrawableComponent.h"
#include "buttons/higgs_DrawableButton.h"
#include "buttons/higgs_HyperlinkButton.h"
#include "buttons/higgs_ImageButton.h"
#include "buttons/higgs_ShapeButton.h"
#include "buttons/higgs_TextButton.h"
#include "buttons/higgs_ToggleButton.h"
#include "layout/higgs_AnimatedPosition.h"
#include "layout/higgs_AnimatedPositionBehaviours.h"
#include "layout/higgs_ComponentBuilder.h"
#include "layout/higgs_ComponentMovementWatcher.h"
#include "layout/higgs_ConcertinaPanel.h"
#include "layout/higgs_GroupComponent.h"
#include "layout/higgs_ResizableBorderComponent.h"
#include "layout/higgs_ResizableCornerComponent.h"
#include "layout/higgs_ResizableEdgeComponent.h"
#include "layout/higgs_ScrollBar.h"
#include "layout/higgs_StretchableLayoutManager.h"
#include "layout/higgs_StretchableLayoutResizerBar.h"
#include "layout/higgs_StretchableObjectResizer.h"
#include "layout/higgs_TabbedButtonBar.h"
#include "layout/higgs_TabbedComponent.h"
#include "accessibility/interfaces/higgs_AccessibilityCellInterface.h"
#include "accessibility/interfaces/higgs_AccessibilityTableInterface.h"
#include "accessibility/interfaces/higgs_AccessibilityTextInterface.h"
#include "accessibility/interfaces/higgs_AccessibilityValueInterface.h"
#include "accessibility/enums/higgs_AccessibilityActions.h"
#include "accessibility/enums/higgs_AccessibilityEvent.h"
#include "accessibility/enums/higgs_AccessibilityRole.h"
#include "accessibility/higgs_AccessibilityState.h"
#include "accessibility/higgs_AccessibilityHandler.h"
#include "layout/higgs_Viewport.h"
#include "menus/higgs_PopupMenu.h"
#include "menus/higgs_MenuBarModel.h"
#include "menus/higgs_MenuBarComponent.h"
#include "positioning/higgs_RelativeCoordinate.h"
#include "positioning/higgs_MarkerList.h"
#include "positioning/higgs_RelativePoint.h"
#include "positioning/higgs_RelativeRectangle.h"
#include "positioning/higgs_RelativeCoordinatePositioner.h"
#include "positioning/higgs_RelativeParallelogram.h"
#include "positioning/higgs_RelativePointPath.h"
#include "widgets/higgs_TextEditor.h"
#include "widgets/higgs_Label.h"
#include "widgets/higgs_ComboBox.h"
#include "widgets/higgs_ImageComponent.h"
#include "widgets/higgs_ListBox.h"
#include "widgets/higgs_ProgressBar.h"
#include "widgets/higgs_Slider.h"
#include "widgets/higgs_TableHeaderComponent.h"
#include "widgets/higgs_TableListBox.h"
#include "widgets/higgs_Toolbar.h"
#include "widgets/higgs_ToolbarItemComponent.h"
#include "widgets/higgs_ToolbarItemFactory.h"
#include "widgets/higgs_ToolbarItemPalette.h"
#include "menus/higgs_BurgerMenuComponent.h"
#include "buttons/higgs_ToolbarButton.h"
#include "misc/higgs_DropShadower.h"
#include "misc/higgs_FocusOutline.h"
#include "widgets/higgs_TreeView.h"
#include "windows/higgs_TopLevelWindow.h"
#include "windows/higgs_MessageBoxOptions.h"
#include "windows/higgs_ScopedMessageBox.h"
#include "windows/higgs_AlertWindow.h"
#include "windows/higgs_CallOutBox.h"
#include "windows/higgs_ComponentPeer.h"
#include "windows/higgs_ResizableWindow.h"
#include "windows/higgs_DocumentWindow.h"
#include "windows/higgs_DialogWindow.h"
#include "windows/higgs_NativeMessageBox.h"
#include "windows/higgs_ThreadWithProgressWindow.h"
#include "windows/higgs_TooltipWindow.h"
#include "windows/higgs_VBlankAttachment.h"
#include "windows/higgs_WindowUtils.h"
#include "windows/higgs_NativeScaleFactorNotifier.h"
#include "layout/higgs_MultiDocumentPanel.h"
#include "layout/higgs_SidePanel.h"
#include "filebrowser/higgs_FileBrowserListener.h"
#include "filebrowser/higgs_DirectoryContentsList.h"
#include "filebrowser/higgs_DirectoryContentsDisplayComponent.h"
#include "filebrowser/higgs_FileBrowserComponent.h"
#include "filebrowser/higgs_FileChooser.h"
#include "filebrowser/higgs_FileChooserDialogBox.h"
#include "filebrowser/higgs_FileListComponent.h"
#include "filebrowser/higgs_FilenameComponent.h"
#include "filebrowser/higgs_FilePreviewComponent.h"
#include "filebrowser/higgs_FileSearchPathListComponent.h"
#include "filebrowser/higgs_FileTreeComponent.h"
#include "filebrowser/higgs_ImagePreviewComponent.h"
#include "filebrowser/higgs_ContentSharer.h"
#include "properties/higgs_PropertyComponent.h"
#include "properties/higgs_BooleanPropertyComponent.h"
#include "properties/higgs_ButtonPropertyComponent.h"
#include "properties/higgs_ChoicePropertyComponent.h"
#include "properties/higgs_PropertyPanel.h"
#include "properties/higgs_SliderPropertyComponent.h"
#include "properties/higgs_TextPropertyComponent.h"
#include "properties/higgs_MultiChoicePropertyComponent.h"
#include "application/higgs_Application.h"
#include "misc/higgs_BubbleComponent.h"
#include "lookandfeel/higgs_LookAndFeel.h"
#include "lookandfeel/higgs_LookAndFeel_V2.h"
#include "lookandfeel/higgs_LookAndFeel_V1.h"
#include "lookandfeel/higgs_LookAndFeel_V3.h"
#include "lookandfeel/higgs_LookAndFeel_V4.h"
#include "mouse/higgs_LassoComponent.h"

#if HIGGS_LINUX || HIGGS_BSD
 #if HIGGS_GUI_BASICS_INCLUDE_XHEADERS
  // If you're missing these headers, you need to install the libx11-dev package
  HIGGS_BEGIN_IGNORE_WARNINGS_GCC_LIKE ("-Wvariadic-macros")
  #include <X11/Xlib.h>
  HIGGS_END_IGNORE_WARNINGS_GCC_LIKE
  #include <X11/Xatom.h>
  #include <X11/Xresource.h>
  #include <X11/Xutil.h>
  #include <X11/Xmd.h>
  #include <X11/keysym.h>
  #include <X11/XKBlib.h>
  #include <X11/cursorfont.h>
  #include <unistd.h>

  #if HIGGS_USE_XRANDR
   // If you're missing this header, you need to install the libxrandr-dev package
   #include <X11/extensions/Xrandr.h>
  #endif

  #if HIGGS_USE_XINERAMA
   // If you're missing this header, you need to install the libxinerama-dev package
   #include <X11/extensions/Xinerama.h>
  #endif

  #if HIGGS_USE_XSHM
   #include <X11/extensions/XShm.h>
   #include <sys/shm.h>
   #include <sys/ipc.h>
  #endif

  #if HIGGS_USE_XRENDER
   // If you're missing these headers, you need to install the libxrender-dev and libxcomposite-dev packages
   #include <X11/extensions/Xrender.h>
   #include <X11/extensions/Xcomposite.h>
  #endif

  #if HIGGS_USE_XCURSOR
   // If you're missing this header, you need to install the libxcursor-dev package
   #include <X11/Xcursor/Xcursor.h>
  #endif

  #if HIGGS_USE_XINPUT
   // If you're missing this header, you need to install the libxi-dev package
   #include <X11/extensions/XInput2.h>
  #endif

  #undef SIZEOF
  #undef KeyPress

  #include "native/higgs_XWindowSystem_linux.h"
  #include "native/higgs_XSymbols_linux.h"
 #endif
#endif

#if HIGGS_GUI_BASICS_INCLUDE_SCOPED_THREAD_DPI_AWARENESS_SETTER && HIGGS_WINDOWS
 #include "native/higgs_ScopedThreadDPIAwarenessSetter_windows.h"
#endif

#include "layout/higgs_FlexItem.h"
#include "layout/higgs_FlexBox.h"

#include "layout/higgs_GridItem.h"
#include "layout/higgs_Grid.h"
#include "native/higgs_ScopedDPIAwarenessDisabler.h"
