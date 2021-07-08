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

#ifdef HIGGS_GRAPHICS_H_INCLUDED
 /* When you add this cpp file to your project, you mustn't include it in a file where you've
    already included any other headers - just put it inside a file on its own, possibly with your config
    flags preceding it, but don't include anything else. That also includes avoiding any automatic prefix
    header files that the compiler may be using.
 */
 #error "Incorrect use of HIGGS cpp file"
#endif

#define HIGGS_CORE_INCLUDE_OBJC_HELPERS 1
#define HIGGS_CORE_INCLUDE_COM_SMART_PTR 1
#define HIGGS_CORE_INCLUDE_JNI_HELPERS 1
#define HIGGS_CORE_INCLUDE_NATIVE_HEADERS 1
#define HIGGS_GRAPHICS_INCLUDE_COREGRAPHICS_HELPERS 1
#define HIGGS_GRAPHICS_INCLUDE_RENDERING_HELPERS 1

#include "higgs_graphics.h"

#include <higgs_core/detail/higgs_FunctionPointerDestructor.h>

//==============================================================================
#if HIGGS_MAC
 #import <QuartzCore/QuartzCore.h>
 #include <CoreImage/CoreImage.h>
 #include <CoreText/CTFont.h>

#elif HIGGS_WINDOWS
 // get rid of some warnings in Window's own headers
 HIGGS_BEGIN_IGNORE_WARNINGS_MSVC (4458)

 /* If you hit a compile error trying to include these files, you may need to update
    your version of the Windows SDK to the latest one. The DirectWrite and Direct2D
    headers are in the version 8 SDKs.

    Need Direct2D 1.3 for sprite batching
 */

 #include <d2d1_3.h>
 #include <d3d11_2.h>
 #include <dcomp.h>
 #include <dwrite_3.h>
 #include <dxgi1_3.h>
 #include <processthreadsapi.h>

 #if HIGGS_ETW_TRACELOGGING
  #include <evntrace.h>
  #include <TraceLoggingProvider.h>

  TRACELOGGING_DEFINE_PROVIDER (HIGGSTraceLogProvider,
                                "HIGGSTraceLogProvider",
                                // {6A612E78-284D-4DDB-877A-5F521EB33132}
                                (0x6a612e78, 0x284d, 0x4ddb, 0x87, 0x7a, 0x5f, 0x52, 0x1e, 0xb3, 0x31, 0x32));

#endif

 HIGGS_END_IGNORE_WARNINGS_MSVC

 #if ! HIGGS_DONT_AUTOLINK_TO_WIN32_LIBRARIES
  #pragma comment(lib, "Dwrite.lib")
  #pragma comment(lib, "D2d1.lib")
  #pragma comment(lib, "DXGI.lib")
  #pragma comment(lib, "D3D11.lib")
  #pragma comment(lib, "DComp.lib")
  #pragma comment(lib, "dxguid.lib")
 #endif

 #include "native/higgs_Direct2DPixelDataPage_windows.h"

#elif HIGGS_IOS
 #import <QuartzCore/QuartzCore.h>
 #import <CoreText/CoreText.h>

#elif HIGGS_LINUX || HIGGS_BSD
 #ifndef HIGGS_USE_FREETYPE
  #define HIGGS_USE_FREETYPE 1
 #endif

 #ifndef HIGGS_USE_FONTCONFIG
  #define HIGGS_USE_FONTCONFIG 1
 #endif
#elif HIGGS_ANDROID
 #include <android/font_matcher.h>
 #include <android/system_fonts.h>
#endif

#if HIGGS_USE_FREETYPE
 #include <ft2build.h>
 #include FT_FREETYPE_H
 #include FT_ADVANCES_H
 #include FT_TRUETYPE_TABLES_H
 #include FT_MULTIPLE_MASTERS_H
 #include FT_GLYPH_H

 #ifdef FT_COLOR_H
  #include FT_COLOR_H
 #endif
#endif

#if HIGGS_USE_FONTCONFIG
 #include <fontconfig/fontconfig.h>
#endif

#undef SIZEOF

#if (HIGGS_MAC || HIGGS_IOS) && USE_COREGRAPHICS_RENDERING && HIGGS_USE_COREIMAGE_LOADER
 #define HIGGS_USING_COREIMAGE_LOADER 1
