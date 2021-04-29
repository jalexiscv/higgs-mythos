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
    This stream will decompress a source-stream using zlib.

    Tip: if you're reading lots of small items from one of these streams, you
         can increase the performance enormously by passing it through a
         BufferedInputStream, so that it has to read larger blocks less often.

    @see GZIPCompressorOutputStream

    @tags{Core}
*/
class HIGGS_API  GZIPDecompressorInputStream  : public InputStream
{
public:
    enum Format
    {
        zlibFormat = 0,
        deflateFormat,
        gzipFormat
    };

    //==============================================================================
    /** Creates a decompressor stream.

        @param sourceStream                 the stream to read from
        @param deleteSourceWhenDestroyed    whether or not to delete the source stream
                                            when this object is destroyed
        @param sourceFormat                 can be used to select which of the supported
                                            formats the data is expected to be in
        @param uncompressedStreamLength     if the creator knows the length that the
                                            uncompressed stream will be, then it can supply this
                                            value, which will be returned by getTotalLength()
    */
    GZIPDecompressorInputStream (InputStream* sourceStream,
                                 bool deleteSourceWhenDestroyed,
                                 Format sourceFormat = zlibFormat,
                                 int64 uncompressedStreamLength = -1);

    /** Creates a decompressor stream.

        @param sourceStream     the stream to read from - the source stream must not be
                                deleted until this object has been destroyed
    */
    GZIPDecompressorInputStream (InputStream& sourceStream);

    /** Destructor. */
    ~GZIPDecompressorInputStream() override;

    //==============================================================================
    int64 getPosition() override;
    bool setPosition (int64 pos) override;
    int64 getTotalLength() override;
    bool isExhausted() override;
    int read (void* destBuffer, int maxBytesToRead) override;

private:
    //==============================================================================
    OptionalScopedPointer<InputStream> sourceStream;
    const int64 uncompressedStreamLength;
    const Format format;
    bool isEof = false;
    int activeBufferSize = 0;
    int64 originalSourcePos, currentPos = 0;
    HeapBlock<uint8> buffer;

    class GZIPDecompressHelper;
    std::unique_ptr<GZIPDecompressHelper> helper;

    HIGGS_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (GZIPDecompressorInputStream)
};

} // namespace higgs
