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

#pragma once


//==============================================================================
#ifdef linux
 #undef linux
#endif

struct TargetOS
{
    enum OS
    {
        windows = 0,
        osx,
        linux,
        unknown
    };

    static OS getThisOS() noexcept
    {
       #if HIGGS_WINDOWS
        return windows;
       #elif HIGGS_MAC
        return osx;
       #elif HIGGS_LINUX || HIGGS_BSD
        return linux;
       #else
        return unknown;
       #endif
    }
};

typedef TargetOS::OS DependencyPathOS;

//==============================================================================
#include "../Settings/mythos_StoredSettings.h"
#include "../Utility/UI/mythos_Icons.h"
#include "../Utility/Helpers/mythos_MiscUtilities.h"
#include "../Utility/Helpers/mythos_CodeHelpers.h"
#include "../Utility/Helpers/mythos_FileHelpers.h"
#include "../Utility/Helpers/mythos_ValueSourceHelpers.h"
#include "../Utility/Helpers/mythos_PresetIDs.h"
#include "mythos_CommandIDs.h"

//==============================================================================
const char* const projectItemDragType   = "Project Items";
const char* const drawableItemDragType  = "Drawable Items";
const char* const componentItemDragType = "Components";

enum ColourIds
{
    backgroundColourId                = 0x2340000,
    secondaryBackgroundColourId       = 0x2340001,
    defaultTextColourId               = 0x2340002,
    widgetTextColourId                = 0x2340003,
    defaultButtonBackgroundColourId   = 0x2340004,
    secondaryButtonBackgroundColourId = 0x2340005,
    userButtonBackgroundColourId      = 0x2340006,
    defaultIconColourId               = 0x2340007,
    treeIconColourId                  = 0x2340008,
    defaultHighlightColourId          = 0x2340009,
    defaultHighlightedTextColourId    = 0x234000a,
    codeEditorLineNumberColourId      = 0x234000b,
    activeTabIconColourId             = 0x234000c,
    inactiveTabBackgroundColourId     = 0x234000d,
    inactiveTabIconColourId           = 0x234000e,
    contentHeaderBackgroundColourId   = 0x234000f,
    widgetBackgroundColourId          = 0x2340010,
    secondaryWidgetBackgroundColourId = 0x2340011,
};

//==============================================================================
static constexpr int mythosstudioMajorVersion = ProjectInfo::versionNumber >> 16;
