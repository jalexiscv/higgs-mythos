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

#include "detail/higgs_LookAndFeelHelpers.h"
#include "detail/higgs_ScopedMessageBoxInterface.h"
#include "detail/higgs_AlertWindowHelpers.h"

#include "lookandfeel/higgs_LookAndFeel.cpp"
#include "lookandfeel/higgs_LookAndFeel_V1.cpp"
#include "lookandfeel/higgs_LookAndFeel_V2.cpp"
#include "lookandfeel/higgs_LookAndFeel_V3.cpp"
#include "lookandfeel/higgs_LookAndFeel_V4.cpp"

#include "positioning/higgs_MarkerList.cpp"
#include "positioning/higgs_RelativeCoordinate.cpp"
#include "positioning/higgs_RelativeCoordinatePositioner.cpp"
#include "positioning/higgs_RelativeParallelogram.cpp"
#include "positioning/higgs_RelativePoint.cpp"
#include "positioning/higgs_RelativePointPath.cpp"
#include "positioning/higgs_RelativeRectangle.cpp"
