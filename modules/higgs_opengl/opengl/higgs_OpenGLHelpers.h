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
    A set of miscellaneous openGL helper functions.

    @tags{OpenGL}
*/
class HIGGS_API  OpenGLHelpers
{
public:
    static bool isOpenGLES();

    /** Clears the GL error state. */
    static void resetErrorState();

    /** Returns true if the current thread has an active OpenGL context. */
    static bool isContextActive();

    /** Clears the current context using the given colour. */
    static void clear (Colour colour);

    static void enableScissorTest (Rectangle<int> clip);

    /** Checks whether the current context supports the specified extension. */
    static bool isExtensionSupported (const char* extensionName);

    /** Returns the address of a named GL extension function */
    static void* getExtensionFunction (const char* functionName);

    /** Returns true if the given function pointer is not null.

        This can be used to check whether a function from the higgs::gl namespace is
        available before calling it. Functions that are resolved at runtime may be missing,
        but when a function is linked statically, comparing it against nullptr directly
        triggers compiler warnings. Passing it through a pointer parameter keeps the check
        well-formed in both situations.
    */
    template <typename Fn>
    static bool isFunctionAvailable (Fn* fn) noexcept
    {
        return fn != nullptr;
    }

    /** Returns a version string such as "#version 150" suitable for prefixing a GLSL
        shader on this platform.
    */
    static String getGLSLVersionString();

    /** Makes some simple textual changes to a shader program to try to convert old GLSL
        keywords to their v3 equivalents.

        Before doing this, the function will check whether the current context is actually
        using a later version of the language, and if not it will not make any changes.
        Obviously this is not a real parser, so will only work on simple code!
    */
    static String translateVertexShaderToV3 (const String&);

    /** Makes some simple textual changes to a shader program to try to convert old GLSL
        keywords to their v3 equivalents.

        Before doing this, the function will check whether the current context is actually
        using a later version of the language, and if not it will not make any changes.
        Obviously this is not a real parser, so will only work on simple code!
    */
    static String translateFragmentShaderToV3 (const String&);
};

} // namespace higgs
