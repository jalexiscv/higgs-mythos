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

OpenGLTexture::OpenGLTexture()
    : textureID (0), width (0), height (0), ownerContext (nullptr)
{
}

OpenGLTexture::~OpenGLTexture()
{
    release();
}

bool OpenGLTexture::isValidSize (int width, int height)
{
    return isPowerOfTwo (width) && isPowerOfTwo (height);
}

void OpenGLTexture::create (const int w, const int h, const void* pixels, GLenum type, bool topLeft)
{
    ownerContext = OpenGLContext::getCurrentContext();

    // Texture objects can only be created when the current thread has an active OpenGL
    // context. You'll need to create this object in one of the OpenGLContext's callbacks.
    jassert (ownerContext != nullptr);

    HIGGS_CHECK_OPENGL_ERROR

    if (textureID == 0)
        glGenTextures (1, &textureID);

    glBindTexture (GL_TEXTURE_2D, textureID);

    // We need to set parameters on every upload, not just when the texture name is
    // generated. Textures that outlive a single upload, like those held by the image
    // cache, would otherwise be stuck with the magnification filter that was current
    // when they were first created, and would ignore any later call to
    // OpenGLContext::setTextureMagnificationFilter().
    const auto glMagFilter = (GLint) (ownerContext != nullptr && ownerContext->texMagFilter == OpenGLContext::nearest
                                          ? GL_NEAREST
                                          : GL_LINEAR);

    glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, glMagFilter);
    glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    HIGGS_CHECK_OPENGL_ERROR

    glPixelStorei (GL_UNPACK_ALIGNMENT, 1);
    HIGGS_CHECK_OPENGL_ERROR

    if (! tryAllocTexture (w, h, type))
    {
        // Completely failed to create a workable texture
        jassertfalse;
        return;
    }

    GLint detectedWidth{}, detectedHeight{};
    glGetTexLevelParameteriv (GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &detectedWidth);
    glGetTexLevelParameteriv (GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &detectedHeight);
    std::tie (width, height) = std::tie (detectedWidth, detectedHeight);

    if (width < w || height < h)
    {
        // Created a texture, but it's not large enough, somehow
        jassertfalse;
        return;
    }

    if (pixels != nullptr)
    {
        glTexSubImage2D (GL_TEXTURE_2D,
                         0,
                         0,
                         topLeft ? (height - h) : 0,
                         w,
                         h,
                         type,
                         GL_UNSIGNED_BYTE,
                         pixels);
    }

    HIGGS_CHECK_OPENGL_ERROR
}

struct UploadScratchBuffer final : public ReferenceCountedObject
{
    UploadScratchBuffer() = default;

    static UploadScratchBuffer& get (OpenGLContext& c)
    {
        const char scratchValueID[] = "ImageUploadScratch";
        auto* scratch = static_cast<UploadScratchBuffer*> (c.getAssociatedObject (scratchValueID));

        if (scratch == nullptr)
        {
            scratch = new UploadScratchBuffer();
            c.setAssociatedObject (scratchValueID, scratch);
        }

        return *scratch;
    }

    [[nodiscard]] Span<PixelARGB> getWithSize (size_t numPixels)
    {
        if (pixels.size() < numPixels)
            pixels = CopyableHeapBlock<PixelARGB> (numPixels);

        return { pixels.data(), numPixels };
    }

private:
    CopyableHeapBlock<PixelARGB> pixels;

    HIGGS_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (UploadScratchBuffer)
};

template <class PixelType>
struct Flipper
{
    static void flip (Span<PixelARGB> dest,
                      const uint8* srcData,
                      const int lineStride,
                      const int pixelStride,
                      const int w,
                      const int h)
    {
        jassert (dest.size() >= (size_t) w * (size_t) h);

        for (int y = 0; y < h; ++y)
        {
            auto* srcLine = srcData + lineStride * y;
            auto* dstLine = dest.data() + w * (h - 1 - y);

            if constexpr (std::is_same_v<PixelType, PixelARGB>)
            {
                if ((size_t) pixelStride == sizeof (PixelARGB))
                {
                    std::memcpy (dstLine, srcLine, (size_t) w * sizeof (PixelARGB));
                    continue;
                }
            }

            for (int x = 0; x < w; ++x)
            {
                auto* srcPixel = srcLine + x * pixelStride;
                dstLine[x].set (*unalignedPointerCast<const PixelType*> (srcPixel));
            }
        }
    }
};

void OpenGLTexture::loadImage (const Image& image)
{
    auto* context = OpenGLContext::getCurrentContext();

    if (context == nullptr)
    {
        jassertfalse;
        return;
    }

    const int imageW = image.getWidth();
    const int imageH = image.getHeight();

    Image::BitmapData srcData (image, Image::BitmapData::readOnly);
    const auto dataCopy = UploadScratchBuffer::get (*context).getWithSize ((size_t) imageW * (size_t) imageH);

    switch (srcData.pixelFormat)
    {
        case Image::ARGB:           Flipper<PixelARGB> ::flip (dataCopy, srcData.data, srcData.lineStride, srcData.pixelStride, imageW, imageH); break;
        case Image::RGB:            Flipper<PixelRGB>  ::flip (dataCopy, srcData.data, srcData.lineStride, srcData.pixelStride, imageW, imageH); break;
        case Image::SingleChannel:  Flipper<PixelAlpha>::flip (dataCopy, srcData.data, srcData.lineStride, srcData.pixelStride, imageW, imageH); break;

        case Image::UnknownFormat:
        default:
            create (imageW, imageH, nullptr, HIGGS_RGBA_FORMAT, true);
            return;
    }

    create (imageW, imageH, dataCopy.data(), HIGGS_RGBA_FORMAT, true);
}

void OpenGLTexture::loadARGB (const PixelARGB* pixels, const int w, const int h)
{
    if (OpenGLContext::getCurrentContext() == nullptr)
    {
        jassertfalse;
        return;
    }

    create (w, h, pixels, HIGGS_RGBA_FORMAT, false);
}

void OpenGLTexture::loadAlpha (const uint8* pixels, int w, int h)
{
    if (OpenGLContext::getCurrentContext() == nullptr)
    {
        jassertfalse;
        return;
    }

    create (w, h, pixels, GL_ALPHA, false);
}

void OpenGLTexture::loadARGBFlipped (const PixelARGB* pixels, int w, int h)
{
    auto* context = OpenGLContext::getCurrentContext();

    if (context == nullptr)
    {
        jassertfalse;
        return;
    }

    const auto flippedCopy = UploadScratchBuffer::get (*context).getWithSize ((size_t) w * (size_t) h);
    Flipper<PixelARGB>::flip (flippedCopy, (const uint8*) pixels, 4 * w, 4, w, h);

    create (w, h, flippedCopy.data(), HIGGS_RGBA_FORMAT, true);
}

void OpenGLTexture::release()
{
    if (textureID != 0)
    {
        // If the texture is deleted while the owner context is not active, it's
        // impossible to delete it, so this will be a leak until the context itself
        // is deleted.
        jassert (ownerContext == OpenGLContext::getCurrentContext());

        if (ownerContext == OpenGLContext::getCurrentContext())
        {
            glDeleteTextures (1, &textureID);

            textureID = 0;
            width = 0;
            height = 0;
        }
    }
}

void OpenGLTexture::bind() const
{
    glBindTexture (GL_TEXTURE_2D, textureID);
}

void OpenGLTexture::unbind() const
{
    glBindTexture (GL_TEXTURE_2D, 0);
}

} // namespace higgs
