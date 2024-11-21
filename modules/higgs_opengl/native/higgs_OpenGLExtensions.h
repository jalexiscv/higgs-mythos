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

/** @internal This macro contains a list of GL extension functions that need to be dynamically loaded on Windows/Linux.
    @see OpenGLExtensionFunctions
*/
#define HIGGS_GL_BASE_FUNCTIONS \
    X (glActiveTexture) \
    X (glBindBuffer) \
    X (glDeleteBuffers) \
    X (glGenBuffers) \
    X (glBufferData) \
    X (glBufferSubData) \
    X (glCreateProgram) \
    X (glDeleteProgram) \
    X (glCreateShader) \
    X (glDeleteShader) \
    X (glShaderSource) \
    X (glCompileShader) \
    X (glAttachShader) \
    X (glLinkProgram) \
    X (glUseProgram) \
    X (glGetShaderiv) \
    X (glGetShaderInfoLog) \
    X (glGetProgramInfoLog) \
    X (glGetProgramiv) \
    X (glGetUniformLocation) \
    X (glGetAttribLocation) \
    X (glVertexAttribPointer) \
    X (glEnableVertexAttribArray) \
    X (glDisableVertexAttribArray) \
    X (glUniform1f) \
    X (glUniform1i) \
    X (glUniform2f) \
    X (glUniform3f) \
    X (glUniform4f) \
    X (glUniform4i) \
    X (glUniform1fv) \
    X (glUniformMatrix2fv) \
    X (glUniformMatrix3fv) \
    X (glUniformMatrix4fv) \
    X (glBindAttribLocation)

/** @internal This macro contains a list of GL extension functions that need to be dynamically loaded on Windows/Linux.
    @see OpenGLExtensionFunctions
*/
#define HIGGS_GL_EXTENSION_FUNCTIONS \
    X (glIsRenderbuffer) \
    X (glBindRenderbuffer) \
    X (glDeleteRenderbuffers) \
    X (glGenRenderbuffers) \
    X (glRenderbufferStorage) \
    X (glGetRenderbufferParameteriv) \
    X (glIsFramebuffer) \
    X (glBindFramebuffer) \
    X (glDeleteFramebuffers) \
    X (glGenFramebuffers) \
    X (glCheckFramebufferStatus) \
    X (glFramebufferTexture2D) \
    X (glFramebufferRenderbuffer) \
    X (glGetFramebufferAttachmentParameteriv)

/** @internal This macro contains a list of GL extension functions that need to be dynamically loaded on Windows/Linux.
    @see OpenGLExtensionFunctions
*/
#define HIGGS_GL_VERTEXBUFFER_FUNCTIONS \
    X (glGenVertexArrays) \
    X (glDeleteVertexArrays) \
    X (glBindVertexArray)

/** This class contains a generated list of OpenGL extension functions, which are either dynamically loaded
    for a specific GL context, or simply call-through to the appropriate OS function where available.

    This class is provided for backwards compatibility. In new code, you should prefer to use
    functions from the higgs::gl namespace. By importing all these symbols with
    `using namespace ::higgs::gl;`, all GL enumerations and functions will be made available at
    global scope. This may be helpful if you need to write code with C source compatibility, or
    which is compatible with a different extension-loading library.
    All the normal guidance about `using namespace` should still apply - don't do this in a header,
    or at all if you can possibly avoid it!

    @tags{OpenGL}
*/
struct OpenGLExtensionFunctions
{
    //==============================================================================
    /** @cond */
    [[deprecated ("A more complete set of GL commands can be found in the higgs::gl namespace. "
                  "You should use higgs::gl::loadFunctions() to load GL functions.")]]
    static void initialise();
    /** @endcond */

   #if HIGGS_WINDOWS
    /** @cond */
    typedef char GLchar;
    typedef pointer_sized_int GLsizeiptr;
    typedef pointer_sized_int GLintptr;
    /** @endcond */
   #endif

   #define X(name) static decltype (::higgs::gl::name)& name;
    HIGGS_GL_BASE_FUNCTIONS
    HIGGS_GL_EXTENSION_FUNCTIONS
    HIGGS_GL_VERTEXBUFFER_FUNCTIONS
   #undef X
};

enum MissingOpenGLDefinitions
{
   #if HIGGS_ANDROID
    HIGGS_RGBA_FORMAT                = ::higgs::gl::GL_RGBA,
   #else
    HIGGS_RGBA_FORMAT                = ::higgs::gl::GL_BGRA_EXT,
   #endif
};

} // namespace higgs
