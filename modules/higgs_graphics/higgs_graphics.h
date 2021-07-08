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

  ID:                 higgs_graphics
  vendor:             higgs
  version:            9.0.5
  name:               Higgs Mythos graphics classes
  description:        Classes for 2D vector graphics, image loading/saving, font handling, etc.
  website:            https://github.com/jalexiscv/higgs-mythos
  license:            AGPLv3
  minimumCppStandard: 17

  dependencies:       higgs_events
  OSXFrameworks:      Cocoa QuartzCore
  iOSFrameworks:      CoreGraphics CoreImage CoreText QuartzCore
  linuxPackages:      freetype2 fontconfig

 END_HIGGS_MODULE_DECLARATION

*******************************************************************************/


#pragma once
#define HIGGS_GRAPHICS_H_INCLUDED

#include <higgs_core/higgs_core.h>
#include <higgs_events/higgs_events.h>

//==============================================================================
/** Config: HIGGS_USE_COREIMAGE_LOADER

    On OSX, enabling this flag means that the CoreImage codecs will be used to load
    PNG/JPEG/GIF files. It is enabled by default, but you may want to disable it if
    you'd rather use libpng, libjpeg, etc.
*/
#ifndef HIGGS_USE_COREIMAGE_LOADER
 #define HIGGS_USE_COREIMAGE_LOADER 1
#endif

/** Config: HIGGS_DISABLE_COREGRAPHICS_FONT_SMOOTHING

    Setting this flag will turn off CoreGraphics font smoothing on macOS, which some people
    find makes the text too 'fat' for their taste.
*/
#ifndef HIGGS_DISABLE_COREGRAPHICS_FONT_SMOOTHING
 #define HIGGS_DISABLE_COREGRAPHICS_FONT_SMOOTHING 0
#endif

/** Config: HIGGS_INCLUDE_PNGLIB_CODE
    This can be used to disable Higgs's embedded 3rd-party pnglib code.
    You might need to tweak this if you're linking to an external pnglib library in your app,
    but for normal apps, this option should be left alone.

    If you disable this, you might also want to set a value for HIGGS_PNGLIB_INCLUDE_PATH, to
    specify the path where your pnglib headers live.
*/
#ifndef HIGGS_INCLUDE_PNGLIB_CODE
 #define HIGGS_INCLUDE_PNGLIB_CODE 1
#endif

#ifndef HIGGS_PNGLIB_INCLUDE_PATH
 #define HIGGS_PNGLIB_INCLUDE_PATH <png.h>
#endif

/** Config: HIGGS_INCLUDE_JPEGLIB_CODE
    This can be used to disable Higgs's embedded 3rd-party jpeglib code.
    You might need to tweak this if you're linking to an external jpeglib library in your app,
    but for normal apps, this option should be left alone.

    If you disable this, you might also want to set a value for HIGGS_JPEGLIB_INCLUDE_PATH, to
    specify the path where your jpeglib headers live.
*/
#ifndef HIGGS_INCLUDE_JPEGLIB_CODE
 #define HIGGS_INCLUDE_JPEGLIB_CODE 1
#endif

#ifndef HIGGS_JPEGLIB_INCLUDE_PATH
 #define HIGGS_JPEGLIB_INCLUDE_PATH <jpeglib.h>
#endif

/** Config: HIGGS_USE_WEBP
    Enables the WebP image format class.
    If your app doesn't need to read or write WebP images, you might want to disable this to
    reduce the size of your binaries and build time.
*/
#ifndef HIGGS_USE_WEBP
 #define HIGGS_USE_WEBP 1
#endif

/** Config: HIGGS_INCLUDE_WEBPLIB_CODE
    This can be used to disable Higgs's embedded 3rd-party libwebp code.
    You might need to tweak this if you're linking to an external libwebp library in your app,
    but for normal apps, this option should be left alone.

    If you disable this, you might also want to set values for HIGGS_WEBPLIB_DECODE_INCLUDE_PATH
    and HIGGS_WEBPLIB_ENCODE_INCLUDE_PATH, to specify the paths where your libwebp headers live.

    This config option only has an effect when HIGGS_USE_WEBP is enabled.
*/
#ifndef HIGGS_INCLUDE_WEBPLIB_CODE
 #define HIGGS_INCLUDE_WEBPLIB_CODE 1
