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
namespace ReturnHelpers
{
    template <typename Type>
    Type returnDefaultConstructedAnyType()               { return {}; }

    template <>
    inline void returnDefaultConstructedAnyType<void>()  {}
}

#define HIGGS_GENERATE_FUNCTION_WITH_DEFAULT(functionName, objectName, args, returnType) \
    using functionName      = returnType (*) args; \
    functionName objectName = [] args -> returnType  { return ReturnHelpers::returnDefaultConstructedAnyType<returnType>(); };

#define HIGGS_GENERATE_FUNCTION_WITH_DEFAULT_RESULT(functionName, objectName, args, returnType, defaultResult) \
    using functionName      = returnType (*) args; \
    functionName objectName = [] args -> returnType  { return defaultResult; };


//==============================================================================
class HIGGS_API  X11Symbols
{
public:
    //==============================================================================
    bool loadAllSymbols();

    //==============================================================================
    HIGGS_GENERATE_FUNCTION_WITH_DEFAULT (XAllocClassHint, xAllocClassHint,
                                         (),
                                         XClassHint*)

    HIGGS_GENERATE_FUNCTION_WITH_DEFAULT (XAllocSizeHints, xAllocSizeHints,
                                         (),
                                         XSizeHints*)

    HIGGS_GENERATE_FUNCTION_WITH_DEFAULT (XAllocWMHints, xAllocWMHints,
                                         (),
                                         XWMHints*)

    HIGGS_GENERATE_FUNCTION_WITH_DEFAULT (XBitmapBitOrder, xBitmapBitOrder,
                                         (::Display*),
                                         int)

    HIGGS_GENERATE_FUNCTION_WITH_DEFAULT (XBitmapUnit, xBitmapUnit,
                                         (::Display*),
                                         int)

    HIGGS_GENERATE_FUNCTION_WITH_DEFAULT (XChangeActivePointerGrab, xChangeActivePointerGrab,
                                         (::Display*, unsigned int, Cursor, ::Time),
                                         void)

    HIGGS_GENERATE_FUNCTION_WITH_DEFAULT (XChangeProperty, xChangeProperty,
                                         (::Display*, ::Window, Atom, Atom, int, int, const unsigned char*, int),
                                         void)

    HIGGS_GENERATE_FUNCTION_WITH_DEFAULT (XCheckTypedWindowEvent, xCheckTypedWindowEvent,
                                         (::Display*, ::Window, int, XEvent*),
                                         Bool)

    HIGGS_GENERATE_FUNCTION_WITH_DEFAULT (XCheckWindowEvent, xCheckWindowEvent,
                                         (::Display*, ::Window, long, XEvent*),
                                         Bool)

    HIGGS_GENERATE_FUNCTION_WITH_DEFAULT (XClearArea, xClearArea,
                                         (::Display*, ::Window, int, int, unsigned int, unsigned int, Bool),
                                         void)

    HIGGS_GENERATE_FUNCTION_WITH_DEFAULT (XCloseDisplay, xCloseDisplay,
                                         (::Display*),
                                         int)

    HIGGS_GENERATE_FUNCTION_WITH_DEFAULT (XConnectionNumber, xConnectionNumber,
                                         (::Display*),
                                         int)

    HIGGS_GENERATE_FUNCTION_WITH_DEFAULT (XConvertSelection, xConvertSelection,
                                         (::Display*, Atom, Atom, Atom, ::Window, ::Time),
                                         void)

    HIGGS_GENERATE_FUNCTION_WITH_DEFAULT (XCreateColormap, xCreateColormap,
                                         (::Display*, ::Window, Visual*, int),
                                         Colormap)

    HIGGS_GENERATE_FUNCTION_WITH_DEFAULT (XCreateFontCursor, xCreateFontCursor,
                                         (::Display*, unsigned int),
                                         Cursor)

