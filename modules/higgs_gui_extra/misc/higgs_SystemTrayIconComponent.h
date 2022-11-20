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

#if HIGGS_WINDOWS || HIGGS_LINUX || HIGGS_BSD || HIGGS_MAC || DOXYGEN

//==============================================================================
/**
    This component sits in the taskbar tray as a small icon.

    (NB: The exact behaviour of this class will differ between OSes, and it
    isn't fully implemented for all OSes)

    To use it, just create one of these components, but don't attempt to make it
    visible, add it to a parent, or put it on the desktop.

    You can then call setIconImage() to create an icon for it in the taskbar.

    To change the icon's tooltip, you can use setIconTooltip().

    To respond to mouse-events, you can override the normal mouseDown(),
    mouseUp(), mouseDoubleClick() and mouseMove() methods, and although the x, y
    position will not be valid, you can use this to respond to clicks. Traditionally
    you'd use a left-click to show your application's window, and a right-click
    to show a pop-up menu.

    @tags{GUI}
*/
class HIGGS_API  SystemTrayIconComponent  : public Component
{
public:
    //==============================================================================
    /** Constructor. */
    SystemTrayIconComponent();

    /** Destructor. */
    ~SystemTrayIconComponent() override;

    //==============================================================================
    /** Changes the image shown in the taskbar.

        On Windows and Linux a full colour Image is used as an icon.
        On macOS a template image is used, where all non-transparent regions will be
        rendered in a monochrome colour selected dynamically by the operating system.

        @param colourImage     An colour image to use as an icon on Windows and Linux
        @param templateImage   A template image to use as an icon on macOS
    */
    void setIconImage (const Image& colourImage, const Image& templateImage);

    /** Changes the icon's tooltip (if the current OS supports this). */
    void setIconTooltip (const String& tooltip);

    /** Highlights the icon (if the current OS supports this). */
    void setHighlighted (bool);

    /** Shows a floating text bubble pointing to the icon (if the current OS supports this). */
    void showInfoBubble (const String& title, const String& content);

    /** Hides the icon's floating text bubble (if the current OS supports this). */
    void hideInfoBubble();

    /** Returns the raw handle to whatever kind of internal OS structure is
        involved in showing this icon.
        @see ComponentPeer::getNativeHandle()
    */
    void* getNativeHandle() const;

   #if HIGGS_LINUX || HIGGS_BSD
    /** @internal */
    void paint (Graphics&) override;
   #endif

   #if HIGGS_MAC
    /** Shows a menu attached to the OSX menu bar icon. */
    void showDropdownMenu (const PopupMenu& menu);
   #endif

private:
    //==============================================================================
    HIGGS_PUBLIC_IN_DLL_BUILD (class Pimpl)
    std::unique_ptr<Pimpl> pimpl;

    [[deprecated ("The new setIconImage function signature requires different images for macOS and the other platforms.")]]
    void setIconImage (const Image& newImage);

    HIGGS_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SystemTrayIconComponent)
};

#endif

} // namespace higgs