#endif

#ifndef HIGGS_WEBPLIB_DECODE_INCLUDE_PATH
 #define HIGGS_WEBPLIB_DECODE_INCLUDE_PATH <webp/decode.h>
#endif

#ifndef HIGGS_WEBPLIB_ENCODE_INCLUDE_PATH
 #define HIGGS_WEBPLIB_ENCODE_INCLUDE_PATH <webp/encode.h>
#endif

#ifndef USE_COREGRAPHICS_RENDERING
 #define USE_COREGRAPHICS_RENDERING 1
#endif

//==============================================================================
namespace higgs
{
    class Image;
    class AffineTransform;
    class Path;
    class Font;
    class Graphics;
    class FillType;
    class LowLevelGraphicsContext;
}

#include "geometry/higgs_AffineTransform.h"
#include "geometry/higgs_Point.h"
#include "geometry/higgs_Line.h"
#include "geometry/higgs_Rectangle.h"
#include "geometry/higgs_Parallelogram.h"
#include "placement/higgs_Justification.h"
#include "geometry/higgs_Path.h"
#include "geometry/higgs_RectangleList.h"
#include "colour/higgs_PixelFormats.h"
#include "colour/higgs_Colour.h"
#include "colour/higgs_ColourGradient.h"
#include "colour/higgs_Colours.h"
#include "geometry/higgs_BorderSize.h"
#include "geometry/higgs_EdgeTable.h"
#include "geometry/higgs_PathIterator.h"
#include "geometry/higgs_PathStrokeType.h"
#include "placement/higgs_RectanglePlacement.h"
#include "images/higgs_ImageCache.h"
#include "images/higgs_ImageConvolutionKernel.h"
#include "images/higgs_ImageFileFormat.h"
#include "fonts/higgs_GlyphArrangementOptions.h"
#include "contexts/higgs_GraphicsContext.h"
#include "images/higgs_Image.h"
#include "colour/higgs_FillType.h"
#include "fonts/higgs_FontFeatures.h"
#include "fonts/higgs_Typeface.h"
#include "fonts/higgs_FontOptions.h"
#include "fonts/higgs_Font.h"
#include "detail/higgs_Ranges.h"
#include "detail/higgs_SimpleShapedText.h"
#include "detail/higgs_JustifiedText.h"
#include "detail/higgs_ShapedText.h"
#include "fonts/higgs_AttributedString.h"
#include "fonts/higgs_GlyphArrangement.h"
#include "fonts/higgs_TextLayout.h"
#include "contexts/higgs_LowLevelGraphicsContext.h"
#include "contexts/higgs_ScopedBlendContext.h"
#include "images/higgs_ScaledImage.h"
#include "contexts/higgs_LowLevelGraphicsSoftwareRenderer.h"
#include "effects/higgs_ImageEffectFilter.h"
#include "effects/higgs_DropShadowEffect.h"
#include "effects/higgs_GlowEffect.h"
#include "detail/higgs_Unicode.h"
#include "detail/higgs_TwoPointConicalGradient.h"
#include "drawables/higgs_Drawable.h"
#include "drawables/higgs_StrokeOptions.h"
#include "drawables/higgs_DrawableShape.h"
#include "drawables/higgs_DrawableComposite.h"
#include "drawables/higgs_DrawableImage.h"
#include "drawables/higgs_DrawablePath.h"
#include "drawables/higgs_DrawableRectangle.h"
#include "drawables/higgs_DrawableText.h"

#if HIGGS_GRAPHICS_INCLUDE_RENDERING_HELPERS
 #include "native/higgs_RenderingHelpers.h"
#endif

#if HIGGS_GRAPHICS_INCLUDE_COREGRAPHICS_HELPERS && (HIGGS_MAC || HIGGS_IOS)
 #include "native/higgs_CoreGraphicsHelpers_mac.h"
 #include "native/higgs_CoreGraphicsContext_mac.h"
#endif