    HIGGS_GENERATE_FUNCTION_WITH_DEFAULT (XCreateGC, xCreateGC,
                                         (::Display*, ::Drawable, unsigned long, XGCValues*),
                                         GC)

    HIGGS_GENERATE_FUNCTION_WITH_DEFAULT (XCreateImage, xCreateImage,
                                         (::Display*, Visual*, unsigned int, int, int, const char*, unsigned int, unsigned int, int, int),
                                         XImage*)

    HIGGS_GENERATE_FUNCTION_WITH_DEFAULT (XCreatePixmap, xCreatePixmap,
                                         (::Display*, ::Drawable, unsigned int, unsigned int, unsigned int),
                                         Pixmap)

    HIGGS_GENERATE_FUNCTION_WITH_DEFAULT (XCreatePixmapCursor, xCreatePixmapCursor,
                                         (::Display*, Pixmap, Pixmap, XColor*, XColor*, unsigned int, unsigned int),
                                         Cursor)

    HIGGS_GENERATE_FUNCTION_WITH_DEFAULT (XCreatePixmapFromBitmapData, xCreatePixmapFromBitmapData,
                                         (::Display*, ::Drawable, const char*, unsigned int, unsigned int, unsigned long, unsigned long, unsigned int),
                                         Pixmap)

    HIGGS_GENERATE_FUNCTION_WITH_DEFAULT (XCreateWindow, xCreateWindow,
                                         (::Display*, ::Window, int, int, unsigned int, unsigned int, unsigned int, int, unsigned int, Visual*, unsigned long, XSetWindowAttributes*),
                                         ::Window)

    HIGGS_GENERATE_FUNCTION_WITH_DEFAULT (XDefaultRootWindow, xDefaultRootWindow,
                                         (::Display*),
                                         ::Window)

    HIGGS_GENERATE_FUNCTION_WITH_DEFAULT (XDefaultScreen, xDefaultScreen,
                                         (::Display*),
                                         int)

    HIGGS_GENERATE_FUNCTION_WITH_DEFAULT (XDefaultScreenOfDisplay, xDefaultScreenOfDisplay,
                                         (::Display*),
                                         Screen*)

    HIGGS_GENERATE_FUNCTION_WITH_DEFAULT (XDefaultVisual, xDefaultVisual,
                                         (::Display*, int),
                                         Visual*)

    HIGGS_GENERATE_FUNCTION_WITH_DEFAULT (XDefineCursor, xDefineCursor,
                                         (::Display*, ::Window, Cursor),
                                         int)

    HIGGS_GENERATE_FUNCTION_WITH_DEFAULT (XDeleteContext, xDeleteContext,
                                         (::Display*, XID, XContext),
                                         int)

    HIGGS_GENERATE_FUNCTION_WITH_DEFAULT (XDeleteProperty, xDeleteProperty,
                                         (::Display*, Window, Atom),
                                         void)

    HIGGS_GENERATE_FUNCTION_WITH_DEFAULT (XDestroyImage, xDestroyImage,
                                         (XImage*),
                                         void)

    HIGGS_GENERATE_FUNCTION_WITH_DEFAULT (XDestroyWindow, xDestroyWindow,
                                         (::Display*, ::Window),
                                         void)

    HIGGS_GENERATE_FUNCTION_WITH_DEFAULT (XDisplayHeight, xDisplayHeight,
                                         (::Display*, int),
                                         int)

    HIGGS_GENERATE_FUNCTION_WITH_DEFAULT (XDisplayHeightMM, xDisplayHeightMM,
                                         (::Display*, int),
                                         int)

    HIGGS_GENERATE_FUNCTION_WITH_DEFAULT (XDisplayWidth, xDisplayWidth,
                                         (::Display*, int),
                                         int)

    HIGGS_GENERATE_FUNCTION_WITH_DEFAULT (XDisplayWidthMM, xDisplayWidthMM,
                                         (::Display*, int),
                                         int)

