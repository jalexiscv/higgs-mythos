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

#if HiggsPlugin_Build_AU

#include <higgs_core/system/higgs_CompilerWarnings.h>

HIGGS_BEGIN_IGNORE_WARNINGS_GCC_LIKE ("-Wambiguous-reversed-operator",
                                     "-Wc99-extensions",
                                     "-Wcast-align",
                                     "-Wcomment",
                                     "-Wconversion",
                                     "-Wdeprecated-anon-enum-enum-conversion",
                                     "-Wextra-semi",
                                     "-Wextra-tokens",
                                     "-Wfloat-equal",
                                     "-Wformat-pedantic",
                                     "-Wfour-char-constants",
                                     "-Wgnu-zero-variadic-macro-arguments",
                                     "-Wignored-qualifiers",
                                     "-Wimplicit-fallthrough",
                                     "-Wmissing-prototypes",
                                     "-Wmissing-designated-field-initializers",
                                     "-Wnullable-to-nonnull-conversion",
                                     "-Wparentheses",
                                     "-Wshadow-all",
                                     "-Wswitch-enum",
                                     "-Wunknown-attributes",
                                     "-Wunused",
                                     "-Wunused-parameter",
                                     "-Wzero-as-null-pointer-constant")

// From MacOS 10.13 and iOS 11 Apple has (sensibly!) stopped defining a whole
// set of functions with rather generic names. However, we still need a couple
// of them to compile the files below.
#ifndef verify
 #define verify(assertion) __Verify(assertion)
#endif
#ifndef verify_noerr
 #define verify_noerr(errorCode)  __Verify_noErr(errorCode)
#endif

#include <higgs_audio_plugin_client/AU/AudioUnitSDK/AUBase.cpp>
#include <higgs_audio_plugin_client/AU/AudioUnitSDK/AUBuffer.cpp>
#include <higgs_audio_plugin_client/AU/AudioUnitSDK/AUBufferAllocator.cpp>
#include <higgs_audio_plugin_client/AU/AudioUnitSDK/AUEffectBase.cpp>
#include <higgs_audio_plugin_client/AU/AudioUnitSDK/AUInputElement.cpp>
#include <higgs_audio_plugin_client/AU/AudioUnitSDK/AUMIDIBase.cpp>
#include <higgs_audio_plugin_client/AU/AudioUnitSDK/AUMIDIEffectBase.cpp>
#include <higgs_audio_plugin_client/AU/AudioUnitSDK/AUOutputElement.cpp>
#include <higgs_audio_plugin_client/AU/AudioUnitSDK/AUPlugInDispatch.cpp>
#include <higgs_audio_plugin_client/AU/AudioUnitSDK/AUScopeElement.cpp>
#include <higgs_audio_plugin_client/AU/AudioUnitSDK/ComponentBase.cpp>
#include <higgs_audio_plugin_client/AU/AudioUnitSDK/MusicDeviceBase.cpp>

#undef verify
#undef verify_noerr

HIGGS_END_IGNORE_WARNINGS_GCC_LIKE

#endif
