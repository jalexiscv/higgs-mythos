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

#if HIGGS_INTERNAL_HAS_LV2

#include <higgs_audio_processors_headless/format_types/higgs_LV2PluginFormatImpl.h>

namespace higgs
{

//==============================================================================
LV2PluginFormatHeadless::LV2PluginFormatHeadless()
    : pimpl (std::make_unique<Pimpl>()) {}

LV2PluginFormatHeadless::~LV2PluginFormatHeadless() = default;

void LV2PluginFormatHeadless::findAllTypesForFile (OwnedArray<PluginDescription>& results,
                                                   const String& fileOrIdentifier)
{
    pimpl->findAllTypesForFile (results, fileOrIdentifier);
}

bool LV2PluginFormatHeadless::fileMightContainThisPluginType (const String& fileOrIdentifier)
{
    return pimpl->fileMightContainThisPluginType (fileOrIdentifier);
}

String LV2PluginFormatHeadless::getNameOfPluginFromIdentifier (const String& fileOrIdentifier)
{
    return pimpl->getNameOfPluginFromIdentifier (fileOrIdentifier);
}

bool LV2PluginFormatHeadless::pluginNeedsRescanning (const PluginDescription& desc)
{
    return pimpl->pluginNeedsRescanning (desc);
}

bool LV2PluginFormatHeadless::doesPluginStillExist (const PluginDescription& desc)
{
    return pimpl->doesPluginStillExist (desc);
}

bool LV2PluginFormatHeadless::canScanForPlugins() const { return true; }
bool LV2PluginFormatHeadless::isTrivialToScan() const { return true; }

StringArray LV2PluginFormatHeadless::searchPathsForPlugins (const FileSearchPath& directoriesToSearch,
                                                            bool recursive,
                                                            bool)
{
    return pimpl->searchPathsForPlugins (directoriesToSearch,
                                         recursive ? Pimpl::Recursive::yes : Pimpl::Recursive::no);
}

FileSearchPath LV2PluginFormatHeadless::getDefaultLocationsToSearch()
{
    return pimpl->getDefaultLocationsToSearch();
}

bool LV2PluginFormatHeadless::requiresUnblockedMessageThreadDuringCreation (const PluginDescription&) const
{
    return false;
}

void LV2PluginFormatHeadless::createPluginInstance (const PluginDescription& desc,
                                                    double sampleRate,
                                                    int bufferSize,
                                                    PluginCreationCallback callback)
{
    Pimpl::createPluginInstance<lv2_host::LV2AudioPluginInstanceHeadless> (*this, desc, sampleRate, bufferSize, std::move (callback));
}

} // namespace higgs

#endif
