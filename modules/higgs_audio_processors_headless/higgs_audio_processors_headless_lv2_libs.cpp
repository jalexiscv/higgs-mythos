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

 #ifndef _CRT_SECURE_NO_WARNINGS
  #define _CRT_SECURE_NO_WARNINGS
 #endif

#include <higgs_core/system/higgs_CompilerWarnings.h>
#include <higgs_core/system/higgs_TargetPlatform.h>

#include <higgs_audio_processors_headless/format/higgs_PluginFormatDefs.h>

#if HIGGS_INTERNAL_HAS_LV2
 #include <higgs_audio_processors_headless/format_types/higgs_LV2SupportLibs.cpp>
#else
 // To get around 'has no symbols' warnings emitted by libtool
 void higgs_audioProcessorsHeadlessLv2LibsPlaceholder();
 void higgs_audioProcessorsHeadlessLv2LibsPlaceholder() {}
#endif
