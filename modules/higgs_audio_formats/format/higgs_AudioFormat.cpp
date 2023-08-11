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

AudioFormat::AudioFormat (String name, StringArray extensions)
   : formatName (name), fileExtensions (extensions)
{
}

AudioFormat::AudioFormat (StringRef name, StringRef extensions)
   : formatName (name.text), fileExtensions (StringArray::fromTokens (extensions, false))
{
}

AudioFormat::~AudioFormat()
{
}

bool AudioFormat::canHandleFile (const File& f)
{
    for (auto& e : getFileExtensions())
        if (f.hasFileExtension (e))
            return true;

    return false;
}

const String& AudioFormat::getFormatName() const                { return formatName; }
StringArray AudioFormat::getFileExtensions() const              { return fileExtensions; }
bool AudioFormat::isCompressed()                                { return false; }
StringArray AudioFormat::getQualityOptions()                    { return {}; }

MemoryMappedAudioFormatReader* AudioFormat::createMemoryMappedReader (const File&)
{
    return nullptr;
}

MemoryMappedAudioFormatReader* AudioFormat::createMemoryMappedReader (FileInputStream* fin)
{
    delete fin;
    return nullptr;
}

bool AudioFormat::isChannelLayoutSupported (const AudioChannelSet& channelSet)
{
    if (channelSet == AudioChannelSet::mono())      return canDoMono();
    if (channelSet == AudioChannelSet::stereo())    return canDoStereo();

    return false;
}

using StringMap = std::unordered_map<String, String>;

static StringMap toMap (const StringPairArray& array)
{
    StringMap result;

    for (auto i = 0; i < array.size(); ++i)
        result[array.getAllKeys()[i]] = array.getAllValues()[i];

    return result;
}

AudioFormatWriter* AudioFormat::createWriterForRawPtr (OutputStream* streamToWriteTo,
                                                       const AudioFormatWriterOptions& opt)
{
    auto owned = rawToUniquePtr (streamToWriteTo);

    if (auto writer = createWriterFor (owned, opt))
    {
        // Creating the writer succeeded, so it's the writer's responsibility to eventually free
        // the stream
        jassert (owned == nullptr);
        return writer.release();
    }

    // Creating the writer failed, so the stream should remain alive for re-use
    jassert (owned != nullptr);
    owned.release();

    return {};
}

AudioFormatWriter* AudioFormat::createWriterFor (OutputStream* streamToWriteTo,
                                                 double sampleRateToUse,
                                                 unsigned int numberOfChannels,
                                                 int bitsPerSample,
                                                 const StringPairArray& metadataValues,
                                                 int qualityOptionIndex)
{
    auto opt = AudioFormatWriter::Options{}.withSampleRate (sampleRateToUse)
                                           .withNumChannels ((int) numberOfChannels)
                                           .withBitsPerSample (bitsPerSample)
                                           .withMetadataValues (toMap (metadataValues))
                                           .withQualityOptionIndex (qualityOptionIndex);
    return createWriterForRawPtr (streamToWriteTo, opt);
}

AudioFormatWriter* AudioFormat::createWriterFor (OutputStream* streamToWriteTo,
                                                 double sampleRateToUse,
                                                 const AudioChannelSet& channelLayout,
                                                 int bitsPerSample,
                                                 const StringPairArray& metadataValues,
                                                 int qualityOptionIndex)
{
    if (! isChannelLayoutSupported (channelLayout))
        return nullptr;

    auto opt = AudioFormatWriter::Options{}.withSampleRate (sampleRateToUse)
                                           .withChannelLayout (channelLayout)
                                           .withBitsPerSample (bitsPerSample)
                                           .withMetadataValues (toMap (metadataValues))
                                           .withQualityOptionIndex (qualityOptionIndex);
    return createWriterForRawPtr (streamToWriteTo, opt);
}

} // namespace higgs
