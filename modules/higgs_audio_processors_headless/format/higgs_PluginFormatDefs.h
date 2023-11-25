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

#pragma once

// The following definitions are PRIVATE and should not be queried or modified
// by user code. These are exclusively used to enable and disable HIGGS
// implementation details.

#if defined (HIGGS_INTERNAL_HAS_VST) \
 || defined (HIGGS_INTERNAL_HAS_VST3) \
 || defined (HIGGS_INTERNAL_HAS_AU) \
 || defined (HIGGS_INTERNAL_HAS_LADSPA) \
 || defined (HIGGS_INTERNAL_HAS_LV2)
 #error These preprocessor definitions should not be set by the build system. Use the HIGGS_PLUGINHOST_* definitions instead.
#endif

#if HIGGS_PLUGINHOST_VST && (HIGGS_MAC || HIGGS_WINDOWS || HIGGS_LINUX || HIGGS_BSD || HIGGS_IOS)
 #define HIGGS_INTERNAL_HAS_VST 1
#else
 #define HIGGS_INTERNAL_HAS_VST 0
#endif

#if HIGGS_PLUGINHOST_VST3 && (HIGGS_MAC || HIGGS_WINDOWS || HIGGS_LINUX || HIGGS_BSD)
 #define HIGGS_INTERNAL_HAS_VST3 1
#else
 #define HIGGS_INTERNAL_HAS_VST3 0
#endif

#if HIGGS_PLUGINHOST_AU && (HIGGS_MAC || HIGGS_IOS)
 #define HIGGS_INTERNAL_HAS_AU 1
#else
 #define HIGGS_INTERNAL_HAS_AU 0
#endif

#if HIGGS_PLUGINHOST_LADSPA && (HIGGS_LINUX || HIGGS_BSD)
 #define HIGGS_INTERNAL_HAS_LADSPA 1
#else
 #define HIGGS_INTERNAL_HAS_LADSPA 0
#endif

#if HIGGS_PLUGINHOST_LV2 && (HIGGS_MAC || HIGGS_LINUX || HIGGS_BSD || HIGGS_WINDOWS)
 #define HIGGS_INTERNAL_HAS_LV2 1
#else
 #define HIGGS_INTERNAL_HAS_LV2 0
#endif

#if HIGGS_PLUGINHOST_ARA && (HIGGS_INTERNAL_HAS_VST3 || HIGGS_INTERNAL_HAS_AU) && (HIGGS_MAC || HIGGS_WINDOWS || HIGGS_LINUX)
 #define HIGGS_INTERNAL_HAS_ARA 1
#else
 #define HIGGS_INTERNAL_HAS_ARA 0
#endif
