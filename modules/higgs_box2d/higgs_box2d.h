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

  ID:                 higgs_box2d
  vendor:             higgs
  version:            9.0.5
  name:               Higgs Mythos wrapper for the Box2D physics engine
  description:        The Box2D physics engine and some utility classes.
  website:            https://github.com/jalexiscv/higgs-mythos
  license:            AGPLv3
  minimumCppStandard: 17

  dependencies:       higgs_graphics

 END_HIGGS_MODULE_DECLARATION

*******************************************************************************/


#pragma once
#define HIGGS_BOX2D_H_INCLUDED

//==============================================================================
#include <higgs_graphics/higgs_graphics.h>

HIGGS_BEGIN_IGNORE_WARNINGS_GCC_LIKE ("-Wconversion",
                                     "-Wdeprecated",
                                     "-Wfloat-equal",
                                     "-Wmaybe-uninitialized",
                                     "-Wshadow-field",
                                     "-Wsign-conversion",
                                     "-Wzero-as-null-pointer-constant",
                                     "-Wsuggest-override")

#include <climits>
#include <cfloat>

#include "box2d/Box2D.h"

HIGGS_END_IGNORE_WARNINGS_GCC_LIKE

// For some reason, Doxygen sees this as a re-definition of Box2DRenderer
/** @cond */
#include "utils/higgs_Box2DRenderer.h"
/** @endcond */
