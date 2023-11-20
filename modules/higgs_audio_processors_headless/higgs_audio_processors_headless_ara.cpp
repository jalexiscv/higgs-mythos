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

#include <higgs_core/system/higgs_CompilerWarnings.h>
#include <higgs_core/system/higgs_TargetPlatform.h>

#include <higgs_audio_processors_headless/format/higgs_PluginFormatDefs.h>

#include <higgs_audio_processors_headless/utilities/ARA/higgs_ARADebug.h>

/* Having WIN32_LEAN_AND_MEAN defined at the point of including ARADebug.c will produce warnings.

   To prevent such problems it's easiest to have it in its own translation unit.
*/

#if (HiggsPlugin_Enable_ARA || HIGGS_INTERNAL_HAS_ARA) && (HIGGS_MAC || HIGGS_WINDOWS || HIGGS_LINUX)

HIGGS_BEGIN_IGNORE_WARNINGS_GCC_LIKE ("-Wgnu-zero-variadic-macro-arguments", "-Wmissing-prototypes")
 #include <ARA_Library/Debug/ARADebug.c>
HIGGS_END_IGNORE_WARNINGS_GCC_LIKE

#else
 // To get around 'has no symbols' warnings emitted by libtool
 void higgs_audioProcessorsHeadlessAraPlaceholder();
 void higgs_audioProcessorsHeadlessAraPlaceholder() {}
#endif
