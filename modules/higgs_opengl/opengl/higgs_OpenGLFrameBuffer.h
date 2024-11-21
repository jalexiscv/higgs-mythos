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
    Creates an openGL frame buffer.

    @tags{OpenGL}
*/
class HIGGS_API  OpenGLFrameBuffer
{
public:
    /** Creates an uninitialised buffer.
        To actually allocate the buffer, use initialise().
    */
    OpenGLFrameBuffer();

    /** Destructor. */
    ~OpenGLFrameBuffer();

    //==============================================================================
    /** Tries to allocates a buffer of the given size.
        Note that a valid openGL context must be selected when you call this method,
        or it will fail.
    */
    bool initialise (OpenGLContext& context, int width, int height);

    /** Tries to allocates a buffer containing a copy of a given image.
        Note that a valid openGL context must be selected when you call this method,
        or it will fail.
    */
    bool initialise (OpenGLContext& context, const Image& content);

    /** Tries to allocate a copy of another framebuffer.
    */
    bool initialise (OpenGLFrameBuffer& other);

    /** Releases the buffer, if one has been allocated.
        Any saved state that was created with saveAndRelease() will also be freed by this call.
    */
    void release();

    /** If the framebuffer is active, this will save a stashed copy of its contents in main memory,
        and will release the GL buffer.
        After saving, the original state can be restored again by calling reloadSavedCopy().
    */
    void saveAndRelease();

    /** Restores the framebuffer content that was previously saved using saveAndRelease().
    */
    bool reloadSavedCopy (OpenGLContext& context);

    //==============================================================================
    /** Returns true if a valid buffer has been allocated. */
    bool isValid() const noexcept;

    /** Returns the width of the buffer. */
    int getWidth() const noexcept;

    /** Returns the height of the buffer. */
    int getHeight() const noexcept;

    /** Returns the texture ID number for using this buffer as a texture.

        The texture may be larger than the buffer itself, see getTextureWidth() and
        getTextureHeight(). The buffer's content occupies the top-left corner of the
        texture, so the top-left pixel of the buffer is at texture coordinate (0, 1).
    */
    GLuint getTextureID() const noexcept;

    /** Returns the width of the texture that backs this buffer.

        This is normally the same as getWidth(), but if the GL implementation is unable
        to create a texture with the exact size that was requested, a larger texture will
        be used instead, and this will return the actual width of that texture.
    */
    int getTextureWidth() const noexcept;

    /** Returns the height of the texture that backs this buffer.
        @see getTextureWidth
    */
    int getTextureHeight() const noexcept;

    //==============================================================================
    /** Selects this buffer as the current OpenGL rendering target. */
    bool makeCurrentRenderingTarget();

    /** Deselects this buffer as the current OpenGL rendering target. */
    void releaseAsRenderingTarget();

    /** Returns the ID of this framebuffer, or 0 if it isn't initialised. */
    GLuint getFrameBufferID() const noexcept;

    /** Returns the current frame buffer ID for the current context. */
    static GLuint getCurrentFrameBufferTarget() noexcept;

    /** Clears the framebuffer with the specified colour. */
    void clear (Colour colour);

    /** Selects the framebuffer as the current target, and clears it to transparent. */
    void makeCurrentAndClear();

    enum class RowOrder
    {
        fromBottomUp,   //< Standard order for OpenGL, the bottom-most row of pixels is first.
                        //< Using this pixel ordering may be faster, but may be incompatible
                        //< with other HIGGS functions that operate on image pixel data, as these
                        //< generally expect the rows to be ordered top-down.
        fromTopDown,    //< Standard order for HIGGS images, the top-most row of pixels is first.
    };

    /** Reads an area of pixels from the framebuffer into a 32-bit ARGB pixel array.

        @param targetData   the array to fill. It must have room for at least
                            sourceArea.getWidth() * sourceArea.getHeight() pixels, which
                            are written contiguously with no padding between rows.
        @param sourceArea   the area to read, in OpenGL coordinates, with its origin at the
                            bottom-left corner of the framebuffer.
        @param order        the order in which the rows of the area are written to targetData.
        @returns            true if the pixels were read, or false if the framebuffer isn't
                            currently allocated.
    */
    bool readPixels (PixelARGB* targetData, const Rectangle<int>& sourceArea, RowOrder order);

    /** Writes an area of pixels into the framebuffer from a specified pixel array.

        @param srcData      the pixels to write. It must contain at least
                            targetArea.getWidth() * targetArea.getHeight() pixels, stored
                            contiguously with no padding between rows.
        @param targetArea   the area to write, in OpenGL coordinates, with its origin at the
                            bottom-left corner of the framebuffer.
        @param order        the order of the rows in srcData.
        @returns            true if the pixels were written, or false if the framebuffer isn't
                            currently allocated.
    */
    bool writePixels (const PixelARGB* srcData, const Rectangle<int>& targetArea, RowOrder order);

private:
    class Pimpl;
    std::unique_ptr<Pimpl> pimpl;

    HIGGS_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (OpenGLFrameBuffer)
};

} // namespace higgs
