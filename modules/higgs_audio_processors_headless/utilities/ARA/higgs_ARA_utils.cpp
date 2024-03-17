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

#if (HiggsPlugin_Enable_ARA || HIGGS_INTERNAL_HAS_ARA) && (HIGGS_MAC || HIGGS_WINDOWS || HIGGS_LINUX)
namespace higgs
{
 #if ARA_ENABLE_INTERNAL_ASSERTS
HIGGS_API void HIGGS_CALLTYPE handleARAAssertion (const char* file, const int line, const char* diagnosis) noexcept
{
  #if (HIGGS_DEBUG && ! HIGGS_DISABLE_ASSERTIONS)
    DBG (diagnosis);
  #endif

    logAssertion (file, line);

  #if (HIGGS_DEBUG && ! HIGGS_DISABLE_ASSERTIONS)
    if (higgs_isRunningUnderDebugger())
        HIGGS_BREAK_IN_DEBUGGER;
    HIGGS_ANALYZER_NORETURN
  #endif
}
 #endif
}
#endif

#if HiggsPlugin_Enable_ARA
#include "higgs_ARADocumentControllerCommon.h"
#include "higgs_ARADocumentController.cpp"
#include "higgs_ARAModelObjects.cpp"
#include "higgs_ARAPlugInInstanceRoles.cpp"
#include "higgs_AudioProcessor_ARAExtensions.cpp"

ARA_SETUP_DEBUG_MESSAGE_PREFIX (HiggsPlugin_Name);
#endif
