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
    Describes the desktop window that Component::addToDesktop() should create for a component.

    @see Component::addToDesktop

    @tags{GUI}
*/
class HIGGS_API  DesktopWindowOptions  final
{
public:
    /** Sets the window's style flags, a combination of the values in ComponentPeer::StyleFlags. */
    [[nodiscard]] DesktopWindowOptions withStyleFlags (int newFlags) const                  { return withMember (*this, &DesktopWindowOptions::styleFlags, newFlags); }

    /** Sets a UIView in which the component should place itself, instead of creating a
        window of its own. This is used on iOS and has no effect on other platforms.
    */
    [[nodiscard]] DesktopWindowOptions withParentUIView (void* view) const                  { return withParent<UIViewParent> (view); }

    /** Sets an Android Activity whose content view the component should become, instead of
        creating a window of its own. This is used on Android and has no effect on other
        platforms.
    */
    [[nodiscard]] DesktopWindowOptions withParentAndroidActivity (void* activity) const     { return withParent<AndroidActivityParent> (activity); }

    /** Sets an Android ViewGroup in which the component should place itself, instead of
        creating a window of its own. This is used on Android and has no effect on other
        platforms.
    */
    [[nodiscard]] DesktopWindowOptions withParentAndroidViewGroup (void* viewGroup) const   { return withParent<AndroidViewGroupParent> (viewGroup); }

    /** Sets an NSView in which the component should place itself, instead of creating a
        window of its own. This is used on macOS and has no effect on other platforms.
    */
    [[nodiscard]] DesktopWindowOptions withParentNSView (void* view) const                  { return withParent<NSViewParent> (view); }

    /** Sets a HWND in which the component should place itself as a child window, instead
        of creating a top-level window. This is used on Windows and has no effect on other
        platforms.
    */
    [[nodiscard]] DesktopWindowOptions withParentHWND (void* hwnd) const                    { return withParent<HWNDParent> (hwnd); }

    /** Sets an X11 window in which the component should place itself as a child window,
        instead of creating a top-level window. This is used on Linux and BSD and has no
        effect on other platforms.
    */
    [[nodiscard]] DesktopWindowOptions withParentX11Window (uint64_t window) const          { return withParent<X11WindowParent> (window); }

    /** Sets the component that the window is opened from.

        The new window is kept above the window containing that component without being
        kept above other applications, and is minimised and restored with it. Where a window
        can position itself, it is also moved when that window is moved. A temporary window
        such as a menu, a tooltip or a drag image should name the component it was opened
        for, and a tool window that floats over a main window should name a component in
        that window, normally omitting ComponentPeer::windowAppearsOnTaskbar from the window
        style flags as well.

        The component's window is looked up when the desktop window is created, so the
        component should be on the desktop, or inside a component that is, by then. It has
        no effect on iOS or Android. On macOS a window takes the level of the window it is
        associated with, so Component::setAlwaysOnTop() has no effect on it.

        A window cannot be associated with a component in itself, or in a window that was
        opened from it, directly or indirectly. Such an association is ignored.

        @see ComponentPeer::getAssociatedComponent, TopLevelWindow::setAssociatedComponent
    */
    [[nodiscard]] DesktopWindowOptions withAssociatedComponent (Component* component) const { return withMember (*this, &DesktopWindowOptions::associatedComponent, component); }

    /** Returns the window's style flags. */
    int getStyleFlags() const noexcept                  { return styleFlags; }

    /** Returns the UIView to attach to. */
    void* getParentUIView() const noexcept              { return getParent<UIViewParent>(); }

    /** Returns the Android Activity to attach to. */
    void* getParentAndroidActivity() const noexcept     { return getParent<AndroidActivityParent>(); }

    /** Returns the Android ViewGroup to attach to. */
    void* getParentAndroidViewGroup() const noexcept    { return getParent<AndroidViewGroupParent>(); }

    /** Returns the NSView to attach to. */
    void* getParentNSView() const noexcept              { return getParent<NSViewParent>(); }

    /** Returns the HWND to attach to. */
    void* getParentHWND() const noexcept                { return getParent<HWNDParent>(); }

    /** Returns the X11 window to attach to. */
    uint64_t getParentX11Window() const noexcept        { return getParent<X11WindowParent>(); }

    /** Returns the component that the window is opened from, or nullptr if none was set. */
    Component* getAssociatedComponent() const noexcept  { return associatedComponent; }

private:
    struct UIViewParent             { void* handle = nullptr; };
    struct AndroidActivityParent    { void* handle = nullptr; };
    struct AndroidViewGroupParent   { void* handle = nullptr; };
    struct NSViewParent             { void* handle = nullptr; };
    struct HWNDParent               { void* handle = nullptr; };
    struct X11WindowParent          { uint64_t handle = 0; };

    using NativeParent = std::variant<std::monostate,
                                      UIViewParent,
                                      AndroidActivityParent,
                                      AndroidViewGroupParent,
                                      NSViewParent,
                                      HWNDParent,
                                      X11WindowParent>;

    template <typename Parent, typename Handle>
    DesktopWindowOptions withParent (Handle handle) const
    {
        return withMember (*this, &DesktopWindowOptions::nativeParent, NativeParent { Parent { handle } });
    }

    template <typename Parent>
    decltype (Parent::handle) getParent() const noexcept
    {
        const auto* parent = std::get_if<Parent> (&nativeParent);
        return parent != nullptr ? parent->handle : decltype (Parent::handle){};
    }

    int styleFlags = 0;
    NativeParent nativeParent;
    Component* associatedComponent = nullptr;
};

} // namespace higgs
