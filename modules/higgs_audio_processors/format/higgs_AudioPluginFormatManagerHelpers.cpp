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

namespace higgs
{

void addDefaultFormatsToManager ([[maybe_unused]] AudioPluginFormatManager& manager)
{
   #if HIGGS_INTERNAL_HAS_AU
    manager.addFormat (std::make_unique<AudioUnitPluginFormat>());
   #endif

   #if HIGGS_INTERNAL_HAS_VST
    manager.addFormat (std::make_unique<VSTPluginFormat>());
   #endif

   #if HIGGS_INTERNAL_HAS_VST3
    manager.addFormat (std::make_unique<VST3PluginFormat>());
   #endif

   #if HIGGS_INTERNAL_HAS_LADSPA
    manager.addFormat (std::make_unique<LADSPAPluginFormat>());
   #endif

   #if HIGGS_INTERNAL_HAS_LV2
    manager.addFormat (std::make_unique<LV2PluginFormat>());
   #endif
}

} // namespace higgs
