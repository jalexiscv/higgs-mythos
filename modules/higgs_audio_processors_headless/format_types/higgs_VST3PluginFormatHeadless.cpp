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

#if HIGGS_INTERNAL_HAS_VST3

#include <higgs_audio_processors_headless/format_types/higgs_VST3PluginFormatImpl.h>

namespace higgs
{

// UB Sanitizer doesn't necessarily have instrumentation for loaded plugins, so
// it won't recognize the dynamic types of pointers to the plugin's interfaces.
HIGGS_BEGIN_NO_SANITIZE ("vptr")

void VST3PluginFormatHeadless::findAllTypesForFile (OwnedArray<PluginDescription>& results, const String& fileOrIdentifier)
{
    if (! fileMightContainThisPluginType (fileOrIdentifier))
        return;

    if (const auto fast = DescriptionLister::findDescriptionsFast (File (fileOrIdentifier)); ! fast.empty())
    {
        for (const auto& d : fast)
            results.add (new PluginDescription (d));

        return;
    }

    for (const auto& file : getLibraryPaths (*this, fileOrIdentifier))
    {
        /**
            Since there is no apparent indication if a VST3 plugin is a shell or not,
            we're stuck iterating through a VST3's factory, creating a description
            for every housed plugin.
        */

        auto handle = RefCountedDllHandle::getHandle (file);

        if (handle == nullptr)
            continue;

        auto pluginFactory = handle->getPluginFactory();

        if (pluginFactory == nullptr)
            continue;

        VSTComSmartPtr host { new VST3HostContextHeadless(), IncrementRef::no };

        for (const auto& d : DescriptionLister::findDescriptionsSlow (*host, *pluginFactory, File (file)))
            results.add (new PluginDescription (d));
    }
}

void VST3PluginFormatHeadless::createARAFactoryAsync (const PluginDescription& description, ARAFactoryCreationCallback callback)
{
    if (! description.hasARAExtension)
    {
        jassertfalse;
        callback ({ {}, "The provided plugin does not support ARA features" });
    }

    const File file (description.fileOrIdentifier);
    auto handle = RefCountedDllHandle::getHandle (file.getFullPathName());
    auto pluginFactory = handle->getPluginFactory();
    const auto* pluginName = description.name.toRawUTF8();

    callback ({ ARAFactoryWrapper { ::higgs::getARAFactory (pluginFactory.get(), pluginName) }, {} });
}

void VST3PluginFormatHeadless::createPluginInstance (const PluginDescription& description,
                                                     double,
                                                     int,
                                                     PluginCreationCallback callback)
{
    createVst3InstanceImpl<VST3PluginInstanceHeadless> (*this,
                                                        { new VST3HostContextHeadless(), IncrementRef::no },
                                                        description,
                                                        callback);
}

bool VST3PluginFormatHeadless::requiresUnblockedMessageThreadDuringCreation (const PluginDescription&) const
{
    return false;
}

bool VST3PluginFormatHeadless::fileMightContainThisPluginType (const String& fileOrIdentifier)
{
    auto f = File::createFileWithoutCheckingPath (fileOrIdentifier);

    return f.hasFileExtension (".vst3") && f.exists();
}

String VST3PluginFormatHeadless::getNameOfPluginFromIdentifier (const String& fileOrIdentifier)
{
    return fileOrIdentifier; //Impossible to tell because every VST3 is a type of shell...
}

bool VST3PluginFormatHeadless::pluginNeedsRescanning (const PluginDescription& description)
{
    return File (description.fileOrIdentifier).getLastModificationTime() != description.lastFileModTime;
}

bool VST3PluginFormatHeadless::doesPluginStillExist (const PluginDescription& description)
{
    return File (description.fileOrIdentifier).exists();
}

StringArray VST3PluginFormatHeadless::searchPathsForPlugins (const FileSearchPath& directoriesToSearch, const bool recursive, bool)
{
    StringArray results;

    for (int i = 0; i < directoriesToSearch.getNumPaths(); ++i)
        recursiveFileSearch (*this, results, directoriesToSearch[i], recursive);

    return results;
}

FileSearchPath VST3PluginFormatHeadless::getDefaultLocationsToSearch()
{
    auto defaults = std::invoke ([]() -> FileSearchPath
    {
      #if HIGGS_WINDOWS
        const auto localAppData = File::getSpecialLocation (File::windowsLocalAppData).getFullPathName();
        const auto programFiles = File::getSpecialLocation (File::windowsProgramFilesCommon).getFullPathName();
        return { localAppData + "\\Programs\\Common\\VST3;" + programFiles + "\\VST3" };
      #elif HIGGS_MAC
        return { "~/Library/Audio/Plug-Ins/VST3;/Library/Audio/Plug-Ins/VST3" };
      #else
       #if HIGGS_64BIT
        if (File ("/usr/lib64/vst3").exists() || File ("/usr/local/lib64/vst3").exists())
            return { "~/.vst3/;/usr/local/lib64/vst3/;/usr/lib64/vst3/" };
       #endif

        return { "~/.vst3/;/usr/local/lib/vst3/;/usr/lib/vst3/" };
      #endif
    });

    FileSearchPath result { SystemStats::getEnvironmentVariable ("VST3_PATH", "").replace (":", ";") };
    result.addPath (defaults);
    return result;
}

HIGGS_END_NO_SANITIZE

} // namespace higgs

#endif