    HIGGS_GENERATE_FUNCTION_WITH_DEFAULT (XEventsQueued, xEventsQueued,
                                         (::Display*, int),
                                         int)

    HIGGS_GENERATE_FUNCTION_WITH_DEFAULT (XFindContext, xFindContext,
                                         (::Display*, XID, XContext, XPointer*),
                                         int)

    HIGGS_GENERATE_FUNCTION_WITH_DEFAULT (XFlush, xFlush,
                                         (::Display*),
                                         int)

    HIGGS_GENERATE_FUNCTION_WITH_DEFAULT (XFree, xFree,
                                         (void*),
                                         void)

    HIGGS_GENERATE_FUNCTION_WITH_DEFAULT (XFreeCursor, xFreeCursor,
                                         (::Display*, Cursor),
                                         void)

    HIGGS_GENERATE_FUNCTION_WITH_DEFAULT (XFreeColormap ,xFreeColormap,
                                         (::Display*, Colormap),
                                         void)

    HIGGS_GENERATE_FUNCTION_WITH_DEFAULT (XFreeEventData, xFreeEventData,
                                         (::Display*, XGenericEventCookie*),
                                         void)

    HIGGS_GENERATE_FUNCTION_WITH_DEFAULT (XFreeGC, xFreeGC,
                                         (::Display*, GC),
                                         void)

    HIGGS_GENERATE_FUNCTION_WITH_DEFAULT (XFreeModifiermap, xFreeModifiermap,
                                         (XModifierKeymap*),
                                         void)

    HIGGS_GENERATE_FUNCTION_WITH_DEFAULT (XFreePixmap, xFreePixmap,
                                         (::Display*, Pixmap),
                                         void)

    HIGGS_GENERATE_FUNCTION_WITH_DEFAULT (XGetAtomName, xGetAtomName,
                                         (::Display*, Atom),
                                         char*)

    HIGGS_GENERATE_FUNCTION_WITH_DEFAULT (XGetErrorDatabaseText, xGetErrorDatabaseText,
                                         (::Display*, const char*, const char*, const char*, const char*, int),
                                         void)

    HIGGS_GENERATE_FUNCTION_WITH_DEFAULT (XGetErrorText, xGetErrorText,
                                         (::Display*, int, const char*, int),
                                         void)

    HIGGS_GENERATE_FUNCTION_WITH_DEFAULT (XGetEventData, xGetEventData,
                                         (::Display*, XGenericEventCookie*),
                                         Bool)

    HIGGS_GENERATE_FUNCTION_WITH_DEFAULT (XGetGeometry, xGetGeometry,
                                         (::Display*, ::Drawable, ::Window*, int*, int*, unsigned int*, unsigned int*, unsigned int*, unsigned int*),
                                         Status)

    HIGGS_GENERATE_FUNCTION_WITH_DEFAULT (XGetImage, xGetImage,
                                         (::Display*, ::Drawable, int, int, unsigned int, unsigned int, unsigned long, int),
                                         XImage*)

    HIGGS_GENERATE_FUNCTION_WITH_DEFAULT (XGetInputFocus, xGetInputFocus,
                                         (::Display*, ::Window*, int*),
                                         void)

    HIGGS_GENERATE_FUNCTION_WITH_DEFAULT (XGetModifierMapping, xGetModifierMapping,
                                         (::Display*),
                                         XModifierKeymap*)

    HIGGS_GENERATE_FUNCTION_WITH_DEFAULT (XGetPointerMapping, xGetPointerMapping,
                                         (::Display*, unsigned char[], int),
                                         int)

    HIGGS_GENERATE_FUNCTION_WITH_DEFAULT (XGetSelectionOwner, xGetSelectionOwner,
                                         (::Display*, Atom),
                                         ::Window)

    HIGGS_GENERATE_FUNCTION_WITH_DEFAULT (XGetVisualInfo, xGetVisualInfo,
                                         (::Display*, long, XVisualInfo*, int*),
                                         XVisualInfo*)

