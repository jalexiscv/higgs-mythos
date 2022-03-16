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

#include "detail/higgs_ViewportHelpers.h"
#include "detail/higgs_ToolbarItemDragAndDropOverlayComponent.h"
#include "detail/higgs_ButtonAccessibilityHandler.h"

#include "widgets/higgs_ComboBox.cpp"
#include "widgets/higgs_ImageComponent.cpp"
#include "widgets/higgs_Label.cpp"
#include "widgets/higgs_ListBox.cpp"
#include "widgets/higgs_ProgressBar.cpp"
#include "widgets/higgs_Slider.cpp"
#include "widgets/higgs_TableHeaderComponent.cpp"
#include "widgets/higgs_TableListBox.cpp"
#include "widgets/higgs_TextEditorModel.cpp"
#include "widgets/higgs_TextEditor.cpp"
#include "widgets/higgs_Toolbar.cpp"
#include "widgets/higgs_ToolbarItemComponent.cpp"
#include "widgets/higgs_ToolbarItemPalette.cpp"
#include "widgets/higgs_TreeView.cpp"
