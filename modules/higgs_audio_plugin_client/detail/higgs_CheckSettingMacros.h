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

// The following checks should cause a compile error if you've forgotten to
// define all your plugin settings properly.

#if ! (HiggsPlugin_Build_VST || HiggsPlugin_Build_VST3 \
        || HiggsPlugin_Build_AU  || HiggsPlugin_Build_AUv3 \
        || HiggsPlugin_Build_AAX || HiggsPlugin_Build_Standalone \
        || HiggsPlugin_Build_LV2 || HiggsPlugin_Build_Unity)
 #error "You need to enable at least one plugin format!"
#endif

#ifdef HIGGS_CHECKSETTINGMACROS_H
 #error "This header should never be included twice! Otherwise something is wrong."
#endif
#define HIGGS_CHECKSETTINGMACROS_H

#ifndef HiggsPlugin_IsSynth
 #error "You need to define the HiggsPlugin_IsSynth value!"
#endif

#ifndef HiggsPlugin_ManufacturerCode
 #error "You need to define the HiggsPlugin_ManufacturerCode value!"
#endif

#ifndef HiggsPlugin_PluginCode
 #error "You need to define the HiggsPlugin_PluginCode value!"
#endif

#ifndef HiggsPlugin_ProducesMidiOutput
 #error "You need to define the HiggsPlugin_ProducesMidiOutput value!"
#endif

#ifndef HiggsPlugin_WantsMidiInput
 #error "You need to define the HiggsPlugin_WantsMidiInput value!"
#endif

#ifdef HiggsPlugin_Latency
 #error "HiggsPlugin_Latency is now deprecated - instead, call the AudioProcessor::setLatencySamples() method if your plugin has a non-zero delay"
#endif

#ifndef HiggsPlugin_EditorRequiresKeyboardFocus
 #error "You need to define the HiggsPlugin_EditorRequiresKeyboardFocus value!"
#endif

//==============================================================================
#if HiggsPlugin_Build_AAX && ! defined (HiggsPlugin_AAXIdentifier)
 #error "You need to define the HiggsPlugin_AAXIdentifier value!"
#endif

#if defined (__ppc__)
 #undef HiggsPlugin_Build_AAX
 #define HiggsPlugin_Build_AAX 0
#endif