    HIGGS_GENERATE_FUNCTION_WITH_DEFAULT (XGetWMHints, xGetWMHints,
                                         (::Display*, ::Window),
                                         XWMHints*)

    HIGGS_GENERATE_FUNCTION_WITH_DEFAULT (XGetWindowAttributes, xGetWindowAttributes,
                                         (::Display*, ::Window, XWindowAttributes*),
                                         Status)

    HIGGS_GENERATE_FUNCTION_WITH_DEFAULT (XGetWindowProperty, xGetWindowProperty,
                                         (::Display*, ::Window, Atom, long, long, Bool, Atom, Atom*, int*, unsigned long*, unsigned long*, unsigned char**),
                                         int)

    HIGGS_GENERATE_FUNCTION_WITH_DEFAULT (XGrabPointer, xGrabPointer,
                                         (::Display*, ::Window, Bool, unsigned int, int, int, ::Window, Cursor, ::Time),
                                         int)

    HIGGS_GENERATE_FUNCTION_WITH_DEFAULT (XGrabServer, xGrabServer,
                                         (::Display*),
                                         int)

    HIGGS_GENERATE_FUNCTION_WITH_DEFAULT (XImageByteOrder, xImageByteOrder,
                                         (::Display*),
                                         int)

    HIGGS_GENERATE_FUNCTION_WITH_DEFAULT (XInitImage, xInitImage,
                                         (XImage*),
                                         Status)

    HIGGS_GENERATE_FUNCTION_WITH_DEFAULT (XInitThreads, xInitThreads,
                                         (),
                                         Status)

    HIGGS_GENERATE_FUNCTION_WITH_DEFAULT (XInstallColormap, xInstallColormap,
                                         (::Display*, Colormap),
                                         void)

    HIGGS_GENERATE_FUNCTION_WITH_DEFAULT (XInternAtom, xInternAtom,
                                         (::Display*, const char*, Bool),
                                         Atom)

   #if HIGGS_USE_XINPUT
    HIGGS_GENERATE_FUNCTION_WITH_DEFAULT_RESULT (XIQueryVersion, xiQueryVersion,
                                                (::Display*, int*, int*),
                                                Status,
                                                BadRequest)

    HIGGS_GENERATE_FUNCTION_WITH_DEFAULT (XIQueryDevice, xiQueryDevice,
                                         (::Display*, int, int*),
                                         XIDeviceInfo*)

    HIGGS_GENERATE_FUNCTION_WITH_DEFAULT (XIFreeDeviceInfo, xiFreeDeviceInfo,
                                         (XIDeviceInfo*),
                                         void)

    HIGGS_GENERATE_FUNCTION_WITH_DEFAULT (XISelectEvents, xiSelectEvents,
                                         (::Display*, ::Window, XIEventMask*, int),
                                         Status)

    HIGGS_GENERATE_FUNCTION_WITH_DEFAULT (XIGetClientPointer, xiGetClientPointer,
                                         (::Display*, ::Window, int*),
                                         Bool)

    HIGGS_GENERATE_FUNCTION_WITH_DEFAULT_RESULT (XIGrabDevice, xiGrabDevice,
                                                (::Display*, int, ::Window, ::Time, Cursor, int, int, Bool, XIEventMask*),
                                                Status,
                                                BadRequest)

    HIGGS_GENERATE_FUNCTION_WITH_DEFAULT (XIUngrabDevice, xiUngrabDevice,
                                         (::Display*, int, ::Time),
                                         Status)
   #endif

    HIGGS_GENERATE_FUNCTION_WITH_DEFAULT (XkbKeycodeToKeysym, xkbKeycodeToKeysym,
                                         (::Display*, KeyCode, unsigned int, unsigned int),
                                         KeySym)

    HIGGS_GENERATE_FUNCTION_WITH_DEFAULT (XKeysymToKeycode, xKeysymToKeycode,
                                         (::Display*, KeySym),
                                         KeyCode)

