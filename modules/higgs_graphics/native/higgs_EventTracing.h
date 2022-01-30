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

#pragma once

namespace higgs::etw
{

//==============================================================================
/*
    The following XML can be passed to Windows Performance Recorder (WPR) to enable tracing from
    HIGGS projects.
    - Save the XML into a file with the name HIGGS.wprp
    - Run the following command from an admin command prompt to start capture. If you use a Developer
      Command Prompt, wpr.exe should be found on the PATH already.
        - <path to wpr>\wpr.exe -start HIGGS.wprp
    - Start your HIGGS project
    - Run the following command from an admin command prompt to stop capture
        - <path to wpr>\wpr.exe -stop TraceCaptureFile.etl description

<?xml version="1.0" encoding="utf-8"?>
<WindowsPerformanceRecorder Version="1.0" Author="Microsoft Corporation" Copyright="Microsoft Corporation" Company="Microsoft Corporation">
  <Profiles>
    <EventCollector Id="EventCollector_HIGGSTraceLogProvider" Name="HIGGSTraceLogProvider">
      <BufferSize Value="64" />
      <Buffers Value="4" />
    </EventCollector>

    <EventProvider Id="EventProvider_HIGGSTraceLogProvider" Name="6A612E78-284D-4DDB-877A-5F521EB33132" />

    <Profile Id="HIGGSTraceLogProvider.Verbose.File" Name="HIGGSTraceLogProvider" Description="HIGGSTraceLogProvider" LoggingMode="File" DetailLevel="Verbose">
      <Collectors>
        <EventCollectorId Value="EventCollector_HIGGSTraceLogProvider">
          <EventProviders>
            <EventProviderId Value="EventProvider_HIGGSTraceLogProvider" />
          </EventProviders>
        </EventCollectorId>
      </Collectors>
    </Profile>

    <Profile Id="HIGGSTraceLogProvider.Light.File" Name="HIGGSTraceLogProvider" Description="HIGGSTraceLogProvider" Base="HIGGSTraceLogProvider.Verbose.File" LoggingMode="File" DetailLevel="Light" />
    <Profile Id="HIGGSTraceLogProvider.Verbose.Memory" Name="HIGGSTraceLogProvider" Description="HIGGSTraceLogProvider" Base="HIGGSTraceLogProvider.Verbose.File" LoggingMode="Memory" DetailLevel="Verbose" />
    <Profile Id="HIGGSTraceLogProvider.Light.Memory" Name="HIGGSTraceLogProvider" Description="HIGGSTraceLogProvider" Base="HIGGSTraceLogProvider.Verbose.File" LoggingMode="Memory" DetailLevel="Light" />

  </Profiles>
</WindowsPerformanceRecorder>

*/

//==============================================================================
enum
{
    paintKeyword            = 1 << 0,
    sizeKeyword             = 1 << 1,
    graphicsKeyword         = 1 << 2,
    crucialKeyword          = 1 << 3,
    threadPaintKeyword      = 1 << 4,
    messageKeyword          = 1 << 5,
    direct2dKeyword         = 1 << 6,
    softwareRendererKeyword = 1 << 7,
    resourcesKeyword        = 1 << 8,
    componentKeyword        = 1 << 9,
    spriteKeyword           = 1 << 10
};

enum : uint64_t
{
    direct2dHwndPaintStart,
    direct2dHwndPaintEnd,
    endDraw,
    present1SwapChainStart,
    present1SwapChainEnd,
    swapChainThreadEvent,
    waitForVBlankDone,
    callVBlankListeners,
    resize,
    createResource,
    presentIdleFrame,
    direct2dImagePaintStart,
    direct2dImagePaintEnd,
    startD2DFrame,
    flush,
    startGDIFrame,
    startGDIImage,
    endGDIFrame,
    createLowLevelGraphicsContext,
    createDeviceResources,
    createSwapChain,
    createSwapChainBuffer,
    createPeer,
    mapBitmap,
    unmapBitmap,
    createDirect2DBitmapFromImage,
    createDirect2DBitmap,
    setOrigin,
    addTransform,
    clipToRectangle,
    clipToRectangleList,
    excludeClipRectangle,
    clipToPath,
    clipToImageAlpha,
    saveState,
    restoreState,
    beginTransparencyLayer,
    endTransparencyLayer,
    setFill,
    setOpacity,
    setInterpolationQuality,
    fillRect,
    fillRectReplace,
    fillRectList,
    drawRectTranslated,
    drawRectTransformed,
    drawRect,
    fillPath,
    strokePath,
    drawPath,
    drawImage,
    drawLine,
    setFont,
    drawGlyph,
    drawGlyphRun,
    drawTextLayout,
    drawRoundedRectangle,
    fillRoundedRectangle,
    drawEllipse,
    fillEllipse,
    filledGeometryRealizationCacheHit,
    filledGeometryRealizationCreated,
    strokedGeometryRealizationCacheHit,
    strokedGeometryRealizationCreated,
    releaseGeometryRealization,
    gradientCacheHit,
    gradientCreated,
    releaseGradient,
    nativeDropShadow,
    nativeGlowEffect,
    resetToDefaultState,
    reduceClipRegionRectangle,
    reduceClipRegionRectangleList,
    reduceClipRegionImage,
    reduceClipRegionPath,
    excludeClipRegion,
    fillAll,
    repaint,
    paintComponentAndChildren,
    paintWithinParentContext,
    createSpriteBatch,
    setSprites,
    addSprites,
    drawSprites,
};

#if HIGGS_WINDOWS && HIGGS_ETW_TRACELOGGING
    HIGGS_BEGIN_IGNORE_WARNINGS_GCC_LIKE ("-Wc++98-compat-extra-semi", "-Wmissing-prototypes", "-Wgnu-zero-variadic-macro-arguments")
    TRACELOGGING_DECLARE_PROVIDER (HIGGSTraceLogProvider);
    HIGGS_END_IGNORE_WARNINGS_GCC_LIKE

