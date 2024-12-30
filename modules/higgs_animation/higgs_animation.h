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

  ID:                 higgs_animation
  vendor:             higgs
  version:            9.0.5
  name:               Higgs Mythos Animation classes
  description:        Classes for defining and handling animations.
  website:            https://github.com/jalexiscv/higgs-mythos
  license:            AGPLv3
  minimumCppStandard: 17

  dependencies:       higgs_gui_basics

 END_HIGGS_MODULE_DECLARATION

*******************************************************************************/


#pragma once
#define HIGGS_ANIMATION_H_INCLUDED

#include <higgs_gui_basics/higgs_gui_basics.h>

//==============================================================================
#include "detail/higgs_ArrayAndTupleOps.h"

//==============================================================================
#include "animation/higgs_Animator.h"
#include "animation/higgs_AnimatorSetBuilder.h"
#include "animation/higgs_AnimatorUpdater.h"
#include "animation/higgs_Easings.h"
#include "animation/higgs_StaticAnimationLimits.h"
#include "animation/higgs_ValueAnimatorBuilder.h"
#include "animation/higgs_VBlankAnimatorUpdater.h"