    HIGGS_GENERATE_FUNCTION_WITH_DEFAULT (XListProperties, xListProperties,
                                         (::Display*, Window, int*),
                                         Atom*)

    HIGGS_GENERATE_FUNCTION_WITH_DEFAULT (XLockDisplay, xLockDisplay,
                                         (::Display*),
                                         void)

    HIGGS_GENERATE_FUNCTION_WITH_DEFAULT (XLookupString, xLookupString,
                                         (XKeyEvent*, const char*, int, KeySym*, XComposeStatus*),
                                         int)

    HIGGS_GENERATE_FUNCTION_WITH_DEFAULT (XMapRaised, xMapRaised,
                                         (::Display*, ::Window),
                                         void)

    HIGGS_GENERATE_FUNCTION_WITH_DEFAULT (XMapWindow, xMapWindow,
                                         (::Display*, ::Window),
                                         void)

    HIGGS_GENERATE_FUNCTION_WITH_DEFAULT (XMoveResizeWindow, xMoveResizeWindow,
                                         (::Display*, ::Window, int, int, unsigned int, unsigned int),
                                         void)

    HIGGS_GENERATE_FUNCTION_WITH_DEFAULT (XNextEvent, xNextEvent,
                                         (::Display*, XEvent*),
                                         int)

    HIGGS_GENERATE_FUNCTION_WITH_DEFAULT (XNextRequest, xNextRequest,
                                         (::Display*),
                                         unsigned long)

    HIGGS_GENERATE_FUNCTION_WITH_DEFAULT (XOpenDisplay, xOpenDisplay,
                                         (const char*),
                                         ::Display*)

    HIGGS_GENERATE_FUNCTION_WITH_DEFAULT (XPeekEvent, xPeekEvent,
                                         (::Display*, XEvent*),
                                         void)

    HIGGS_GENERATE_FUNCTION_WITH_DEFAULT (XPending, xPending,
                                         (::Display*),
                                         int)

    HIGGS_GENERATE_FUNCTION_WITH_DEFAULT (XPutImage, xPutImage,
                                         (::Display*, ::Drawable, GC, XImage*, int, int, int, int, unsigned int, unsigned int),
                                         void)

    HIGGS_GENERATE_FUNCTION_WITH_DEFAULT (XPutPixel, xPutPixel,
                                         (XImage*, int, int, unsigned long),
                                         void)

    HIGGS_GENERATE_FUNCTION_WITH_DEFAULT (XQueryBestCursor, xQueryBestCursor,
                                         (::Display*, ::Drawable, unsigned int, unsigned int, unsigned int*, unsigned int*),
                                         Status)

    HIGGS_GENERATE_FUNCTION_WITH_DEFAULT (XQueryExtension, xQueryExtension,
                                         (::Display*, const char*, int*, int*, int*),
                                         Bool)

    HIGGS_GENERATE_FUNCTION_WITH_DEFAULT (XQueryPointer, xQueryPointer,
                                         (::Display*, ::Window, ::Window*, ::Window*, int*, int*, int*, int*, unsigned int*),
                                         Bool)

    HIGGS_GENERATE_FUNCTION_WITH_DEFAULT (XQueryTree, xQueryTree,
                                         (::Display*, ::Window, ::Window*, ::Window*, ::Window**, unsigned int*),
                                         Status)

    HIGGS_GENERATE_FUNCTION_WITH_DEFAULT (XRefreshKeyboardMapping, xRefreshKeyboardMapping,
                                         (XMappingEvent*),
                                         void)

    HIGGS_GENERATE_FUNCTION_WITH_DEFAULT (XReparentWindow, xReparentWindow,
                                         (::Display*, ::Window, ::Window, int, int),
                                         void)

    HIGGS_GENERATE_FUNCTION_WITH_DEFAULT (XResizeWindow, xResizeWindow,
                                         (::Display*, Window, unsigned int, unsigned int),
                                         void)