    #define HIGGS_WRITE_TRACE_LOG_VA(code, keyword, ...) \
        TraceLoggingWrite (::higgs::etw::HIGGSTraceLogProvider, \
                           #code, \
                           TraceLoggingLevel (TRACE_LEVEL_INFORMATION), \
                           TraceLoggingKeyword ((UINT64) keyword), \
                           __VA_ARGS__, \
                           TraceLoggingValue ((unsigned short) code, "code"))
    #define HIGGS_WRITE_TRACE_LOG(code, keyword) \
        TraceLoggingWrite (::higgs::etw::HIGGSTraceLogProvider, \
                           #code, \
                           TraceLoggingLevel (TRACE_LEVEL_INFORMATION), \
                           TraceLoggingKeyword ((UINT64) keyword), \
                           TraceLoggingValue ((unsigned short) code, "code"))
#else
    #define HIGGS_WRITE_TRACE_LOG_VA(code, keyword, ...)
    #define HIGGS_WRITE_TRACE_LOG(code, keyword)
#endif

template <typename Number>
auto toVector (const Rectangle<Number>& r)
{
    return std::vector { r.getX(), r.getY(), r.getWidth(), r.getHeight() };
}

template <typename Number>
auto toVector (const RectangleList<Number>& list)
{
    std::vector<Number> result;

    for (const auto& r : list)
        result.insert (result.end(), { r.getX(), r.getY(), r.getWidth(), r.getHeight() });

    return result;
}

//==============================================================================
#if HIGGS_WINDOWS && HIGGS_ETW_TRACELOGGING
    #define HIGGS_SCOPED_TRACE_EVENT_FRAME(code, keyword, frameNumber) \
        const ScopeGuard scopedEvent_ ## __LINE__ { [start = ::higgs::Time::getHighResolutionTicks(), frame = (frameNumber)] \
        {                                                    \
            const auto ticks = ::higgs::Time::getHighResolutionTicks() - start; \
            HIGGS_WRITE_TRACE_LOG_VA (code, keyword, TraceLoggingValue (ticks), TraceLoggingValue (frame)); \
        } };

    #define HIGGS_SCOPED_TRACE_EVENT_FRAME_RECT_F32(code, keyword, frameNumber, rectIn) \
        const ScopeGuard scopedEvent_ ## __LINE__ { [start = ::higgs::Time::getHighResolutionTicks(), frame = (frameNumber), rect = (rectIn)] \
        { \
            const auto ticks = ::higgs::Time::getHighResolutionTicks() - start;       \
            const std::vector<float> vec = ::higgs::etw::toVector (rect); \
            HIGGS_WRITE_TRACE_LOG_VA (code,                                             \
                                     keyword,                                          \
                                     TraceLoggingValue (ticks),                        \
                                     TraceLoggingValue (frame),                        \
                                     TraceLoggingFloat32Array (vec.data(), (UINT16) vec.size(), "rect")); \
        } };

    #define HIGGS_SCOPED_TRACE_EVENT_FRAME_RECT_I32(code, keyword, frameNumber, rectIn) \
        const ScopeGuard scopedEvent_ ## __LINE__ { [start = ::higgs::Time::getHighResolutionTicks(), frame = (frameNumber), rect = (rectIn)] \
        { \
            const auto ticks = ::higgs::Time::getHighResolutionTicks() - start;       \
            const std::vector<INT32> vec = ::higgs::etw::toVector (rect); \
            HIGGS_WRITE_TRACE_LOG_VA (code,                                             \
                                     keyword,                                          \
                                     TraceLoggingValue (ticks),                        \
                                     TraceLoggingValue (frame),                        \
                                     TraceLoggingInt32Array (vec.data(), (UINT16) vec.size(), "rect")); \
        } };
