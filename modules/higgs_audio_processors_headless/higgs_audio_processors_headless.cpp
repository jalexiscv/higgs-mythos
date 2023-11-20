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

#ifdef HIGGS_AUDIO_PROCESSORS_HEADLESS_H_INCLUDED
 /* When you add this cpp file to your project, you mustn't include it in a file where you've
    already included any other headers - just put it inside a file on its own, possibly with your config
    flags preceding it, but don't include anything else. That also includes avoiding any automatic prefix
    header files that the compiler may be using.
 */
 #error "Incorrect use of HIGGS cpp file"
#endif

#define HIGGS_CORE_INCLUDE_NATIVE_HEADERS 1
#define HIGGS_CORE_INCLUDE_OBJC_HELPERS 1

#include "higgs_audio_processors_headless.h"

#include <higgs_audio_processors_headless/processors/higgs_AudioProcessorListener.cpp>
#include <higgs_audio_processors_headless/utilities/higgs_AAXClientExtensions.cpp>
#include <higgs_audio_processors_headless/utilities/higgs_VST2ClientExtensions.cpp>
#include <higgs_audio_processors_headless/utilities/higgs_VST3ClientExtensions.cpp>
#include <higgs_audio_processors_headless/processors/higgs_AudioProcessorParameter.cpp>
#include <higgs_audio_processors_headless/processors/higgs_AudioProcessorParameterGroup.cpp>
#include <higgs_audio_processors_headless/processors/higgs_AudioProcessor.cpp>
#include <higgs_audio_processors_headless/processors/higgs_PluginDescription.cpp>
#include <higgs_audio_processors_headless/processors/higgs_AudioPluginInstance.cpp>
#include <higgs_audio_processors_headless/processors/higgs_AudioProcessorGraph.cpp>
#include <higgs_audio_processors_headless/format/higgs_AudioPluginFormat.cpp>
#include <higgs_audio_processors_headless/utilities/higgs_AudioProcessorParameterWithID.cpp>
#include <higgs_audio_processors_headless/utilities/higgs_RangedAudioParameter.cpp>
#include <higgs_audio_processors_headless/utilities/higgs_AudioParameterFloat.cpp>
#include <higgs_audio_processors_headless/utilities/higgs_AudioParameterInt.cpp>
#include <higgs_audio_processors_headless/utilities/higgs_AudioParameterBool.cpp>
#include <higgs_audio_processors_headless/utilities/higgs_AudioParameterChoice.cpp>
#include <higgs_audio_processors_headless/utilities/ARA/higgs_ARA_utils.cpp>
#include <higgs_audio_processors_headless/format_types/higgs_AudioUnitPluginFormatHeadless.mm>
#include <higgs_audio_processors_headless/format_types/higgs_LADSPAPluginFormatHeadless.cpp>
#include <higgs_audio_processors_headless/format_types/higgs_LV2PluginFormatHeadless.cpp>
#include <higgs_audio_processors_headless/format_types/higgs_VST3PluginFormatHeadless.cpp>
#include <higgs_audio_processors_headless/format_types/higgs_VSTPluginFormatHeadless.cpp>
#include <higgs_audio_processors_headless/format_types/higgs_ARAHosting.cpp>
#include <higgs_audio_processors_headless/format/higgs_AudioPluginFormatManager.cpp>