    HIGGS_GENERATE_FUNCTION_WITH_DEFAULT (XRestackWindows, xRestackWindows,
                                         (::Display*, ::Window[], int),
                                         void)

    HIGGS_GENERATE_FUNCTION_WITH_DEFAULT (XRootWindow, xRootWindow,
                                         (::Display*, int),
                                         ::Window)

    HIGGS_GENERATE_FUNCTION_WITH_DEFAULT (XSaveContext, xSaveContext,
                                         (::Display*, XID, XContext, XPointer),
                                         int)

    HIGGS_GENERATE_FUNCTION_WITH_DEFAULT (XScreenCount, xScreenCount,
                                         (::Display*),
                                         int)

    HIGGS_GENERATE_FUNCTION_WITH_DEFAULT (XScreenNumberOfScreen, xScreenNumberOfScreen,
                                         (Screen*),
                                         int)

    HIGGS_GENERATE_FUNCTION_WITH_DEFAULT (XSelectInput, xSelectInput,
                                         (::Display*, ::Window, long),
                                         void)

    HIGGS_GENERATE_FUNCTION_WITH_DEFAULT (XSendEvent, xSendEvent,
                                         (::Display*, ::Window, Bool, long, XEvent*),
                                         Status)

    HIGGS_GENERATE_FUNCTION_WITH_DEFAULT (XSetClassHint, xSetClassHint,
                                         (::Display*, ::Window, XClassHint*),
                                         void)

    using XAfterFunction = int (*) (::Display*);

    HIGGS_GENERATE_FUNCTION_WITH_DEFAULT (XSetAfterFunction, xSetAfterFunction,
                                         (::Display*, XAfterFunction),
                                         XAfterFunction)

    HIGGS_GENERATE_FUNCTION_WITH_DEFAULT (XSetErrorHandler, xSetErrorHandler,
                                         (XErrorHandler),
                                         XErrorHandler)

    HIGGS_GENERATE_FUNCTION_WITH_DEFAULT (XSetIOErrorHandler, xSetIOErrorHandler,
                                         (XIOErrorHandler),
                                         XIOErrorHandler)

    HIGGS_GENERATE_FUNCTION_WITH_DEFAULT (XSetInputFocus, xSetInputFocus,
                                         (::Display*, ::Window, int, ::Time),
                                         void)

    HIGGS_GENERATE_FUNCTION_WITH_DEFAULT (XSetSelectionOwner, xSetSelectionOwner,
                                         (::Display*, Atom, ::Window, ::Time),
                                         void)

    HIGGS_GENERATE_FUNCTION_WITH_DEFAULT (XSetTransientForHint, xSetTransientForHint,
                                         (::Display*, ::Window, ::Window),
                                         int)

    HIGGS_GENERATE_FUNCTION_WITH_DEFAULT (XSetWMHints, xSetWMHints,
                                         (::Display*, ::Window, XWMHints*),
                                         void)

    HIGGS_GENERATE_FUNCTION_WITH_DEFAULT (XSetWMIconName, xSetWMIconName,
                                         (::Display*, ::Window, XTextProperty*),
                                         void)

    HIGGS_GENERATE_FUNCTION_WITH_DEFAULT (XSetWMName, xSetWMName,
                                         (::Display*, ::Window, XTextProperty*),
                                         void)

    HIGGS_GENERATE_FUNCTION_WITH_DEFAULT (XSetWMNormalHints, xSetWMNormalHints,
                                         (::Display*, ::Window, XSizeHints*),
                                         void)

    HIGGS_GENERATE_FUNCTION_WITH_DEFAULT (XStringListToTextProperty, xStringListToTextProperty,
                                         (char**, int, XTextProperty*),
                                         Status)

    HIGGS_GENERATE_FUNCTION_WITH_DEFAULT (Xutf8TextListToTextProperty, xutf8TextListToTextProperty,
                                         (::Display*, char**, int, XICCEncodingStyle, XTextProperty*),
                                         int)

