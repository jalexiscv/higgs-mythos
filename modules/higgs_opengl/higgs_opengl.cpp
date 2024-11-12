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

#ifdef HIGGS_OPENGL_H_INCLUDED
 /* When you add this cpp file to your project, you mustn't include it in a file where you've
    already included any other headers - just put it inside a file on its own, possibly with your config
    flags preceding it, but don't include anything else. That also includes avoiding any automatic prefix
    header files that the compiler may be using.
 */
 #error "Incorrect use of HIGGS cpp file"
#endif

#define HIGGS_CORE_INCLUDE_OBJC_HELPERS 1
#define HIGGS_CORE_INCLUDE_JNI_HELPERS 1
#define HIGGS_CORE_INCLUDE_NATIVE_HEADERS 1
#define HIGGS_GRAPHICS_INCLUDE_COREGRAPHICS_HELPERS 1
#define HIGGS_GRAPHICS_INCLUDE_RENDERING_HELPERS 1
#define HIGGS_GUI_BASICS_INCLUDE_XHEADERS 1
#define HIGGS_GUI_BASICS_INCLUDE_SCOPED_THREAD_DPI_AWARENESS_SETTER 1

#include "higgs_opengl.h"

#define HIGGS_STATIC_LINK_GL_VERSION_1_0 1
#define HIGGS_STATIC_LINK_GL_VERSION_1_1 1

#if HIGGS_MAC
 #define HIGGS_STATIC_LINK_GL_VERSION_1_2 1
 #define HIGGS_STATIC_LINK_GL_VERSION_1_3 1
 #define HIGGS_STATIC_LINK_GL_VERSION_1_4 1
 #define HIGGS_STATIC_LINK_GL_VERSION_1_5 1
 #define HIGGS_STATIC_LINK_GL_VERSION_2_0 1
 #define HIGGS_STATIC_LINK_GL_VERSION_2_1 1
 #define HIGGS_STATIC_LINK_GL_VERSION_3_0 1
 #define HIGGS_STATIC_LINK_GL_VERSION_3_1 1
 #define HIGGS_STATIC_LINK_GL_VERSION_3_2 1
#endif

#define HIGGS_STATIC_LINK_GL_ES_VERSION_2_0 1
#if ! HIGGS_ANDROID || HIGGS_ANDROID_GL_ES_VERSION_3_0
#define HIGGS_STATIC_LINK_GL_ES_VERSION_3_0 1
#endif

#if HIGGS_OPENGL_ES
 #include "opengl/higgs_gles2.cpp"
#else
 #include "opengl/higgs_gl.cpp"
#endif

//==============================================================================
#if HIGGS_IOS
 #import <QuartzCore/QuartzCore.h>

//==============================================================================
#elif HIGGS_WINDOWS
 #include <windowsx.h>

 #if ! HIGGS_DONT_AUTOLINK_TO_WIN32_LIBRARIES
  #pragma comment(lib, "OpenGL32.Lib")
 #endif

//==============================================================================
#elif HIGGS_LINUX || HIGGS_BSD
 /* Got an include error here?

    If you want to install OpenGL support, the packages to get are "mesa-common-dev",
    "freeglut3-dev", and "libegl-dev".
 */
 #include <EGL/egl.h>
 #include <EGL/eglext.h>

//==============================================================================
#elif HIGGS_MAC
 #include <OpenGL/CGLCurrent.h> // These are both just needed with the 10.5 SDK
 #include <OpenGL/OpenGL.h>

//==============================================================================
#elif HIGGS_ANDROID
 #include <android/native_window.h>
 #include <android/native_window_jni.h>
 #include <EGL/egl.h>
#endif

//==============================================================================
namespace higgs
{

using namespace ::higgs::gl;

void OpenGLExtensionFunctions::initialise()
{
    gl::loadFunctions();
}

#define X(name) decltype (::higgs::gl::name)& OpenGLExtensionFunctions::name = ::higgs::gl::name;
HIGGS_GL_BASE_FUNCTIONS
HIGGS_GL_EXTENSION_FUNCTIONS
HIGGS_GL_VERTEXBUFFER_FUNCTIONS
#undef X

#if HIGGS_DEBUG && ! HIGGS_DISABLE_ASSERTIONS && ! defined (HIGGS_CHECK_OPENGL_ERROR)
static const char* getGLErrorMessage (const GLenum e) noexcept
{
    switch (e)
    {
        case GL_INVALID_ENUM:                   return "GL_INVALID_ENUM";
        case GL_INVALID_VALUE:                  return "GL_INVALID_VALUE";
        case GL_INVALID_OPERATION:              return "GL_INVALID_OPERATION";
        case GL_OUT_OF_MEMORY:                  return "GL_OUT_OF_MEMORY";
       #ifdef GL_STACK_OVERFLOW
        case GL_STACK_OVERFLOW:                 return "GL_STACK_OVERFLOW";
       #endif
       #ifdef GL_STACK_UNDERFLOW
        case GL_STACK_UNDERFLOW:                return "GL_STACK_UNDERFLOW";
       #endif
       #ifdef GL_INVALID_FRAMEBUFFER_OPERATION
        case GL_INVALID_FRAMEBUFFER_OPERATION:  return "GL_INVALID_FRAMEBUFFER_OPERATION";
       #endif
        default: break;
    }

    return "Unknown error";
}

#if HIGGS_MAC || HIGGS_IOS

