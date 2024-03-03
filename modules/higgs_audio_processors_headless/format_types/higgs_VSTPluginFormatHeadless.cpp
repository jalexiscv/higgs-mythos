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

#if HIGGS_INTERNAL_HAS_VST

#include <higgs_audio_processors_headless/format_types/higgs_VSTPluginFormatImpl.h>
#include <higgs_audio_processors_headless/utilities/higgs_CommonProcessorUtilities.h>

namespace higgs
{

void VSTPluginFormatHeadless::findAllTypesForFile (OwnedArray<PluginDescription>& results,
                                                   const String& fileOrIdentifier)
{
    if (! fileMightContainThisPluginType (fileOrIdentifier))
        return;

    PluginDescription desc;
    desc.fileOrIdentifier = fileOrIdentifier;
    desc.uniqueId = desc.deprecatedUid = 0;

    auto instance = createAndUpdateDesc (*this, desc);

    if (instance == nullptr)
        return;

    if (instance->getVstCategory() != Vst2::kPlugCategShell)
    {
        // Normal plugin...
        results.add (new PluginDescription (desc));

        instance->dispatch (Vst2::effOpen, 0, 0, nullptr, 0);
    }
    else
    {
        // It's a shell plugin, so iterate all the subtypes...
        for (;;)
        {
            char shellEffectName [256] = { 0 };
            auto uid = (int) instance->dispatch (Vst2::effShellGetNextPlugin, 0, 0, shellEffectName, 0);

            if (uid == 0)
                break;

            desc.uniqueId = desc.deprecatedUid = uid;
            desc.name = shellEffectName;

            aboutToScanVSTShellPlugin (desc);

            std::unique_ptr<VSTPluginInstanceHeadless> shellInstance (createAndUpdateDesc (*this, desc));

            if (shellInstance != nullptr)
            {
                jassert (desc.deprecatedUid == uid);
                jassert (desc.uniqueId == uid);
                desc.hasSharedContainer = true;
                desc.name = shellEffectName;

                if (! arrayContainsPlugin (results, desc))
                    results.add (new PluginDescription (desc));
            }
        }
    }
}

void VSTPluginFormatHeadless::createPluginInstance (const PluginDescription& desc,
                                                    double sampleRate,
                                                    int blockSize,
                                                    PluginCreationCallback callback)
{
    createVstPluginInstance<VSTPluginInstanceHeadless> (*this, desc, sampleRate, blockSize, callback);
}

bool VSTPluginFormatHeadless::requiresUnblockedMessageThreadDuringCreation (const PluginDescription&) const
{
    return false;
}

bool VSTPluginFormatHeadless::fileMightContainThisPluginType (const String& fileOrIdentifier)
{
    auto f = File::createFileWithoutCheckingPath (fileOrIdentifier);

  #if HIGGS_MAC || HIGGS_IOS
    return f.isDirectory() && f.hasFileExtension (".vst");
  #elif HIGGS_WINDOWS
    return f.existsAsFile() && f.hasFileExtension (".dll");
  #elif HIGGS_LINUX || HIGGS_BSD || HIGGS_ANDROID
    return f.existsAsFile() && f.hasFileExtension (".so");
  #endif
}

String VSTPluginFormatHeadless::getNameOfPluginFromIdentifier (const String& fileOrIdentifier)
{
    return fileOrIdentifier;
}

bool VSTPluginFormatHeadless::pluginNeedsRescanning (const PluginDescription& desc)
{
    return File (desc.fileOrIdentifier).getLastModificationTime() != desc.lastFileModTime;
}

bool VSTPluginFormatHeadless::doesPluginStillExist (const PluginDescription& desc)
{
    return File (desc.fileOrIdentifier).exists();
}

StringArray VSTPluginFormatHeadless::searchPathsForPlugins (const FileSearchPath& directoriesToSearch, const bool recursive, bool)
{
    StringArray results;

    for (int j = 0; j < directoriesToSearch.getNumPaths(); ++j)
        recursiveFileSearch (results, directoriesToSearch [j], recursive);

    return results;
}

void VSTPluginFormatHeadless::recursiveFileSearch (StringArray& results, const File& dir, const bool recursive)
{
    // avoid allowing the dir iterator to be recursive, because we want to avoid letting it delve inside
    // .component or .vst directories.
    for (const auto& iter : RangedDirectoryIterator (dir, false, "*", File::findFilesAndDirectories))
    {
        auto f = iter.getFile();
        bool isPlugin = false;

        if (fileMightContainThisPluginType (f.getFullPathName()))
        {
            isPlugin = true;
            results.add (f.getFullPathName());
        }

        if (recursive && (! isPlugin) && f.isDirectory())
            recursiveFileSearch (results, f, true);
    }
}

FileSearchPath VSTPluginFormatHeadless::getDefaultLocationsToSearch()
{
    auto defaults = std::invoke ([]() -> FileSearchPath
    {
       #if HIGGS_MAC
        return { "~/Library/Audio/Plug-Ins/VST;/Library/Audio/Plug-Ins/VST" };
       #elif HIGGS_LINUX || HIGGS_BSD || HIGGS_ANDROID
        return { "/usr/lib/vst;/usr/local/lib/vst;~/.vst" };
       #elif HIGGS_WINDOWS
        auto programFiles = File::getSpecialLocation (File::globalApplicationsDirectory).getFullPathName();

        FileSearchPath paths;
        paths.add (WindowsRegistry::getValue ("HKEY_LOCAL_MACHINE\\Software\\VST\\VSTPluginsPath"));
        paths.addIfNotAlreadyThere (programFiles + "\\Steinberg\\VstPlugins");
        paths.addIfNotAlreadyThere (programFiles + "\\VstPlugins");
        paths.removeRedundantPaths();
        return paths;
       #elif HIGGS_IOS
        // on iOS you can only load plug-ins inside the hosts bundle folder
        CFUniquePtr<CFURLRef> relativePluginDir (CFBundleCopyBuiltInPlugInsURL (CFBundleGetMainBundle()));
        CFUniquePtr<CFURLRef> pluginDir (CFURLCopyAbsoluteURL (relativePluginDir.get()));

        CFUniquePtr<CFStringRef> path (CFURLCopyFileSystemPath (pluginDir.get(), kCFURLPOSIXPathStyle));
        return { String (CFStringGetCStringPtr (path.get(), kCFStringEncodingUTF8)) };
       #endif
    });

    FileSearchPath result { SystemStats::getEnvironmentVariable ("VST_PATH", "").replace (":", ";") };
    result.addPath (defaults);
    return result;
}

AudioPluginInstance* VSTPluginFormatHeadless::createCustomVSTFromMainCall (void* entryPointFunction,
                                                                           double initialSampleRate,
                                                                           int initialBufferSize)
{
    return createCustomVSTFromMainCallImpl<VSTPluginInstanceHeadless> (entryPointFunction, initialSampleRate, initialBufferSize).release();
}

AudioPluginInstance* VSTPluginFormatHeadless::getPluginInstanceFromVstEffectInterface (void* aEffect)
{
    if (auto* vstAEffect = reinterpret_cast<Vst2::AEffect*> (aEffect))
        if (auto* instanceVST = reinterpret_cast<VSTPluginInstanceHeadless*> (vstAEffect->resvd2))
            return dynamic_cast<AudioPluginInstance*> (instanceVST);

    return nullptr;
}

void VSTPluginFormatHeadless::aboutToScanVSTShellPlugin (const PluginDescription&) {}

} // namespace higgs

#endif