    HIGGS_GENERATE_FUNCTION_WITH_DEFAULT (XSync, xSync,
                                         (::Display*, Bool),
                                         void)

    HIGGS_GENERATE_FUNCTION_WITH_DEFAULT (XSynchronize, xSynchronize,
                                         (::Display*, Bool),
                                         int)

    HIGGS_GENERATE_FUNCTION_WITH_DEFAULT (XTranslateCoordinates, xTranslateCoordinates,
                                         (::Display*, ::Window, ::Window, int, int, int*, int*, ::Window*),
                                         Bool)

    HIGGS_GENERATE_FUNCTION_WITH_DEFAULT (XrmUniqueQuark, xrmUniqueQuark,
                                         (),
                                         int)

    HIGGS_GENERATE_FUNCTION_WITH_DEFAULT (XUngrabPointer, xUngrabPointer,
                                         (::Display*, ::Time),
                                         void)

    HIGGS_GENERATE_FUNCTION_WITH_DEFAULT (XUngrabServer, xUngrabServer,
                                         (::Display*),
                                         int)

    HIGGS_GENERATE_FUNCTION_WITH_DEFAULT (XUnlockDisplay, xUnlockDisplay,
                                         (::Display*),
                                         void)

    HIGGS_GENERATE_FUNCTION_WITH_DEFAULT (XUnmapWindow, xUnmapWindow,
                                         (::Display*, ::Window),
                                         void)

    HIGGS_GENERATE_FUNCTION_WITH_DEFAULT (XWarpPointer, xWarpPointer,
                                         (::Display*, ::Window, ::Window, int, int, unsigned int, unsigned int, int, int),
                                         void)
   #if HIGGS_USE_XCURSOR
    HIGGS_GENERATE_FUNCTION_WITH_DEFAULT (XcursorImageCreate, xcursorImageCreate,
                                         (int, int),
                                         XcursorImage*)

    HIGGS_GENERATE_FUNCTION_WITH_DEFAULT (XcursorImageLoadCursor, xcursorImageLoadCursor,
                                         (::Display*, XcursorImage*),
                                         Cursor)

    HIGGS_GENERATE_FUNCTION_WITH_DEFAULT (XcursorImageDestroy, xcursorImageDestroy,
                                         (XcursorImage*),
                                         void)
   #endif
   #if HIGGS_USE_XINERAMA
    HIGGS_GENERATE_FUNCTION_WITH_DEFAULT (XineramaIsActive, xineramaIsActive,
                                         (::Display*),
                                         Bool)

    HIGGS_GENERATE_FUNCTION_WITH_DEFAULT (XineramaQueryScreens, xineramaQueryScreens,
                                         (::Display*, int*),
                                         XineramaScreenInfo*)
   #endif
   #if HIGGS_USE_XRENDER
    HIGGS_GENERATE_FUNCTION_WITH_DEFAULT (XRenderQueryVersion, xRenderQueryVersion,
                                         (::Display*, int*, int*),
                                         Status)

    HIGGS_GENERATE_FUNCTION_WITH_DEFAULT (XRenderFindStandardFormat, xRenderFindStandardFormat,
                                         (Display*, int),
                                         XRenderPictFormat*)

    HIGGS_GENERATE_FUNCTION_WITH_DEFAULT (XRenderFindFormat, xRenderFindFormat,
                                         (Display*, unsigned long, XRenderPictFormat*, int),
                                         XRenderPictFormat*)

    HIGGS_GENERATE_FUNCTION_WITH_DEFAULT (XRenderFindVisualFormat, xRenderFindVisualFormat,
                                         (Display*, Visual*),
                                         XRenderPictFormat*)
   #endif
   #if HIGGS_USE_XRANDR
    HIGGS_GENERATE_FUNCTION_WITH_DEFAULT (XRRGetScreenResources, xRRGetScreenResources,
                                         (::Display*, Window),
                                         XRRScreenResources*)

