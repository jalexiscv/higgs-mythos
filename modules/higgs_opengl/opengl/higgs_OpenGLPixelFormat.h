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
    Represents the various properties of an OpenGL pixel format.

    @see OpenGLContext::setPixelFormat

    @tags{OpenGL}
*/
class HIGGS_API  OpenGLPixelFormat
{
public:
    //==============================================================================
    /** Creates an OpenGLPixelFormat.

        The default constructor just initialises the object as a simple 8-bit
        RGBA format.
    */
    OpenGLPixelFormat (int bitsPerRGBComponent = 8,
                       int alphaBits = 8,
                       int depthBufferBits = 16,
                       int stencilBufferBits = 0) noexcept;

    bool operator== (const OpenGLPixelFormat&) const noexcept;
    bool operator!= (const OpenGLPixelFormat&) const noexcept;

    //==============================================================================
    int redBits;          /**< The number of bits per pixel to use for the red channel. */
    int greenBits;        /**< The number of bits per pixel to use for the green channel. */
    int blueBits;         /**< The number of bits per pixel to use for the blue channel. */
    int alphaBits;        /**< The number of bits per pixel to use for the alpha channel. */

    int depthBufferBits;      /**< The number of bits per pixel to use for a depth buffer. */
    int stencilBufferBits;    /**< The number of bits per pixel to use for a stencil buffer. */

    int accumulationBufferRedBits;    /**< The number of bits per pixel to use for an accumulation buffer's red channel. */
    int accumulationBufferGreenBits;  /**< The number of bits per pixel to use for an accumulation buffer's green channel. */
    int accumulationBufferBlueBits;   /**< The number of bits per pixel to use for an accumulation buffer's blue channel. */
    int accumulationBufferAlphaBits;  /**< The number of bits per pixel to use for an accumulation buffer's alpha channel. */

    uint8 multisamplingLevel;         /**< The number of samples to use for full-scene multisampled anti-aliasing (if available). */
};

} // namespace higgs
