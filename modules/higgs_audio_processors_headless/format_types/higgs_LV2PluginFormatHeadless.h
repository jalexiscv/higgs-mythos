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

#if HIGGS_INTERNAL_HAS_LV2 || DOXYGEN

/**
    Implements a plugin format for LV2 plugins.

    @tags{Audio}
*/
class HIGGS_API LV2PluginFormatHeadless  : public AudioPluginFormat
{
public:
    LV2PluginFormatHeadless();
    ~LV2PluginFormatHeadless() override;

    static String getFormatName()       { return "LV2"; }
    String getName() const override     { return getFormatName(); }

    void findAllTypesForFile (OwnedArray<PluginDescription>& results,
                              const String& fileOrIdentifier) override;

    bool fileMightContainThisPluginType (const String& fileOrIdentifier) override;

    String getNameOfPluginFromIdentifier (const String& fileOrIdentifier) override;

    bool pluginNeedsRescanning (const PluginDescription&) override;

    bool doesPluginStillExist (const PluginDescription&) override;

    bool canScanForPlugins() const override;

    bool isTrivialToScan() const override;

    StringArray searchPathsForPlugins (const FileSearchPath& directoriesToSearch,
                                       bool recursive,
                                       bool allowPluginsWhichRequireAsynchronousInstantiation = false) override;

    FileSearchPath getDefaultLocationsToSearch() override;

    /** @internal */
    class Pimpl;

private:
    bool requiresUnblockedMessageThreadDuringCreation (const PluginDescription&) const override;
    void createPluginInstance (const PluginDescription&, double, int, PluginCreationCallback) override;

    std::unique_ptr<Pimpl> pimpl;

    HIGGS_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (LV2PluginFormatHeadless)
};

#endif

} // namespace higgs
