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

AudioFormatWriter::AudioFormatWriter (OutputStream* const out,
                                      const String& formatName_,
                                      const double rate,
                                      const unsigned int numChannels_,
                                      const unsigned int bitsPerSample_)
  : sampleRate (rate),
    numChannels (numChannels_),
    bitsPerSample (bitsPerSample_),
    usesFloatingPointData (false),
    channelLayout (AudioChannelSet::canonicalChannelSet (static_cast<int> (numChannels_))),
    output (out),
    formatName (formatName_)
{
}

AudioFormatWriter::AudioFormatWriter (OutputStream* const out,
                                      const String& formatName_,
                                      const double rate,
                                      const AudioChannelSet& channelLayout_,
                                      const unsigned int bitsPerSample_)
  : sampleRate (rate),
    numChannels (static_cast<unsigned int> (channelLayout_.size())),
    bitsPerSample (bitsPerSample_),
    usesFloatingPointData (false),
    channelLayout (channelLayout_),
    output (out),
    formatName (formatName_)
{
}

AudioFormatWriter::~AudioFormatWriter()
{
    delete output;
}

static void convertFloatsToInts (int* dest, const float* src, int numSamples) noexcept
{
    while (--numSamples >= 0)
    {
        const double samp = *src++;

        if (samp <= -1.0)
            *dest = std::numeric_limits<int>::min();
        else if (samp >= 1.0)
            *dest = std::numeric_limits<int>::max();
        else
            *dest = roundToInt (std::numeric_limits<int>::max() * samp);

        ++dest;
    }
}

bool AudioFormatWriter::writeFromAudioReader (AudioFormatReader& reader,
                                              int64 startSample,
                                              int64 numSamplesToRead)
{
    // numChannels is set by the writer subclass, so may hold a value that doesn't fit in an int
    const auto numDestChannels = (int) jmin (numChannels, (unsigned int) std::numeric_limits<int>::max());

    constexpr int preferredBufferSize = 16384;
    constexpr int maxBufferedSamples = 1 << 20;
    const auto bufferSize = jlimit (1, preferredBufferSize, maxBufferedSamples / jmax (1, numDestChannels));

    AudioBuffer<float> tempBuffer (numDestChannels, bufferSize);
    std::vector<int*> buffers ((size_t) numDestChannels + 1);

    for (int i = tempBuffer.getNumChannels(); --i >= 0;)
        buffers[(size_t) i] = reinterpret_cast<int*> (tempBuffer.getWritePointer (i, 0));

    if (numSamplesToRead < 0)
        numSamplesToRead = reader.lengthInSamples;

    while (numSamplesToRead > 0)
    {
        const int numToDo = (int) jmin (numSamplesToRead, (int64) bufferSize);

        if (! reader.read (buffers.data(), numDestChannels, startSample, numToDo, false))
            return false;

        if (reader.usesFloatingPointData != isFloatingPoint())
        {
            int** bufferChan = buffers.data();

            while (*bufferChan != nullptr)
            {
                void* const b = *bufferChan++;

                constexpr auto scaleFactor = 1.0f / static_cast<float> (0x7fffffff);

                if (isFloatingPoint())
                    FloatVectorOperations::convertFixedToFloat ((float*) b, (int*) b, scaleFactor, numToDo);
                else
                    convertFloatsToInts ((int*) b, (float*) b, numToDo);
            }
        }

        if (! write (const_cast<const int**> (buffers.data()), numToDo))
            return false;

        numSamplesToRead -= numToDo;
        startSample += numToDo;
    }

    return true;
}

bool AudioFormatWriter::writeFromAudioSource (AudioSource& source, int numSamplesToRead, const int samplesPerBlock)
{
    AudioBuffer<float> tempBuffer (getNumChannels(), samplesPerBlock);

    while (numSamplesToRead > 0)
    {
        auto numToDo = jmin (numSamplesToRead, samplesPerBlock);

        AudioSourceChannelInfo info (&tempBuffer, 0, numToDo);
        info.clearActiveBufferRegion();

        source.getNextAudioBlock (info);

        if (! writeFromAudioSampleBuffer (tempBuffer, 0, numToDo))
            return false;

        numSamplesToRead -= numToDo;
    }

    return true;
}

bool AudioFormatWriter::writeFromFloatArrays (const float* const* channels, int numSourceChannels, int numSamples)
{
    if (numSamples <= 0)
        return true;

    jassert (numSourceChannels > 0);

    if (numSourceChannels <= 0)
        return false;

    if (isFloatingPoint())
        return write ((const int**) channels, numSamples);

    constexpr int preferredScratchSize = 4096;
    std::vector<int*> chans ((size_t) numSourceChannels + 1);
    std::vector<int> scratch ((size_t) jmax (preferredScratchSize, numSourceChannels));

    const int maxSamples = (int) scratch.size() / numSourceChannels;

    for (int i = 0; i < numSourceChannels; ++i)
        chans[(size_t) i] = scratch.data() + (i * maxSamples);

    chans[(size_t) numSourceChannels] = nullptr;
    int startSample = 0;

    while (numSamples > 0)
    {
        auto numToDo = jmin (numSamples, maxSamples);

        for (int i = 0; i < numSourceChannels; ++i)
            convertFloatsToInts (chans[(size_t) i], channels[(size_t) i] + startSample, numToDo);

        if (! write ((const int**) chans.data(), numToDo))
            return false;

        startSample += numToDo;
        numSamples  -= numToDo;
    }

    return true;
}