#else
 #define HIGGS_USING_COREIMAGE_LOADER 0
#endif

#if HIGGS_USE_FREETYPE
 #include <higgs_graphics/fonts/harfbuzz/hb-ft.h>
#endif

#if HIGGS_WINDOWS
 #include <higgs_graphics/fonts/harfbuzz/hb-directwrite.h>
#elif HIGGS_MAC || HIGGS_IOS
 #include <higgs_graphics/fonts/harfbuzz/hb-coretext.h>
#endif

#include <higgs_graphics/fonts/harfbuzz/hb-ot.h>

extern "C"
{
#include <higgs_graphics/unicode/sheenbidi/Headers/SheenBidi/SheenBidi.h>
} // extern "C"

#if HIGGS_UNIT_TESTS
 #include "fonts/higgs_TypefaceTestData.cpp"
#endif

//==============================================================================
#include "higgs_core/zip/higgs_zlib.h"

#include "native/higgs_EventTracing.h"
#include "images/higgs_ImagePixelDataNativeExtensions.h"

#include "unicode/higgs_UnicodeGenerated.cpp"
#include "unicode/higgs_UnicodeUtils.cpp"
#include "unicode/higgs_UnicodeLine.cpp"
#include "unicode/higgs_UnicodeScript.cpp"
#include "unicode/higgs_UnicodeBidi.cpp"
#include "unicode/higgs_Unicode.cpp"
#include "colour/higgs_Colour.cpp"
#include "detail/higgs_TwoPointConicalGradient.cpp"
#include "colour/higgs_ColourGradient.cpp"
#include "colour/higgs_Colours.cpp"
#include "colour/higgs_PixelFormats.cpp"
#include "colour/higgs_FillType.cpp"
#include "geometry/higgs_AffineTransform.cpp"
#include "geometry/higgs_EdgeTable.cpp"
#include "geometry/higgs_Path.cpp"
#include "geometry/higgs_PathIterator.cpp"
#include "geometry/higgs_PathStrokeType.cpp"
#include "placement/higgs_RectanglePlacement.cpp"
#include "contexts/higgs_GraphicsContext.cpp"
#include "contexts/higgs_LowLevelGraphicsSoftwareRenderer.cpp"
#include "contexts/higgs_ScopedBlendContext.cpp"
#include "images/higgs_Image.cpp"
#include "images/higgs_ImageCache.cpp"
#include "images/higgs_ImageConvolutionKernel.cpp"
#include "images/higgs_ImageFileFormat.cpp"
#include "image_formats/higgs_GIFLoader.cpp"
#include "image_formats/higgs_JPEGLoader.cpp"
#include "image_formats/higgs_PNGLoader.cpp"
#include "image_formats/higgs_WebPLoader.cpp"
#include "fonts/higgs_AttributedString.cpp"
#include "fonts/higgs_FontComparators.h"
#include "fonts/higgs_Typeface.cpp"
#include "fonts/higgs_FontFeatures.cpp"
#include "fonts/higgs_FontOptions.cpp"
#include "fonts/higgs_Font.cpp"
#include "detail/higgs_Ranges.cpp"
#include "detail/higgs_SimpleShapedText.cpp"
#include "detail/higgs_JustifiedText.cpp"
#include "detail/higgs_ShapedText.cpp"
#include "fonts/higgs_GlyphArrangementOptions.cpp"
#include "fonts/higgs_GlyphArrangement.cpp"
#include "fonts/higgs_TextLayout.cpp"
#include "effects/higgs_DropShadowEffect.cpp"
#include "effects/higgs_GlowEffect.cpp"


#include "drawables/higgs_StrokeOptions.cpp"
#include "drawables/higgs_Drawable.cpp"
#include "drawables/higgs_DrawableComposite.cpp"
#include "drawables/higgs_DrawableImage.cpp"
#include "drawables/higgs_DrawablePath.cpp"
#include "drawables/higgs_DrawableRectangle.cpp"
#include "drawables/higgs_DrawableShape.cpp"
#include "drawables/higgs_DrawableText.cpp"

#include "detail/higgs_LunaSvgFontReplacement.cpp"

// A project may be linking against lunasvg in which case this may already be defined on the command-line
#ifndef LUNASVG_BUILD
 #define LUNASVG_BUILD
