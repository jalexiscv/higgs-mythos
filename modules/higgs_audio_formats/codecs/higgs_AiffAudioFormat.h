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

//==============================================================================
/**
    Reads and Writes AIFF format audio files.

    @see AudioFormat

    @tags{Audio}
*/
class HIGGS_API  AiffAudioFormat  : public AudioFormat
{
public:
    //==============================================================================
    /** Creates an format object. */
    AiffAudioFormat();

    /** Destructor. */
    ~AiffAudioFormat() override;

    //==============================================================================
    /** Metadata property name used when reading a aiff file with a basc chunk. */
    static const char* const appleOneShot;
    /** Metadata property name used when reading a aiff file with a basc chunk. */
    static const char* const appleRootSet;
    /** Metadata property name used when reading a aiff file with a basc chunk. */
    static const char* const appleRootNote;
    /** Metadata property name used when reading a aiff file with a basc chunk. */
    static const char* const appleBeats;
    /** Metadata property name used when reading a aiff file with a basc chunk. */
    static const char* const appleDenominator;
    /** Metadata property name used when reading a aiff file with a basc chunk. */
    static const char* const appleNumerator;
    /** Metadata property name used when reading a aiff file with a basc chunk. */
    static const char* const appleTag;
    /** Metadata property name used when reading a aiff file with a basc chunk. */
    static const char* const appleKey;

    //==============================================================================
    Array<int> getPossibleSampleRates() override;
    Array<int> getPossibleBitDepths() override;
    bool canDoStereo() override;
    bool canDoMono() override;

   #if HIGGS_MAC
    bool canHandleFile (const File& fileToTest) override;
   #endif

    //==============================================================================
    AudioFormatReader* createReaderFor (InputStream* sourceStream,
                                        bool deleteStreamIfOpeningFails) override;

    MemoryMappedAudioFormatReader* createMemoryMappedReader (const File&)      override;
    MemoryMappedAudioFormatReader* createMemoryMappedReader (FileInputStream*) override;

    std::unique_ptr<AudioFormatWriter> createWriterFor (std::unique_ptr<OutputStream>& streamToWriteTo,
                                                        const AudioFormatWriterOptions& options) override;

    using AudioFormat::createWriterFor;

private:
    HIGGS_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AiffAudioFormat)
};

} // namespace higgs
