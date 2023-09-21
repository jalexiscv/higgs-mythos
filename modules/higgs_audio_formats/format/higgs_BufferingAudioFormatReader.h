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
    An AudioFormatReader that uses a background thread to pre-read data from
    another reader.

    @see AudioFormatReader

    @tags{Audio}
*/
class HIGGS_API  BufferingAudioReader  : public AudioFormatReader,
                                        private TimeSliceClient
{
public:
    /** Creates a reader.

        @param sourceReader     the source reader to wrap. This BufferingAudioReader
                                takes ownership of this object and will delete it later
                                when no longer needed
        @param timeSliceThread  the thread that should be used to do the background reading.
                                Make sure that the thread you supply is running, and won't
                                be deleted while the reader object still exists.
        @param samplesToBuffer  the total number of samples to buffer ahead.
    */
    BufferingAudioReader (AudioFormatReader* sourceReader,
                          TimeSliceThread& timeSliceThread,
                          int samplesToBuffer);

    ~BufferingAudioReader() override;

    /** Sets a number of milliseconds that the reader can block for in its readSamples()
        method before giving up and returning silence.

        A value of less that 0 means "wait forever". The default timeout is 0.
    */
    void setReadTimeout (int timeoutMilliseconds) noexcept;

    //==============================================================================
    bool readSamples (int* const* destSamples, int numDestChannels, int startOffsetInDestBuffer,
                      int64 startSampleInFile, int numSamples) override;

private:
    struct BufferedBlock
    {
        BufferedBlock (AudioFormatReader& reader, int64 pos, int numSamples);

        Range<int64> range;
        AudioBuffer<float> buffer;
        bool allSamplesRead = false;
    };

    int useTimeSlice() override;
    BufferedBlock* getBlockContaining (int64 pos) const noexcept;
    bool readNextBufferChunk();

    static constexpr int samplesPerBlock = 32768;

    std::unique_ptr<AudioFormatReader> source;
    TimeSliceThread& thread;
    std::atomic<int64> nextReadPosition { 0 };
    const int numBlocks;
    int timeoutMs = 0;

    CriticalSection lock;
    OwnedArray<BufferedBlock> blocks;

    HIGGS_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (BufferingAudioReader)
};

} // namespace higgs