#endif
#ifndef LUNASVG_BUILD_STATIC
 #define LUNASVG_BUILD_STATIC
#endif
#define HIGGS_PLUTOVG_BUILD
#define HIGGS_PLUTOVG_BUILD_STATIC

HIGGS_BEGIN_IGNORE_WARNINGS_MSVC (4100 4244 4267 6323)
HIGGS_BEGIN_IGNORE_WARNINGS_GCC_LIKE ("-Wfloat-conversion",
                                     "-Wfloat-equal",
                                     "-Wmissing-field-initializers",
                                     "-Wshadow-field-in-constructor",
                                     "-Wshadow-uncaptured-local",
                                     "-Wshorten-64-to-32",
                                     "-Wsign-conversion",
                                     "-Wswitch-enum",
                                     "-Wunused-parameter",
                                     "-Wimplicit-int-float-conversion",
                                     "-Wshadow",
                                     "-Wunused-function",
                                     "-Wignored-qualifiers")

#include "drawables/lunasvg/include/lunasvg.h"
#include "drawables/lunasvg/source/graphics.h"

#include "drawables/lunasvg/source/graphics.cpp"
#include "drawables/lunasvg/source/lunasvg.cpp"
#include "drawables/lunasvg/source/svgelement.cpp"
#include "drawables/lunasvg/source/svggeometryelement.cpp"
#include "drawables/lunasvg/source/svglayoutstate.cpp"
#include "drawables/lunasvg/source/svgpaintelement.cpp"
#include "drawables/lunasvg/source/svgparser.cpp"
#include "drawables/lunasvg/source/svgproperty.cpp"
#include "drawables/lunasvg/source/svgrenderstate.cpp"
#include "drawables/lunasvg/source/svgtextelement.cpp"

HIGGS_END_IGNORE_WARNINGS_GCC_LIKE
HIGGS_END_IGNORE_WARNINGS_MSVC

#undef HIGGS_PLUTOVG_BUILD_STATIC
#undef HIGGS_PLUTOVG_BUILD
#undef LUNASVG_BUILD_STATIC
#undef LUNASVG_BUILD

#include "drawables/higgs_SVGParser.cpp"

#if HIGGS_UNIT_TESTS
 #include "geometry/higgs_Parallelogram_test.cpp"
 #include "geometry/higgs_Rectangle_test.cpp"
 #include "geometry/higgs_RectangleList_test.cpp"
#endif

#if HIGGS_USE_FREETYPE
 #include "fonts/higgs_TypefaceFileCache.h"
 #include "native/higgs_Fonts_freetype.cpp"
#endif

//==============================================================================
#if HIGGS_MAC || HIGGS_IOS
 #include "native/higgs_Fonts_mac.mm"
 #include "native/higgs_CoreGraphicsContext_mac.mm"
 #include "native/higgs_IconHelpers_mac.cpp"

#elif HIGGS_WINDOWS
 #include "native/higgs_Direct2DMetrics_windows.h"
 #include "native/higgs_Direct2DGraphicsContext_windows.h"
 #include "native/higgs_DirectX_windows.h"
 #include "native/higgs_Direct2DImage_windows.h"
 #include "native/higgs_Direct2DGraphicsContextImpl_windows.h"
 #include "native/higgs_Direct2DImageContext_windows.h"

 #include "native/higgs_DirectX_windows.cpp"
 #include "native/higgs_DirectWriteTypeface_windows.cpp"
 #include "native/higgs_IconHelpers_windows.cpp"
 #include "native/higgs_Direct2DGraphicsContextImpl_windows.cpp"
 #include "native/higgs_Direct2DGraphicsContext_windows.cpp"
 #include "native/higgs_Direct2DImageContext_windows.cpp"
 #include "native/higgs_Direct2DImage_windows.cpp"
 #include "native/higgs_Direct2DMetrics_windows.cpp"

#elif HIGGS_LINUX || HIGGS_BSD
 #include "native/higgs_Fonts_linux.cpp"
 #include "native/higgs_IconHelpers_linux.cpp"

#elif HIGGS_ANDROID
 #include "fonts/higgs_TypefaceFileCache.h"
 #include "native/higgs_GraphicsContext_android.cpp"
 #include "native/higgs_Fonts_android.cpp"
 #include "native/higgs_IconHelpers_android.cpp"

#endif
