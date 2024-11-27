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
    Creates an openGL texture from an Image.

    @tags{OpenGL}
*/
class HIGGS_API  OpenGLTexture
{
public:
    OpenGLTexture();
    ~OpenGLTexture();

    /** Creates a texture from the given image.

        Note that if the image's dimensions aren't a power-of-two, the texture may
        be created with a larger size.

        The image will be arranged so that its top-left corner is at texture
        coordinate (0, 1).

        This must be called while a context is current. If there is no current
        context, this function does nothing, and the texture is left unchanged.
    */
    void loadImage (const Image& image);

    /** Creates a texture from a raw array of pixels.

        If width and height are not powers-of-two, the texture will be created with a
        larger size, and only the subsection (0, 0, width, height) will be initialised.
        The data is sent directly to the OpenGL driver without being flipped vertically,
        so the first pixel will be mapped onto texture coordinate (0, 0).

        This must be called while a context is current. If there is no current
        context, this function does nothing, and the texture is left unchanged.
    */
    void loadARGB (const PixelARGB* pixels, int width, int height);

    /** Creates a texture from a raw array of pixels.

        This is like loadARGB, but will vertically flip the data so that the first
        pixel ends up at texture coordinate (0, 1), and if the width and height are
        not powers-of-two, it will compensate by using a larger texture size.

        This must be called while a context is current. If there is no current
        context, this function does nothing, and the texture is left unchanged.
    */
    void loadARGBFlipped (const PixelARGB* pixels, int width, int height);

    /** Creates an alpha-channel texture from an array of alpha values.

        If width and height are not powers-of-two, the texture will be created with a
        larger size, and only the subsection (0, 0, width, height) will be initialised.
        The data is sent directly to the OpenGL driver without being flipped vertically,
        so the first pixel will be mapped onto texture coordinate (0, 0).

        This must be called while a context is current. If there is no current
        context, this function does nothing, and the texture is left unchanged.
    */
    void loadAlpha (const uint8* pixels, int width, int height);

    /** Frees the texture, if there is one.

        The texture can only be freed while the context that created it is
        current. If a different context is current, this function asserts and
        the texture will leak until the context that created it is destroyed.
    */
    void release();

    /** Binds the texture to the currently active openGL context. */
    void bind() const;

    /** Unbinds the texture to the currently active openGL context. */
    void unbind() const;

    /** Returns the GL texture ID number. */
    GLuint getTextureID() const noexcept        { return textureID; }

    int getWidth() const noexcept               { return width; }
    int getHeight() const noexcept              { return height; }

    /** Returns true if a texture can be created with the given size.
        Some systems may require that the sizes are powers-of-two.
    */
    static bool isValidSize (int width, int height);

private:
    GLuint textureID;
    int width, height;
    OpenGLContext* ownerContext;

    void create (int w, int h, const void*, GLenum, bool topLeft);

    HIGGS_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (OpenGLTexture)
};

} // namespace higgs
