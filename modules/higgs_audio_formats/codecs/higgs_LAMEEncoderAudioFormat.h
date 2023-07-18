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

#if HIGGS_USE_LAME_AUDIO_FORMAT || DOXYGEN

//==============================================================================
/**
    An AudioFormat class which can use an installed version of the LAME mp3
    encoder to encode a file.

    This format can't read MP3s, it just writes them. Internally, the
    AudioFormatWriter object that is returned writes the incoming audio data
    to a temporary WAV file, and then when the writer is deleted, it invokes
    the LAME executable to convert the data to an MP3, whose data is then
    piped into the original OutputStream that was used when first creating
    the writer.

    @see AudioFormat

    @tags{Audio}
*/
class HIGGS_API  LAMEEncoderAudioFormat    : public AudioFormat
{
public:
    /** Creates a LAMEEncoderAudioFormat that expects to find a working LAME
        executable at the location given.
    */
    LAMEEncoderAudioFormat (const File& lameExecutableToUse);

    bool canHandleFile (const File&) override;
    Array<int> getPossibleSampleRates() override;
    Array<int> getPossibleBitDepths() override;
    bool canDoStereo() override;
    bool canDoMono() override;
    bool isCompressed() override;
    StringArray getQualityOptions() override;

    std::unique_ptr<AudioFormatWriter> createWriterFor (std::unique_ptr<OutputStream>& streamToWriteTo,
                                                        const AudioFormatWriterOptions& options) override;

    AudioFormatReader* createReaderFor (InputStream*, bool deleteStreamIfOpeningFails) override;

    using AudioFormat::createWriterFor;

private:
    File lameApp;
    class Writer;

    HIGGS_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (LAMEEncoderAudioFormat)
};

#endif

} // namespace higgs
