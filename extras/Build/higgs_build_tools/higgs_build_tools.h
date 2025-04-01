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

  ID:                 higgs_build_tools
  vendor:             higgs
  version:            9.0.5
  name:               HIGGS Build Tools
  description:        Classes for generating intermediate files for HIGGS projects.
  website:            https://github.com/jalexiscv/higgs-mythos
  license:            AGPLv3

  dependencies:       higgs_graphics

 END_HIGGS_MODULE_DECLARATION

*******************************************************************************/

// This module is shared by higgsaide and the MythosStudio, but should not be
// considered 'public'. That is, its API, functionality, and contents (and
// existence!) may change between releases without warning.

#pragma once
#define HIGGS_BUILD_TOOLS_H_INCLUDED

#include <higgs_graphics/higgs_graphics.h>

#include "utils/higgs_ProjectType.h"
#include "utils/higgs_BuildHelperFunctions.h"
#include "utils/higgs_BinaryResourceFile.h"
#include "utils/higgs_RelativePath.h"
#include "utils/higgs_Icons.h"
#include "utils/higgs_PlistOptions.h"
#include "utils/higgs_ResourceFileHelpers.h"
#include "utils/higgs_ResourceRc.h"
#include "utils/higgs_VersionNumbers.h"
#include "utils/higgs_Entitlements.h"
