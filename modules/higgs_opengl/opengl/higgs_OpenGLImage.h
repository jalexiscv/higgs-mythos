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
    A type of ImagePixelData that stores its image data in an OpenGL
    framebuffer, allowing a HIGGS Image object to wrap a framebuffer.

    By creating an Image from an instance of an OpenGLFrameBufferImage,
    you can then use a Graphics object to draw into the framebuffer using normal
    HIGGS 2D operations.

    OpenGLFrameBuffer only supports RGBA framebuffers, so single channel images
    keep their image data in CPU memory instead, and upload it to a texture when
    drawn. For these images getFrameBufferFrom() will return nullptr.

    A request for an Image::RGB image is promoted to Image::ARGB, and will use a
    framebuffer in the same way as an Image::ARGB request.

    @see Image, ImageType, ImagePixelData, OpenGLFrameBuffer

    @tags{OpenGL}
*/
class HIGGS_API  OpenGLImageType     : public ImageType
{
public:
    OpenGLImageType();
    ~OpenGLImageType() override;

    ImagePixelData::Ptr create (Image::PixelFormat format, int width, int height, bool shouldClearImage) const override;
    int getTypeID() const override;

    static OpenGLFrameBuffer* getFrameBufferFrom (const Image&);
};

} // namespace higgs