 #ifndef HIGGS_IOS_MAC_VIEW
  #if HIGGS_IOS
   #define HIGGS_IOS_MAC_VIEW    UIView
   #define HIGGS_IOS_MAC_WINDOW  UIWindow
  #else
   #define HIGGS_IOS_MAC_VIEW    NSView
   #define HIGGS_IOS_MAC_WINDOW  NSWindow
  #endif
 #endif

#endif

static bool checkPeerIsValid (OpenGLContext* context)
{
    jassert (context != nullptr);

    if (context != nullptr)
    {
        if (auto* comp = context->getTargetComponent())
        {
            if (auto* peer [[maybe_unused]] = comp->getPeer())
            {
               #if HIGGS_MAC || HIGGS_IOS
                if (auto* nsView = (HIGGS_IOS_MAC_VIEW*) peer->getNativeHandle())
                {
                    if ([[maybe_unused]] auto nsWindow = [nsView window])
                    {
                       #if HIGGS_MAC
                        return ([nsWindow isVisible]
                                  && (! [nsWindow hidesOnDeactivate] || [NSApp isActive]));
                       #else
                        return true;
                       #endif
                    }
                }
               #else
                return true;
               #endif
            }
        }
    }

    return false;
}

static void checkGLError ([[maybe_unused]] const char* file, [[maybe_unused]] const int line)
{
    for (;;)
    {
        const GLenum e = glGetError();

        if (e == GL_NO_ERROR)
            break;

        // if the peer is not valid then ignore errors
        if (! checkPeerIsValid (OpenGLContext::getCurrentContext()))
            continue;

        DBG ("***** " << getGLErrorMessage (e) << "  at " << file << " : " << line);
        jassertfalse;
    }
}

 #define HIGGS_CHECK_OPENGL_ERROR checkGLError (__FILE__, __LINE__);
#else
 #define HIGGS_CHECK_OPENGL_ERROR ;
#endif

static void clearDebugGLError() noexcept
{
   #if HIGGS_DEBUG
    OpenGLHelpers::resetErrorState();
   #endif
}

struct OpenGLTargetSaver
{
    OpenGLTargetSaver() noexcept
        : oldFramebuffer (OpenGLFrameBuffer::getCurrentFrameBufferTarget())
    {
        glGetIntegerv (GL_VIEWPORT, oldViewport);
    }

    ~OpenGLTargetSaver() noexcept
    {
        gl::glBindFramebuffer (GL_FRAMEBUFFER, oldFramebuffer);
        glViewport (oldViewport[0], oldViewport[1], oldViewport[2], oldViewport[3]);
    }

    HIGGS_DECLARE_NON_COPYABLE (OpenGLTargetSaver)
    HIGGS_DECLARE_NON_MOVEABLE (OpenGLTargetSaver)

private:
    GLuint oldFramebuffer;
    GLint oldViewport[4];
};

} // namespace higgs

//==============================================================================
#include "opengl/higgs_OpenGLFrameBuffer.cpp"
#include "opengl/higgs_OpenGLHelpers.cpp"
#include "opengl/higgs_OpenGLGraphicsContext.cpp"
#include "opengl/higgs_OpenGLImage.cpp"
#include "opengl/higgs_OpenGLPixelFormat.cpp"
#include "opengl/higgs_OpenGLShaderProgram.cpp"
#include "opengl/higgs_OpenGLTexture.cpp"

//==============================================================================
#if HIGGS_MAC || HIGGS_IOS

 #if HIGGS_MAC
  #include "native/higgs_OpenGL_mac.h"
 #else
  #include "native/higgs_OpenGL_ios.h"
 #endif

#elif HIGGS_WINDOWS
 #include "opengl/higgs_wgl.h"
 #include "native/higgs_OpenGL_windows.h"

#define HIGGS_IMPL_WGL_EXTENSION_FUNCTION(name) \
    decltype (higgs::OpenGLContext::NativeContext::name) higgs::OpenGLContext::NativeContext::name = nullptr;

HIGGS_IMPL_WGL_EXTENSION_FUNCTION (wglChoosePixelFormatARB)
HIGGS_IMPL_WGL_EXTENSION_FUNCTION (wglSwapIntervalEXT)
HIGGS_IMPL_WGL_EXTENSION_FUNCTION (wglGetSwapIntervalEXT)
HIGGS_IMPL_WGL_EXTENSION_FUNCTION (wglCreateContextAttribsARB)

#undef HIGGS_IMPL_WGL_EXTENSION_FUNCTION

#elif HIGGS_LINUX || HIGGS_BSD
 #include <higgs_gui_basics/native/higgs_ScopedWindowAssociation_linux.h>
 #include "native/higgs_OpenGL_linux.h"

#elif HIGGS_ANDROID
 #include "native/higgs_OpenGL_android.h"

#endif

#include "opengl/higgs_OpenGLContext.cpp"
#include "utils/higgs_OpenGLAppComponent.cpp"
