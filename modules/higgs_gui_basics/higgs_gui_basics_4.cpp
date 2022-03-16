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
#include "detail/higgs_ButtonAccessibilityHandler.h"

#include "buttons/higgs_ArrowButton.cpp"
#include "buttons/higgs_Button.cpp"
#include "buttons/higgs_DrawableButton.cpp"
#include "buttons/higgs_HyperlinkButton.cpp"
#include "buttons/higgs_ImageButton.cpp"
#include "buttons/higgs_ShapeButton.cpp"
#include "buttons/higgs_TextButton.cpp"
#include "buttons/higgs_ToggleButton.cpp"
#include "buttons/higgs_ToolbarButton.cpp"

#include "layout/higgs_ComponentAnimator.cpp"
#include "layout/higgs_ComponentBoundsConstrainer.cpp"
#include "layout/higgs_BorderedComponentBoundsConstrainer.cpp"
#include "layout/higgs_ComponentBuilder.cpp"
#include "layout/higgs_ComponentMovementWatcher.cpp"
#include "layout/higgs_ConcertinaPanel.cpp"
#include "layout/higgs_FlexBox.cpp"
#include "layout/higgs_Grid.cpp"
#include "layout/higgs_GridItem.cpp"
#include "layout/higgs_GroupComponent.cpp"
#include "layout/higgs_MultiDocumentPanel.cpp"
#include "layout/higgs_ResizableBorderComponent.cpp"
#include "layout/higgs_ResizableCornerComponent.cpp"
#include "layout/higgs_ResizableEdgeComponent.cpp"
#include "layout/higgs_ScrollBar.cpp"
#include "layout/higgs_SidePanel.cpp"
#include "layout/higgs_StretchableLayoutManager.cpp"
#include "layout/higgs_StretchableLayoutResizerBar.cpp"
#include "layout/higgs_StretchableObjectResizer.cpp"
#include "layout/higgs_TabbedButtonBar.cpp"
#include "layout/higgs_TabbedComponent.cpp"
#include "layout/higgs_Viewport.cpp"

#include "properties/higgs_BooleanPropertyComponent.cpp"
#include "properties/higgs_ButtonPropertyComponent.cpp"
#include "properties/higgs_ChoicePropertyComponent.cpp"
#include "properties/higgs_MultiChoicePropertyComponent.cpp"
#include "properties/higgs_PropertyComponent.cpp"
#include "properties/higgs_PropertyPanel.cpp"
#include "properties/higgs_SliderPropertyComponent.cpp"
#include "properties/higgs_TextPropertyComponent.cpp"
