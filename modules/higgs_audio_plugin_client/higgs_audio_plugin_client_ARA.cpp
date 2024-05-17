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
#include <higgs_audio_plugin_client/detail/higgs_CheckSettingMacros.h>

#if HiggsPlugin_Enable_ARA

#include <higgs_audio_plugin_client/detail/higgs_IncludeSystemHeaders.h>
#include <higgs_audio_plugin_client/detail/higgs_IncludeModuleHeaders.h>

HIGGS_BEGIN_IGNORE_WARNINGS_GCC_LIKE ("-Wunused-parameter",
                                     "-Wgnu-zero-variadic-macro-arguments",
                                     "-Wmissing-prototypes",
                                     "-Wfloat-equal",
                                     "-Wc++20-extensions")
HIGGS_BEGIN_IGNORE_WARNINGS_MSVC (4100)

#include <ARA_Library/PlugIn/ARAPlug.cpp>
#include <ARA_Library/Dispatch/ARAPlugInDispatch.cpp>
#include <ARA_Library/Utilities/ARAPitchInterpretation.cpp>
#include <ARA_Library/Utilities/ARAChannelFormat.cpp>

HIGGS_END_IGNORE_WARNINGS_MSVC
HIGGS_END_IGNORE_WARNINGS_GCC_LIKE

#else
// To get around 'has no symbols' warnings emitted by libtool
void higgs_audioPluginClientAraPlaceholder();
void higgs_audioPluginClientAraPlaceholder() {}
#endif
