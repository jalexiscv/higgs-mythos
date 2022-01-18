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
    Base-class for codecs that can read and write image file formats such
    as PNG, JPEG, etc.

    This class also contains static methods to make it easy to load images
    from files, streams or from memory.

    @see Image, ImageCache

    @tags{Graphics}
*/
class HIGGS_API  ImageFileFormat
{
protected:
    //==============================================================================
    /** Creates an ImageFormat. */
    ImageFileFormat() = default;

public:
    /** Destructor. */
    virtual ~ImageFileFormat() = default;

    //==============================================================================
    /** Returns a description of this file format.

        E.g. "JPEG", "PNG"
    */
    virtual String getFormatName() = 0;

    /** Returns true if the given stream seems to contain data that this format understands.

        The format class should only read the first few bytes of the stream and sniff
        for header bytes that it understands.

        Note that this will advance the stream and leave it in a new position, so if you're
        planning on re-using it, you may want to rewind it after calling this method.

        @see decodeImage
    */
    virtual bool canUnderstand (InputStream& input) = 0;

    /** Returns true if this format uses the file extension of the given file. */
    virtual bool usesFileExtension (const File& possibleFile) = 0;

    /** Tries to decode and return an image from the given stream.

        This will be called for an image format after calling its canUnderStand() method
        to see if it can handle the stream.

        @param input    the stream to read the data from. The stream will be positioned
                        at the start of the image data (but this may not necessarily
                        be position 0)
        @returns        the image that was decoded, or an invalid image if it fails.
        @see loadFrom
    */
    virtual Image decodeImage (InputStream& input) = 0;

    //==============================================================================
    /** Attempts to write an image to a stream.

        To specify extra information like encoding quality, there will be appropriate parameters
        in the subclasses of the specific file types.

        @returns        true if it nothing went wrong.
    */
    virtual bool writeImageToStream (const Image& sourceImage,
                                     OutputStream& destStream) = 0;

    //==============================================================================
    /** Tries the built-in formats to see if it can find one to read this stream.
        There are currently built-in decoders for PNG, JPEG, GIF and WebP formats.
        The object that is returned should not be deleted by the caller.
        @see canUnderstand, decodeImage, loadFrom
    */
    static ImageFileFormat* findImageFormatForStream (InputStream& input);

    /** Looks for a format that can handle the given file extension.
        There are currently built-in formats for PNG, JPEG, GIF and WebP formats.
        The object that is returned should not be deleted by the caller.
    */
    static ImageFileFormat* findImageFormatForFileExtension (const File& file);

    //==============================================================================
    /** Tries to load an image from a stream.

        This will use the findImageFormatForStream() method to locate a suitable
        codec, and use that to load the image.

        @returns        the image that was decoded, or an invalid image if it fails.
    */
    static Image loadFrom (InputStream& input);

    /** Tries to load an image from a file.

        This will use the findImageFormatForStream() method to locate a suitable
        codec, and use that to load the image.

        @returns        the image that was decoded, or an invalid image if it fails.
    */
    static Image loadFrom (const File& file);

    /** Tries to load an image from a block of raw image data.

        This will use the findImageFormatForStream() method to locate a suitable
        codec, and use that to load the image.

        @returns        the image that was decoded, or an invalid image if it fails.
    */
    static Image loadFrom (const void* rawData,
                           size_t numBytesOfData);
};

//==============================================================================
/**
    A subclass of ImageFileFormat for reading and writing PNG files.

    @see ImageFileFormat, JPEGImageFormat

    @tags{Graphics}
*/
class HIGGS_API  PNGImageFormat  : public ImageFileFormat
{
public:
    //==============================================================================
    PNGImageFormat();
    ~PNGImageFormat() override;

    //==============================================================================
    String getFormatName() override;
    bool usesFileExtension (const File&) override;
    bool canUnderstand (InputStream&) override;
    Image decodeImage (InputStream&) override;
    bool writeImageToStream (const Image&, OutputStream&) override;
};


//==============================================================================
/**
    A subclass of ImageFileFormat for reading and writing JPEG files.

    @see ImageFileFormat, PNGImageFormat

    @tags{Graphics}
*/
class HIGGS_API  JPEGImageFormat  : public ImageFileFormat
{
public:
    //==============================================================================
    JPEGImageFormat();
    ~JPEGImageFormat() override;

    //==============================================================================
    /** Specifies the quality to be used when writing a JPEG file.

        @param newQuality  a value 0 to 1.0, where 0 is low quality, 1.0 is best, or
                           any negative value is "default" quality
    */
    void setQuality (float newQuality);

    //==============================================================================
    String getFormatName() override;
    bool usesFileExtension (const File&) override;
    bool canUnderstand (InputStream&) override;
    Image decodeImage (InputStream&) override;
    bool writeImageToStream (const Image&, OutputStream&) override;

private:
    float quality;
};

//==============================================================================
/**
    A subclass of ImageFileFormat for reading GIF files.

    @see ImageFileFormat, PNGImageFormat, JPEGImageFormat

    @tags{Graphics}
*/
class HIGGS_API  GIFImageFormat  : public ImageFileFormat
{
public:
    //==============================================================================
    GIFImageFormat();
    ~GIFImageFormat() override;

    //==============================================================================
    String getFormatName() override;
    bool usesFileExtension (const File&) override;
    bool canUnderstand (InputStream&) override;
    Image decodeImage (InputStream&) override;
    bool writeImageToStream (const Image&, OutputStream&) override;
};

//==============================================================================
#if HIGGS_USE_WEBP || DOXYGEN

/**
    A subclass of ImageFileFormat for reading and writing WebP files.

    Both lossy and lossless WebP images can be read and written. Writing is lossy
    by default; call setLossless() to change that.

    Animated WebP files can be read, but as ImageFileFormat has no concept of
    multiple frames, only the first frame is returned. Writing animations is not
    supported.

    @see ImageFileFormat, PNGImageFormat, JPEGImageFormat

    @tags{Graphics}
*/
class HIGGS_API  WebPImageFormat  : public ImageFileFormat
{
public:
    //==============================================================================
    /** Specifies the quality to be used when writing a lossy WebP file.

        @param newQuality  a value 0 to 1.0, where 0 is low quality, 1.0 is best, the
                           default value is 0.9

        This has no effect when writing losslessly.
    */
    void setQuality (float newQuality);

    /** Specifies whether images should be written losslessly.

        This is false by default, meaning that written images are lossy, using the
        quality given to setQuality().
    */
    void setLossless (bool shouldBeLossless);

    //==============================================================================
    String getFormatName() override;
    bool usesFileExtension (const File&) override;
    bool canUnderstand (InputStream&) override;
    Image decodeImage (InputStream&) override;
    bool writeImageToStream (const Image&, OutputStream&) override;

private:
    // cwebp's default is 75 but it looks bad. We shouldn't have a default looking as bad as that.
    float quality = 0.9f;
    bool lossless = false;
};

#endif

} // namespace higgs
