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

#include "higgs_gui_basics.h"

#include "detail/higgs_ComponentHelpers.h"
#include "detail/higgs_WindowingHelpers.h"
#include "detail/higgs_FocusRestorer.h"

#include "commands/higgs_ApplicationCommandInfo.cpp"
#include "commands/higgs_ApplicationCommandManager.cpp"
#include "commands/higgs_ApplicationCommandTarget.cpp"
#include "commands/higgs_KeyPressMappingSet.cpp"

#include "filebrowser/higgs_DirectoryContentsDisplayComponent.cpp"
#include "filebrowser/higgs_DirectoryContentsList.cpp"
#include "filebrowser/higgs_FileBrowserComponent.cpp"
#include "filebrowser/higgs_FileChooser.cpp"
#include "filebrowser/higgs_FileChooserDialogBox.cpp"
#include "filebrowser/higgs_FileListComponent.cpp"
#include "filebrowser/higgs_FileSearchPathListComponent.cpp"
#include "filebrowser/higgs_FileTreeComponent.cpp"
#include "filebrowser/higgs_FilenameComponent.cpp"
#include "filebrowser/higgs_ImagePreviewComponent.cpp"

#include "keyboard/higgs_CaretComponent.cpp"
#include "keyboard/higgs_KeyListener.cpp"
#include "keyboard/higgs_KeyPress.cpp"
#include "keyboard/higgs_ModifierKeys.cpp"

#include "mouse/higgs_ComponentDragger.cpp"
#include "mouse/higgs_DragAndDropContainer.cpp"
#include "mouse/higgs_MouseEvent.cpp"
#include "mouse/higgs_MouseInactivityDetector.cpp"
#include "mouse/higgs_MouseListener.cpp"

#include "windows/higgs_CallOutBox.cpp"
#include "windows/higgs_DialogWindow.cpp"
#include "windows/higgs_DocumentWindow.cpp"
#include "windows/higgs_MessageBoxOptions.cpp"
#include "windows/higgs_ResizableWindow.cpp"
#include "windows/higgs_ThreadWithProgressWindow.cpp"
#include "windows/higgs_TooltipWindow.cpp"
#include "windows/higgs_VBlankAttachment.cpp"
#include "windows/higgs_NativeScaleFactorNotifier.cpp"

#include "menus/higgs_BurgerMenuComponent.cpp"
#include "menus/higgs_MenuBarComponent.cpp"
#include "menus/higgs_MenuBarModel.cpp"

#include "misc/higgs_BubbleComponent.cpp"
#include "misc/higgs_FocusOutline.cpp"
#include "misc/higgs_DrawableComponent.cpp"