    HIGGS_GENERATE_FUNCTION_WITH_DEFAULT (XRRFreeScreenResources, xRRFreeScreenResources,
                                         (XRRScreenResources*),
                                         void)

    HIGGS_GENERATE_FUNCTION_WITH_DEFAULT (XRRGetOutputInfo, xRRGetOutputInfo,
                                         (::Display*, XRRScreenResources*, RROutput),
                                         XRROutputInfo*)

    HIGGS_GENERATE_FUNCTION_WITH_DEFAULT (XRRFreeOutputInfo, xRRFreeOutputInfo,
                                         (XRROutputInfo*),
                                         void)

    HIGGS_GENERATE_FUNCTION_WITH_DEFAULT (XRRGetCrtcInfo, xRRGetCrtcInfo,
                                         (::Display*, XRRScreenResources*, RRCrtc),
                                         XRRCrtcInfo*)

    HIGGS_GENERATE_FUNCTION_WITH_DEFAULT (XRRFreeCrtcInfo, xRRFreeCrtcInfo,
                                         (XRRCrtcInfo*),
                                         void)

    HIGGS_GENERATE_FUNCTION_WITH_DEFAULT (XRRGetOutputPrimary, xRRGetOutputPrimary,
                                         (::Display*, ::Window),
                                         RROutput)
   #endif
   #if HIGGS_USE_XSHM
    HIGGS_GENERATE_FUNCTION_WITH_DEFAULT (XShmAttach, xShmAttach,
                                         (::Display*, XShmSegmentInfo*),
                                         Bool)

    HIGGS_GENERATE_FUNCTION_WITH_DEFAULT (XShmCreateImage, xShmCreateImage,
                                         (::Display*, Visual*, unsigned int, int, const char*, XShmSegmentInfo*, unsigned int, unsigned int),
                                         XImage*)

    HIGGS_GENERATE_FUNCTION_WITH_DEFAULT (XShmDetach, xShmDetach,
                                         (::Display*, XShmSegmentInfo*),
                                         Bool)

    HIGGS_GENERATE_FUNCTION_WITH_DEFAULT (XShmGetEventBase, xShmGetEventBase,
                                         (::Display*),
                                         Status)

    HIGGS_GENERATE_FUNCTION_WITH_DEFAULT (XShmPutImage, xShmPutImage,
                                         (::Display*, ::Drawable, GC, XImage*, int, int, int, int, unsigned int, unsigned int, bool),
                                         Bool)

    HIGGS_GENERATE_FUNCTION_WITH_DEFAULT (XShmQueryVersion, xShmQueryVersion,
                                         (::Display*, int*, int*, Bool*),
                                         Bool)
   #endif

    //==============================================================================
    HIGGS_DECLARE_SINGLETON_INLINE (X11Symbols, false)

private:
    X11Symbols() = default;

    ~X11Symbols()
    {
        clearSingletonInstance();
    }

    //==============================================================================
    DynamicLibrary xLib { "libX11.so.6" }, xextLib { "libXext.so.6" };

   #if HIGGS_USE_XCURSOR
    DynamicLibrary xcursorLib  { "libXcursor.so.1" };
   #endif
   #if HIGGS_USE_XINERAMA
    DynamicLibrary xineramaLib { "libXinerama.so.1" };
   #endif
   #if HIGGS_USE_XRENDER
    DynamicLibrary xrenderLib  { "libXrender.so.1" };
   #endif
   #if HIGGS_USE_XRANDR
    DynamicLibrary xrandrLib   { "libXrandr.so.2" };
   #endif
   #if HIGGS_USE_XINPUT
    DynamicLibrary xinputLib   { "libXi.so.6" };
   #endif

    //==============================================================================
    HIGGS_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (X11Symbols)
};

} // namespace higgs
