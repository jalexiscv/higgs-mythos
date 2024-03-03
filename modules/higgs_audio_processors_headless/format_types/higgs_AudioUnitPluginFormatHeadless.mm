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

#if HIGGS_INTERNAL_HAS_AU

#include <higgs_audio_processors_headless/format_types/higgs_AudioUnitPluginFormatImpl.h>

namespace higgs
{

void AudioUnitPluginFormatHeadless::findAllTypesForFile (OwnedArray<PluginDescription>& results,
                                                         const String& fileOrIdentifier)
{
    if (! fileMightContainThisPluginType (fileOrIdentifier))
        return;

    PluginDescription desc;
    desc.fileOrIdentifier = fileOrIdentifier;
    desc.uniqueId = desc.deprecatedUid = 0;

    if (MessageManager::getInstance()->isThisTheMessageThread()
          && requiresUnblockedMessageThreadDuringCreation (desc))
        return;

    try
    {
        auto createdInstance = createInstanceFromDescription (desc, 44100.0, 512);

        if (auto auInstance = dynamic_cast<AudioUnitPluginInstanceHeadless*> (createdInstance.get()))
            results.add (new PluginDescription (auInstance->getPluginDescription()));
    }
    catch (...)
    {
        // crashed while loading...
    }
}

void AudioUnitPluginFormatHeadless::createPluginInstance (const PluginDescription& desc,
                                                          double rate,
                                                          int blockSize,
                                                          PluginCreationCallback callback)
{
    createAudioUnitPluginInstance<AudioUnitPluginInstanceHeadless> (*this, desc, rate, blockSize, callback);
}

void AudioUnitPluginFormatHeadless::createARAFactoryAsync (const PluginDescription& desc, ARAFactoryCreationCallback callback)
{
    auto auComponentResult = getAudioComponent (*this, desc);

    if (! auComponentResult.isValid())
    {
        callback ({ {}, "Failed to create AudioComponent for " + desc.descriptiveName });
        return;
    }

    getOrCreateARAAudioUnit (auComponentResult.component, [cb = std::move (callback)] (auto dylibKeepAliveAudioUnit)
    {
        cb ([&]() -> ARAFactoryResult
            {
                if (dylibKeepAliveAudioUnit != nullptr)
                    return { ARAFactoryWrapper { ::higgs::getARAFactory (std::move (dylibKeepAliveAudioUnit)) }, "" };

                return { {}, "Failed to create ARAFactory from the provided AudioUnit" };
            }());
    });
}

bool AudioUnitPluginFormatHeadless::requiresUnblockedMessageThreadDuringCreation (const PluginDescription& desc) const
{
    String pluginName, version, manufacturer;
    AudioComponentDescription componentDesc;

    if (AudioUnitFormatHelpers::getComponentDescFromIdentifier (desc.fileOrIdentifier, componentDesc,
                                                                pluginName, version, manufacturer)
           || AudioUnitFormatHelpers::getComponentDescFromFile (desc.fileOrIdentifier, componentDesc,
                                                                pluginName, version, manufacturer))
    {
        if (AudioComponent auComp = AudioComponentFindNext (nullptr, &componentDesc))
        {
            if (AudioComponentGetDescription (auComp, &componentDesc) == noErr)
                return AudioUnitFormatHelpers::isPluginAUv3 (componentDesc);
        }
    }

    return false;
}

StringArray AudioUnitPluginFormatHeadless::searchPathsForPlugins (const FileSearchPath&, bool /*recursive*/, bool allowPluginsWhichRequireAsynchronousInstantiation)
{
    StringArray result;
    AudioComponent comp = nullptr;

    for (;;)
    {
        AudioComponentDescription desc;
        zerostruct (desc);

        comp = AudioComponentFindNext (comp, &desc);

        if (comp == nullptr)
            break;

        if (AudioComponentGetDescription (comp, &desc) != noErr)
            continue;

        if (desc.componentType == kAudioUnitType_MusicDevice
             || desc.componentType == kAudioUnitType_MusicEffect
             || desc.componentType == kAudioUnitType_Effect
             || desc.componentType == kAudioUnitType_Generator
             || desc.componentType == kAudioUnitType_Panner
             || desc.componentType == kAudioUnitType_Mixer
             || desc.componentType == kAudioUnitType_MIDIProcessor)
        {
            if (allowPluginsWhichRequireAsynchronousInstantiation || ! AudioUnitFormatHelpers::isPluginAUv3 (desc))
                result.add (AudioUnitFormatHelpers::createPluginIdentifier (desc));
        }
    }

    return result;
}

bool AudioUnitPluginFormatHeadless::fileMightContainThisPluginType (const String& fileOrIdentifier)
{
    AudioComponentDescription desc;
    String name, version, manufacturer;

    if (AudioUnitFormatHelpers::getComponentDescFromIdentifier (fileOrIdentifier, desc, name, version, manufacturer))
        return AudioComponentFindNext (nullptr, &desc) != nullptr;

    auto f = File::createFileWithoutCheckingPath (fileOrIdentifier);

    return (f.hasFileExtension (".component") || f.hasFileExtension (".appex"))
             && f.isDirectory();
}

String AudioUnitPluginFormatHeadless::getNameOfPluginFromIdentifier (const String& fileOrIdentifier)
{
    AudioComponentDescription desc;
    String name, version, manufacturer;
    AudioUnitFormatHelpers::getComponentDescFromIdentifier (fileOrIdentifier, desc, name, version, manufacturer);

    if (name.isEmpty())
        name = fileOrIdentifier;

    return name;
}

bool AudioUnitPluginFormatHeadless::pluginNeedsRescanning (const PluginDescription& desc)
{
    AudioComponentDescription newDesc;
    String name, version, manufacturer;

    return ! (AudioUnitFormatHelpers::getComponentDescFromIdentifier (desc.fileOrIdentifier, newDesc,
                                                                      name, version, manufacturer)
               && version == desc.version);
}

bool AudioUnitPluginFormatHeadless::doesPluginStillExist (const PluginDescription& desc)
{
    if (desc.fileOrIdentifier.startsWithIgnoreCase (AudioUnitFormatHelpers::auIdentifierPrefix))
        return fileMightContainThisPluginType (desc.fileOrIdentifier);

    return File (desc.fileOrIdentifier).exists();
}

FileSearchPath AudioUnitPluginFormatHeadless::getDefaultLocationsToSearch()
{
    return {};
}

} // namespace higgs

#endif