#else
    #define HIGGS_SCOPED_TRACE_EVENT_FRAME(code, keyword, frameNumber)
    #define HIGGS_SCOPED_TRACE_EVENT_FRAME_RECT_F32(code, keyword, frameNumber, rectIn)
    #define HIGGS_SCOPED_TRACE_EVENT_FRAME_RECT_I32(code, keyword, frameNumber, rectIn)
#endif

//==============================================================================
#define HIGGS_TRACE_LOG_D2D_PAINT_CALL(code, frameNumber) \
    HIGGS_WRITE_TRACE_LOG_VA (code, etw::paintKeyword | etw::direct2dKeyword, TraceLoggingValue ((UINT64) frameNumber, "frame"))

#define HIGGS_TRACE_LOG_HIGGS_VBLANK_THREAD_EVENT \
    HIGGS_WRITE_TRACE_LOG (etw::waitForVBlankDone, etw::softwareRendererKeyword)

#define HIGGS_TRACE_LOG_HIGGS_VBLANK_CALL_LISTENERS \
    HIGGS_WRITE_TRACE_LOG (etw::callVBlankListeners, etw::softwareRendererKeyword)

#define HIGGS_TRACE_LOG_D2D_RESIZE(message) \
    HIGGS_WRITE_TRACE_LOG_VA (etw::resize, etw::paintKeyword | etw::direct2dKeyword, TraceLoggingValue (message, "message"))

#define HIGGS_TRACE_LOG_D2D_IMAGE_MAP_DATA \
    HIGGS_WRITE_TRACE_LOG (etw::mapBitmap, etw::direct2dKeyword)

#define HIGGS_TRACE_LOG_D2D_IMAGE_UNMAP_DATA \
    HIGGS_WRITE_TRACE_LOG (etw::unmapBitmap, etw::direct2dKeyword)

#define HIGGS_TRACE_LOG_PAINT_COMPONENT_AND_CHILDREN(depth) \
    HIGGS_WRITE_TRACE_LOG_VA (etw::paintComponentAndChildren, etw::paintKeyword, TraceLoggingValue (depth, "depth"))

#define HIGGS_TRACE_LOG_PAINT_CALL(code, frameNumber) \
    HIGGS_WRITE_TRACE_LOG_VA (code, etw::softwareRendererKeyword, TraceLoggingValue ((UINT64) frameNumber, "frame"))

#if HIGGS_ETW_TRACELOGGING
    #define HIGGS_TRACE_EVENT_INT_RECT_LIST(code, keyword, frameNumber, rect) \
        { \
            const std::vector<INT32> vec = ::higgs::etw::toVector (rect); \
            HIGGS_WRITE_TRACE_LOG_VA (code,                              \
                                     etw::softwareRendererKeyword,      \
                                     TraceLoggingValue ((UINT64) frameNumber, "frame"), \
                                     TraceLoggingInt32Array (vec.data(), (UINT16) vec.size(), "rect")); \
        }

    #define HIGGS_TRACE_EVENT_INT_RECT(code, keyword, rect) \
        { \
            const std::vector<INT32> vec = ::higgs::etw::toVector (rect); \
            HIGGS_WRITE_TRACE_LOG_VA (code,                              \
                                     etw::softwareRendererKeyword,      \
                                     TraceLoggingInt32Array (vec.data(), (UINT16) vec.size(), "rect")); \
        }
#else
    #define HIGGS_TRACE_EVENT_INT_RECT_LIST(code, keyword, frameNumber, rect)
    #define HIGGS_TRACE_EVENT_INT_RECT(code, keyword, rect)
#endif

} // namespace higgs::etw
