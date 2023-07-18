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

#if HIGGS_USE_OPUS || DOXYGEN

//==============================================================================
/**
    Reads and writes the Opus audio format ('.opus' files).

    An Opus stream always decodes at 48000 Hz, so that is the sample rate every
    reader created by this class reports. The sample rate given to the writer
    describes the incoming audio, which is resampled to 48000 Hz while encoding.

    To compile this, you'll need to set the HIGGS_USE_OPUS flag.

    @see AudioFormat

    @tags{Audio}
*/
class HIGGS_API  OpusAudioFormat  : public AudioFormat
{
public:
    //==============================================================================
    OpusAudioFormat();

    //==============================================================================
    /** Returns a set of sample rates that the format can read and write. The Opus file format
        only ever stores information at 48000 Hz, but the encoder can resample the audio from
        these rates.
    */
    Array<int> getPossibleSampleRates() override;

    Array<int> getPossibleBitDepths() override;
    bool canDoStereo() override;
    bool canDoMono() override;
    bool isChannelLayoutSupported (const AudioChannelSet& channelSet) override;
    bool isCompressed() override;
    StringArray getQualityOptions() override;

    //==============================================================================
    /** Metadata property name used by the opus writer - if you set a string for this
        value, it will be written into the opus file as the name of the encoder app.

        @see createWriterFor
    */
    inline static constexpr const char* encoderName = "encoder";
    inline static constexpr const char* id3title = "id3title";              /**< Metadata key for setting an ID3 title. */
    inline static constexpr const char* id3artist = "id3artist";            /**< Metadata key for setting an ID3 artist name. */
    inline static constexpr const char* id3album = "id3album";              /**< Metadata key for setting an ID3 album. */
    inline static constexpr const char* id3comment = "id3comment";          /**< Metadata key for setting an ID3 comment. */
    inline static constexpr const char* id3date = "id3date";                /**< Metadata key for setting an ID3 date. */
    inline static constexpr const char* id3genre = "id3genre";              /**< Metadata key for setting an ID3 genre. */
    inline static constexpr const char* id3trackNumber = "id3trackNumber";  /**< Metadata key for setting an ID3 track number. */

    //==============================================================================
    AudioFormatReader* createReaderFor (InputStream* sourceStream,
                                        bool deleteStreamIfOpeningFails) override;

    std::unique_ptr<AudioFormatWriter> createWriterFor (std::unique_ptr<OutputStream>& streamToWriteTo,
                                                        const AudioFormatWriterOptions& options) override;

    using AudioFormat::createWriterFor;

private:
    HIGGS_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (OpusAudioFormat)
};

#endif

} // namespace higgs
