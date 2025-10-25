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

#include "PluginGraph.h"


//==============================================================================
/**
    Manages the internal plugin types.
*/
class InternalPluginFormat final : public AudioPluginFormat
{
public:
    //==============================================================================
    InternalPluginFormat();

    //==============================================================================
    const std::vector<PluginDescription>& getAllTypes() const;

    //==============================================================================
    static String getIdentifier()                                                       { return "Internal"; }
    String getName() const override                                                     { return getIdentifier(); }
    bool fileMightContainThisPluginType (const String&) override                        { return true; }
    FileSearchPath getDefaultLocationsToSearch() override                               { return {}; }
    bool canScanForPlugins() const override                                             { return false; }
    bool isTrivialToScan() const override                                               { return true; }
    void findAllTypesForFile (OwnedArray<PluginDescription>&, const String&) override   {}
    bool doesPluginStillExist (const PluginDescription&) override                       { return true; }
    String getNameOfPluginFromIdentifier (const String& fileOrIdentifier) override      { return fileOrIdentifier; }
    bool pluginNeedsRescanning (const PluginDescription&) override                      { return false; }
    StringArray searchPathsForPlugins (const FileSearchPath&, bool, bool) override      { return {}; }

private:
    class InternalPluginFactory
    {
    public:
        using Constructor = std::function<std::unique_ptr<AudioPluginInstance>()>;

        explicit InternalPluginFactory (const std::initializer_list<Constructor>& constructorsIn);

        const std::vector<PluginDescription>& getDescriptions() const       { return descriptions; }

        std::unique_ptr<AudioPluginInstance> createInstance (const String& name) const;

    private:
        const std::vector<Constructor> constructors;
        const std::vector<PluginDescription> descriptions;
    };

    //==============================================================================
    void createPluginInstance (const PluginDescription&,
                               double initialSampleRate, int initialBufferSize,
                               PluginCreationCallback) override;

    std::unique_ptr<AudioPluginInstance> createInstance (const String& name);

    bool requiresUnblockedMessageThreadDuringCreation (const PluginDescription&) const override;

    InternalPluginFactory factory;
};
