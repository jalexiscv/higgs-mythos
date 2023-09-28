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

#ifdef HIGGS_AUDIO_PROCESSORS_H_INCLUDED
 /* When you add this cpp file to your project, you mustn't include it in a file where you've
    already included any other headers - just put it inside a file on its own, possibly with your config
    flags preceding it, but don't include anything else. That also includes avoiding any automatic prefix
    header files that the compiler may be using.
 */
 #error "Incorrect use of HIGGS cpp file"
#endif

#define HIGGS_CORE_INCLUDE_NATIVE_HEADERS 1
#define HIGGS_CORE_INCLUDE_OBJC_HELPERS 1
#define HIGGS_GUI_BASICS_INCLUDE_XHEADERS 1
#define HIGGS_GUI_BASICS_INCLUDE_SCOPED_THREAD_DPI_AWARENESS_SETTER 1
#define HIGGS_GRAPHICS_INCLUDE_COREGRAPHICS_HELPERS 1
#define HIGGS_VST3HEADERS_INCLUDE_HEADERS_ONLY 1

#include "higgs_audio_processors.h"
#include <higgs_gui_extra/higgs_gui_extra.h>

//==============================================================================
#if HIGGS_INTERNAL_HAS_VST && (HIGGS_LINUX || HIGGS_BSD)
 HIGGS_BEGIN_IGNORE_WARNINGS_GCC_LIKE ("-Wvariadic-macros")
 #include <X11/Xlib.h>
 HIGGS_END_IGNORE_WARNINGS_GCC_LIKE
 #include <X11/Xutil.h>
 #undef KeyPress
#endif

#if HIGGS_INTERNAL_HAS_AU
 #include <AudioUnit/AudioUnit.h>
#endif

#include "format/higgs_AudioPluginFormatManagerHelpers.cpp"
#include "processors/higgs_AudioProcessorEditor.cpp"
#include "processors/higgs_GenericAudioProcessorEditor.cpp"
#include "format_types/higgs_VSTPluginFormat.cpp"
#include "format_types/higgs_VST3PluginFormat.cpp"
#include "format_types/higgs_AudioUnitPluginFormat.mm"
#include "scanning/higgs_KnownPluginList.cpp"
#include "scanning/higgs_PluginDirectoryScanner.cpp"
#include "scanning/higgs_PluginListComponent.cpp"
#include "utilities/higgs_ParameterAttachments.cpp"
#include "utilities/higgs_AudioProcessorValueTreeState.cpp"
#include "utilities/higgs_PluginHostType.cpp"

#include "format_types/higgs_LV2PluginFormat.cpp"

#if HIGGS_UNIT_TESTS
 #if HIGGS_INTERNAL_HAS_VST3
  #include "format_types/higgs_VST3PluginFormat_test.cpp"
 #endif

 #if HIGGS_INTERNAL_HAS_AU
  #include "format_types/higgs_AudioUnitPluginFormat_test.cpp"
 #endif

 #if HIGGS_INTERNAL_HAS_LV2
  #include "format_types/higgs_LV2PluginFormat_test.cpp"
 #endif
#endif
