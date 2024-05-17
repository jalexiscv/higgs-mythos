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

#include <higgs_core/system/higgs_TargetPlatform.h>

#if HiggsPlugin_Build_AAX

#include <AAX_Version.h>

static_assert (AAX_SDK_CURRENT_REVISION >= AAX_SDK_2p4p0_REVISION, "HIGGS requires AAX SDK version 2.4.0 or higher");

#if HIGGS_INTEL || (HIGGS_MAC && HIGGS_ARM)

#include <higgs_core/system/higgs_CompilerWarnings.h>

// Utilities
HIGGS_BEGIN_IGNORE_WARNINGS_GCC_LIKE ("-Wzero-as-null-pointer-constant")
#include <Libs/AAXLibrary/source/AAX_CAutoreleasePool.Win.cpp>
HIGGS_END_IGNORE_WARNINGS_GCC_LIKE

HIGGS_BEGIN_IGNORE_WARNINGS_GCC_LIKE ("-Wdeprecated-declarations",
                                     "-Wextra-semi",
                                     "-Wfloat-equal",
                                     "-Winconsistent-missing-destructor-override",
                                     "-Wshift-sign-overflow",
                                     "-Wunused-parameter",
                                     "-Wzero-as-null-pointer-constant",
                                     "-Wfour-char-constants",
                                     "-Wdeprecated-copy-with-user-provided-dtor",
                                     "-Wdeprecated",
                                     "-Wlanguage-extension-token",
                                     "-Wmicrosoft-enum-value",
                                     "-Wmisleading-indentation",
                                     "-Wregister")
HIGGS_BEGIN_IGNORE_WARNINGS_MSVC (6001 6053 4996 5033 4068 4996 5272)

#include <Libs/AAXLibrary/source/AAX_CChunkDataParser.cpp>
#include <Libs/AAXLibrary/source/AAX_CHostServices.cpp>

#if defined (_WIN32) && ! defined (WIN32)
 #define WIN32
#endif
#include <Libs/AAXLibrary/source/AAX_CMutex.cpp>

#include <Libs/AAXLibrary/source/AAX_CommonConversions.cpp>
#include <Libs/AAXLibrary/source/AAX_CPacketDispatcher.cpp>
#include <Libs/AAXLibrary/source/AAX_CString.cpp>

// Versioned Interfaces
#include <Interfaces/ACF/CACFClassFactory.cpp>
#include <Libs/AAXLibrary/source/AAX_CACFUnknown.cpp>

#include <Libs/AAXLibrary/source/AAX_CUIDs.cpp>
#include <Libs/AAXLibrary/source/AAX_IEffectDirectData.cpp>
#include <Libs/AAXLibrary/source/AAX_IEffectGUI.cpp>
#include <Libs/AAXLibrary/source/AAX_IEffectParameters.cpp>
#include <Libs/AAXLibrary/source/AAX_IHostProcessor.cpp>
#include <Libs/AAXLibrary/source/AAX_Properties.cpp>
#include <Libs/AAXLibrary/source/AAX_VAutomationDelegate.cpp>
#include <Libs/AAXLibrary/source/AAX_VCollection.cpp>
#include <Libs/AAXLibrary/source/AAX_VComponentDescriptor.cpp>
#include <Libs/AAXLibrary/source/AAX_VController.cpp>
#include <Libs/AAXLibrary/source/AAX_VDescriptionHost.cpp>
#include <Libs/AAXLibrary/source/AAX_VEffectDescriptor.cpp>
#include <Libs/AAXLibrary/source/AAX_VFeatureInfo.cpp>
#include <Libs/AAXLibrary/source/AAX_VHostProcessorDelegate.cpp>
#include <Libs/AAXLibrary/source/AAX_VHostServices.cpp>
#include <Libs/AAXLibrary/source/AAX_VPageTable.cpp>
#include <Libs/AAXLibrary/source/AAX_VPrivateDataAccess.cpp>
#include <Libs/AAXLibrary/source/AAX_VPropertyMap.cpp>
#include <Libs/AAXLibrary/source/AAX_VTransport.cpp>
#include <Libs/AAXLibrary/source/AAX_VViewContainer.cpp>
#include <Libs/AAXLibrary/source/AAX_CEffectDirectData.cpp>
#include <Libs/AAXLibrary/source/AAX_CEffectGUI.cpp>

#include <Libs/AAXLibrary/source/AAX_CEffectParameters.cpp>
#include <Libs/AAXLibrary/source/AAX_CHostProcessor.cpp>
#include <Libs/AAXLibrary/source/AAX_CParameter.cpp>
#include <Libs/AAXLibrary/source/AAX_CParameterManager.cpp>
#include <Libs/AAXLibrary/source/AAX_Init.cpp>
#include <Libs/AAXLibrary/source/AAX_SliderConversions.cpp>

HIGGS_END_IGNORE_WARNINGS_MSVC
HIGGS_END_IGNORE_WARNINGS_GCC_LIKE

#else
 #error "This version of the AAX SDK does not support the current platform."
#endif
#endif
