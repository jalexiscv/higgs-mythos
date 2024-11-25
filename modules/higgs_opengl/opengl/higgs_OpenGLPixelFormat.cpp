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

OpenGLPixelFormat::OpenGLPixelFormat (const int bitsPerRGBComponent,
                                      const int alphaBits_,
                                      const int depthBufferBits_,
                                      const int stencilBufferBits_) noexcept
    : redBits (bitsPerRGBComponent),
      greenBits (bitsPerRGBComponent),
      blueBits (bitsPerRGBComponent),
      alphaBits (alphaBits_),
      depthBufferBits (depthBufferBits_),
      stencilBufferBits (stencilBufferBits_),
      accumulationBufferRedBits (0),
      accumulationBufferGreenBits (0),
      accumulationBufferBlueBits (0),
      accumulationBufferAlphaBits (0),
      multisamplingLevel (0)
{
}

static auto tie (const OpenGLPixelFormat& fmt)
{
    return std::tie (fmt.redBits,
                     fmt.greenBits,
                     fmt.blueBits,
                     fmt.alphaBits,
                     fmt.depthBufferBits,
                     fmt.stencilBufferBits,
                     fmt.accumulationBufferRedBits,
                     fmt.accumulationBufferGreenBits,
                     fmt.accumulationBufferBlueBits,
                     fmt.accumulationBufferAlphaBits,
                     fmt.multisamplingLevel);
}

bool OpenGLPixelFormat::operator== (const OpenGLPixelFormat& other) const noexcept
{
    return tie (*this) == tie (other);
}

bool OpenGLPixelFormat::operator!= (const OpenGLPixelFormat& other) const noexcept
{
    return ! operator== (other);
}

} // namespace higgs
