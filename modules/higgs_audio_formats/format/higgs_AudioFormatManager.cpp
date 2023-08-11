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

AudioFormatManager::AudioFormatManager() {}
AudioFormatManager::~AudioFormatManager() {}

//==============================================================================
void AudioFormatManager::registerFormat (AudioFormat* newFormat, bool makeThisTheDefaultFormat)
{
    jassert (newFormat != nullptr);

    if (newFormat != nullptr)
    {
       #if HIGGS_DEBUG
        for (auto* af : knownFormats)
        {
            if (af->getFormatName() == newFormat->getFormatName())
                jassertfalse; // trying to add the same format twice!
        }
       #endif

        if (makeThisTheDefaultFormat)
            defaultFormatIndex = getNumKnownFormats();

        knownFormats.add (newFormat);
    }
}

void AudioFormatManager::registerBasicFormats()
{
    registerFormat (new WavAudioFormat(), true);
    registerFormat (new AiffAudioFormat(), false);

   #if HIGGS_USE_FLAC
    registerFormat (new FlacAudioFormat(), false);
   #endif

   #if HIGGS_USE_OGGVORBIS
    registerFormat (new OggVorbisAudioFormat(), false);
   #endif

   #if HIGGS_USE_OPUS
    registerFormat (new OpusAudioFormat(), false);
   #endif

   #if HIGGS_MAC || HIGGS_IOS
    registerFormat (new CoreAudioFormat(), false);
   #endif

   #if HIGGS_USE_MP3AUDIOFORMAT
    registerFormat (new MP3AudioFormat(), false);
   #endif

   #if HIGGS_USE_WINDOWS_MEDIA_FORMAT
    registerFormat (new WindowsMediaAudioFormat(), false);
   #endif
}

void AudioFormatManager::clearFormats()
{
    knownFormats.clear();
    defaultFormatIndex = 0;
}

int AudioFormatManager::getNumKnownFormats() const                  { return knownFormats.size(); }
AudioFormat* AudioFormatManager::getKnownFormat (int index) const   { return knownFormats[index]; }
AudioFormat* AudioFormatManager::getDefaultFormat() const           { return getKnownFormat (defaultFormatIndex); }

AudioFormat* AudioFormatManager::findFormatForFileExtension (const String& fileExtension) const
{
    if (! fileExtension.startsWithChar ('.'))
        return findFormatForFileExtension ("." + fileExtension);

    for (auto* af : knownFormats)
        if (af->getFileExtensions().contains (fileExtension, true))
            return af;

    return nullptr;
}

String AudioFormatManager::getWildcardForAllFormats() const
{
    StringArray extensions;

    for (auto* af : knownFormats)
        extensions.addArray (af->getFileExtensions());

    extensions.trim();
    extensions.removeEmptyStrings();

    for (auto& e : extensions)
        e = (e.startsWithChar ('.') ? "*" : "*.") + e;

    extensions.removeDuplicates (true);
    return extensions.joinIntoString (";");
}

//==============================================================================
AudioFormatReader* AudioFormatManager::createReaderFor (const File& file)
{
    // you need to actually register some formats before the manager can
    // use them to open a file!
    jassert (getNumKnownFormats() > 0);

    for (auto* af : knownFormats)
        if (af->canHandleFile (file))
            if (auto in = file.createInputStream())
                if (auto* r = af->createReaderFor (in.release(), true))
                    return r;

    return nullptr;
}

AudioFormatReader* AudioFormatManager::createReaderFor (std::unique_ptr<InputStream> audioFileStream)
{
    // you need to actually register some formats before the manager can
    // use them to open a file!
    jassert (getNumKnownFormats() > 0);

    if (audioFileStream != nullptr)
    {
        auto originalStreamPos = audioFileStream->getPosition();

        for (auto* af : knownFormats)
        {
            if (auto* r = af->createReaderFor (audioFileStream.get(), false))
            {
                audioFileStream.release();
                return r;
            }

            audioFileStream->setPosition (originalStreamPos);

            // the stream that is passed-in must be capable of being repositioned so
            // that all the formats can have a go at opening it.
            jassert (audioFileStream->getPosition() == originalStreamPos);
        }
    }

    return nullptr;
}

} // namespace higgs
