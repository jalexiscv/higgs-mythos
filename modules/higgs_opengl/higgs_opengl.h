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


/*******************************************************************************
 The block below describes the properties of this module, and is read by
 the MythosStudio to automatically generate project code that uses it.
 For details about the syntax and how to create or use a module, see the
 Higgs Mythos Module Format.md file.


 BEGIN_HIGGS_MODULE_DECLARATION

  ID:                 higgs_opengl
  vendor:             higgs
  version:            9.0.5
  name:               Higgs Mythos OpenGL classes
  description:        Classes for rendering OpenGL in a Higgs Mythos window.
  website:            https://github.com/jalexiscv/higgs-mythos
  license:            AGPLv3
  minimumCppStandard: 17

  dependencies:       higgs_gui_extra
  OSXFrameworks:      OpenGL
  iOSFrameworks:      OpenGLES
  linuxPackages:      egl gl

 END_HIGGS_MODULE_DECLARATION

*******************************************************************************/


#pragma once
#define HIGGS_OPENGL_H_INCLUDED

#include <higgs_core/system/higgs_TargetPlatform.h>

#undef HIGGS_OPENGL
#define HIGGS_OPENGL 1

//==============================================================================
// iOS and Android only provide OpenGL ES
#if HIGGS_IOS || HIGGS_ANDROID
 #ifndef HIGGS_OPENGL_ES
  #define HIGGS_OPENGL_ES 1
 #endif
#endif

/** Config: HIGGS_OPENGL_ES
    Builds higgs_opengl against OpenGL ES rather than desktop OpenGL.

    This is only a choice on Linux and BSD, for systems whose graphics drivers only
    provide OpenGL ES, such as many embedded Arm boards. It is always enabled on iOS
    and Android and cannot be enabled on macOS or Windows.

    Building against OpenGL ES requires linking libGLESv2 rather than libGL. With
    CMake, enable it with the NEEDS_OPENGL_ES target option rather than by defining
    this macro, since that option also links the right libraries. In the MythosStudio,
    enable the module flag, or add HIGGS_OPENGL_ES=1 to the preprocessor definitions
    of a Linux exporter when the project also targets other platforms.
*/
#ifndef HIGGS_OPENGL_ES
 #define HIGGS_OPENGL_ES 0
#endif

#if (HIGGS_IOS || HIGGS_ANDROID) && ! HIGGS_OPENGL_ES
 #error "OpenGL ES is the only OpenGL API available on this platform"
#elif (HIGGS_MAC || HIGGS_WINDOWS) && HIGGS_OPENGL_ES
 #error "OpenGL ES is not available on this platform"
#endif

#if HIGGS_OPENGL_ES
#include "opengl/higgs_gles2.h"
#else
#include "opengl/higgs_gl.h"
#endif

#include <higgs_gui_extra/higgs_gui_extra.h>

//==============================================================================
#if HIGGS_OPENGL_ES || DOXYGEN
 /** This macro is a helper for use in GLSL shader code which needs to compile on both GLES and desktop GL.
     Since it's mandatory in GLES to mark a variable with a precision, but the keywords don't exist in normal GLSL,
     these macros define the various precision keywords only on GLES.
 */
 #define HIGGS_MEDIUMP  "mediump"

 /** This macro is a helper for use in GLSL shader code which needs to compile on both GLES and desktop GL.
     Since it's mandatory in GLES to mark a variable with a precision, but the keywords don't exist in normal GLSL,
     these macros define the various precision keywords only on GLES.
 */
 #define HIGGS_HIGHP    "highp"

 /** This macro is a helper for use in GLSL shader code which needs to compile on both GLES and desktop GL.
     Since it's mandatory in GLES to mark a variable with a precision, but the keywords don't exist in normal GLSL,
     these macros define the various precision keywords only on GLES.
 */
 #define HIGGS_LOWP     "lowp"
#else
 #define HIGGS_MEDIUMP
 #define HIGGS_HIGHP
 #define HIGGS_LOWP
#endif

//==============================================================================
namespace higgs
{
    class OpenGLTexture;
    class OpenGLFrameBuffer;
    class OpenGLShaderProgram;
}

#include "geometry/higgs_Vector3D.h"
#include "geometry/higgs_Matrix3D.h"
#include "geometry/higgs_Quaternion.h"
#include "geometry/higgs_Draggable3DOrientation.h"
#include "opengl/higgs_OpenGLHelpers.h"
#include "opengl/higgs_OpenGLPixelFormat.h"
#include "native/higgs_OpenGLExtensions.h"
#include "opengl/higgs_OpenGLRenderer.h"
#include "opengl/higgs_OpenGLContext.h"
#include "opengl/higgs_OpenGLFrameBuffer.h"
#include "opengl/higgs_OpenGLGraphicsContext.h"
#include "opengl/higgs_OpenGLImage.h"
#include "opengl/higgs_OpenGLShaderProgram.h"
#include "opengl/higgs_OpenGLTexture.h"
#include "utils/higgs_OpenGLAppComponent.h"