bool AudioFormatWriter::writeFromAudioSampleBuffer (const AudioBuffer<float>& source, int startSample, int numSamples)
{
    auto numSourceChannels = source.getNumChannels();
    jassert (startSample >= 0 && startSample + numSamples <= source.getNumSamples() && numSourceChannels > 0);

    if (startSample == 0)
        return writeFromFloatArrays (source.getArrayOfReadPointers(), numSourceChannels, numSamples);

    std::vector<const float*> chans ((size_t) numSourceChannels + 1);

    for (int i = 0; i < numSourceChannels; ++i)
        chans[(size_t) i] = source.getReadPointer (i, startSample);

    return writeFromFloatArrays (chans.data(), numSourceChannels, numSamples);
}

bool AudioFormatWriter::flush()
{
    return false;
}

//==============================================================================
class AudioFormatWriter::ThreadedWriter::Buffer final : private TimeSliceClient
{
public:
    Buffer (TimeSliceThread& tst, AudioFormatWriter* w, int channels, int numSamples)
        : fifo (numSamples),
          buffer (channels, numSamples),
          timeSliceThread (tst),
          writer (w)
    {
        timeSliceThread.addTimeSliceClient (this);
    }

    ~Buffer() override
    {
        isRunning = false;
        timeSliceThread.removeTimeSliceClient (this);

        while (writePendingData() == 0)
        {}
    }

    bool write (const float* const* data, int numSamples)
    {
        if (numSamples <= 0 || ! isRunning)
            return true;

        jassert (timeSliceThread.isThreadRunning());  // you need to get your thread running before pumping data into this!

        int start1, size1, start2, size2;
        fifo.prepareToWrite (numSamples, start1, size1, start2, size2);

        if (size1 + size2 < numSamples)
            return false;

        for (int i = buffer.getNumChannels(); --i >= 0;)
        {
            buffer.copyFrom (i, start1, data[i], size1);
            buffer.copyFrom (i, start2, data[i] + size1, size2);
        }

        fifo.finishedWrite (size1 + size2);
        timeSliceThread.notify();
        return true;
    }

    int useTimeSlice() override
    {
        return writePendingData();
    }

    int writePendingData()
    {
        auto numToDo = fifo.getTotalSize() / 4;

        int start1, size1, start2, size2;
        fifo.prepareToRead (numToDo, start1, size1, start2, size2);

        if (size1 <= 0)
            return 10;

        writer->writeFromAudioSampleBuffer (buffer, start1, size1);

        const ScopedLock sl (thumbnailLock);

        if (receiver != nullptr)
            receiver->addBlock (samplesWritten, buffer, start1, size1);

        samplesWritten += size1;

        if (size2 > 0)
        {
            writer->writeFromAudioSampleBuffer (buffer, start2, size2);

            if (receiver != nullptr)
                receiver->addBlock (samplesWritten, buffer, start2, size2);

            samplesWritten += size2;
        }

        fifo.finishedRead (size1 + size2);

        if (samplesPerFlush > 0)
        {
            flushSampleCounter -= size1 + size2;

            if (flushSampleCounter <= 0)
            {
                flushSampleCounter = samplesPerFlush;
                writer->flush();
            }
        }

        return 0;
    }

    void setDataReceiver (IncomingDataReceiver* newReceiver)
    {
        if (newReceiver != nullptr)
            newReceiver->reset (buffer.getNumChannels(), writer->getSampleRate(), 0);

        const ScopedLock sl (thumbnailLock);
        receiver = newReceiver;
        samplesWritten = 0;
    }

    void setFlushInterval (int numSamples) noexcept
    {
        samplesPerFlush = numSamples;
    }

private:
    AbstractFifo fifo;
    AudioBuffer<float> buffer;
    TimeSliceThread& timeSliceThread;
    std::unique_ptr<AudioFormatWriter> writer;
    CriticalSection thumbnailLock;
    IncomingDataReceiver* receiver = {};
    int64 samplesWritten = 0;
    int samplesPerFlush = 0, flushSampleCounter = 0;
    std::atomic<bool> isRunning { true };

    HIGGS_DECLARE_NON_COPYABLE (Buffer)
};

AudioFormatWriter::ThreadedWriter::ThreadedWriter (AudioFormatWriter* writer, TimeSliceThread& backgroundThread, int numSamplesToBuffer)
    : buffer (new AudioFormatWriter::ThreadedWriter::Buffer (backgroundThread, writer, (int) writer->numChannels, numSamplesToBuffer))
{
}

AudioFormatWriter::ThreadedWriter::~ThreadedWriter()
{
}

bool AudioFormatWriter::ThreadedWriter::write (const float* const* data, int numSamples)
{
    return buffer->write (data, numSamples);
}

void AudioFormatWriter::ThreadedWriter::setDataReceiver (AudioFormatWriter::ThreadedWriter::IncomingDataReceiver* receiver)
{
    buffer->setDataReceiver (receiver);
}

void AudioFormatWriter::ThreadedWriter::setFlushInterval (int numSamplesPerFlush) noexcept
{
    buffer->setFlushInterval (numSamplesPerFlush);
}

} // namespace higgs
