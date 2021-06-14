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

  ID:                 higgs_data_structures
  vendor:             higgs
  version:            9.0.5
  name:               Higgs Mythos data model helper classes
  description:        Classes for undo/redo management, and smart data structures.
  website:            https://github.com/jalexiscv/higgs-mythos
  license:            AGPLv3
  minimumCppStandard: 17

  dependencies:       higgs_events

 END_HIGGS_MODULE_DECLARATION

*******************************************************************************/


#pragma once
#define HIGGS_DATA_STRUCTURES_H_INCLUDED

//==============================================================================
#include <higgs_events/higgs_events.h>

#include "undomanager/higgs_UndoableAction.h"
#include "undomanager/higgs_UndoManager.h"
#include "values/higgs_Value.h"
#include "values/higgs_ValueTree.h"
#include "values/higgs_ValueTreeSynchroniser.h"
#include "values/higgs_CachedValue.h"
#include "values/higgs_ValueTreePropertyWithDefault.h"
#include "app_properties/higgs_PropertiesFile.h"
#include "app_properties/higgs_ApplicationProperties.h"
