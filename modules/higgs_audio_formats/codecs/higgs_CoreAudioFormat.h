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

#if HIGGS_MAC || HIGGS_IOS || DOXYGEN

//==============================================================================
/**
    OSX and iOS only - This uses the AudioToolbox framework to read any audio
    format that the system has a codec for.

    This should be able to understand formats such as mp3, m4a, etc.

    @see AudioFormat

    @tags{Audio}
*/
class HIGGS_API  CoreAudioFormat     : public AudioFormat
{
public:
    /** File type hints. */
    enum class StreamKind
    {
        kNone,
        kAiff,
        kAifc,
        kWave,
        kSoundDesigner2,
        kNext,
        kMp3,
        kMp2,
        kMp1,
        kAc3,
        kAacAdts,
        kMpeg4,
        kM4a,
        kM4b,
        kCaf,
        k3gp,
        k3gp2,
        kAmr,
    };

    //==============================================================================
    /** Creates a format object. */
    CoreAudioFormat();

    /** Creates a format object and provides a hint as to the format of data
        to be read or written.
    */
    explicit CoreAudioFormat (StreamKind);

    /** Destructor. */
    ~CoreAudioFormat() override;

    //==============================================================================
    /** Metadata property name used when reading a caf file with a MIDI chunk. */
    static const char* const midiDataBase64;
    /** Metadata property name used when reading a caf file with tempo information. */
    static const char* const tempo;
    /** Metadata property name used when reading a caf file time signature information. */
    static const char* const timeSig;
    /** Metadata property name used when reading a caf file time signature information. */
    static const char* const keySig;

    //==============================================================================
    Array<int> getPossibleSampleRates() override;
    Array<int> getPossibleBitDepths() override;
    bool canDoStereo() override;
    bool canDoMono() override;

    //==============================================================================
    AudioFormatReader* createReaderFor (InputStream*,
                                        bool deleteStreamIfOpeningFails) override;

    std::unique_ptr<AudioFormatWriter> createWriterFor (std::unique_ptr<OutputStream>& streamToWriteTo,
                                                        const AudioFormatWriterOptions& options) override;

    using AudioFormat::createWriterFor;

private:
    StreamKind streamKind = StreamKind::kNone;

    HIGGS_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (CoreAudioFormat)
};

#endif

} // namespace higgs
