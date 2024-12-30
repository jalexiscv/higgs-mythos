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

#ifdef HIGGS_ANIMATION_H_INCLUDED
 /* When you add this cpp file to your project, you mustn't include it in a file where you've
    already included any other headers - just put it inside a file on its own, possibly with your config
    flags preceding it, but don't include anything else. That also includes avoiding any automatic prefix
    header files that the compiler may be using.
 */
 #error "Incorrect use of HIGGS cpp file"
#endif

//==============================================================================
#include "higgs_animation.h"

//==============================================================================
namespace chromium
{

HIGGS_BEGIN_IGNORE_WARNINGS_MSVC (4701 6001)
HIGGS_BEGIN_IGNORE_WARNINGS_GCC_LIKE ("-Wfloat-conversion",
                                     "-Wfloat-equal",
                                     "-Wconditional-uninitialized")


#include "detail/chromium/cubic_bezier.h"
#include "detail/chromium/cubic_bezier.cc"

HIGGS_END_IGNORE_WARNINGS_MSVC
HIGGS_END_IGNORE_WARNINGS_GCC_LIKE

} // namespace chromium

//==============================================================================
#include "animation/higgs_Animator.cpp"
#include "animation/higgs_AnimatorSetBuilder.cpp"
#include "animation/higgs_AnimatorUpdater.cpp"
#include "animation/higgs_Easings.cpp"
#include "animation/higgs_ValueAnimatorBuilder.cpp"
