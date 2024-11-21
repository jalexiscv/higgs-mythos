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

/** Creates a graphics context object that will render into the given OpenGL target. */
std::unique_ptr<LowLevelGraphicsContext> createOpenGLGraphicsContext (OpenGLContext&, int width, int height);

/** Creates a graphics context object that will render into the given OpenGL framebuffer. */
std::unique_ptr<LowLevelGraphicsContext> createOpenGLGraphicsContext (OpenGLContext&, OpenGLFrameBuffer&);

/** Creates a graphics context object that will render into the given OpenGL framebuffer,
    with the given size.
*/
std::unique_ptr<LowLevelGraphicsContext> createOpenGLGraphicsContext (OpenGLContext&,
                                                                      unsigned int frameBufferID,
                                                                      int width, int height);


//==============================================================================
/**
    Used to create custom shaders for use with an openGL 2D rendering context.

    Given a GL-based rendering context, you can write a fragment shader that applies some
    kind of per-pixel effect.

    @tags{OpenGL}
*/
struct HIGGS_API  OpenGLGraphicsContextCustomShader
{
    /** Creates a custom shader.

        The shader code will not be compiled until actually needed, so it's OK to call this
        constructor when no GL context is active.

        The code should be a normal fragment shader. As well as the usual GLSL variables, there is
        also an automatically declared varying vec2 called "pixelPos", which indicates the pixel
        position within the graphics context of the pixel being drawn. There is also a varying value
        "pixelAlpha", which indicates the alpha by which the pixel should be multiplied, so that the
        edges of any clip-region masks are anti-aliased correctly.
    */
    OpenGLGraphicsContextCustomShader (const String& fragmentShaderCode);

    /** Destructor. */
    ~OpenGLGraphicsContextCustomShader();

    /** Returns the program, if it has been linked and is active.
        This can be called when you're about to use fillRect, to set up any uniforms/textures that
        the program may require.
    */
    OpenGLShaderProgram* getProgram (LowLevelGraphicsContext&) const;

    /** Applies the shader to a rectangle within the graphics context. */
    void fillRect (LowLevelGraphicsContext&, Rectangle<int> area) const;

    /** Attempts to compile the program if necessary, and returns an error message if it fails. */
    Result checkCompilation (LowLevelGraphicsContext&);

    /** Returns the code that was used to create this object. */
    const String& getFragmentShaderCode() const noexcept           { return code; }

    /** Optional lambda that will be called when the shader is activated, to allow
        user code to do setup tasks.
    */
    std::function<void (OpenGLShaderProgram&)> onShaderActivated;

private:
    String code, hashName;

    HIGGS_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (OpenGLGraphicsContextCustomShader)
};

} // namespace higgs
