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
    A base class for top-level windows.

    This class is used for components that are considered a major part of your
    application - e.g. ResizableWindow, DocumentWindow, DialogWindow, AlertWindow,
    etc. Things like menus that pop up briefly aren't derived from it.

    A TopLevelWindow is probably on the desktop, but this isn't mandatory - it
    could itself be the child of another component.

    The class manages a list of all instances of top-level windows that are in use,
    and each one is also given the concept of being "active". The active window is
    one that is actively being used by the user. This isn't quite the same as the
    component with the keyboard focus, because there may be a popup menu or other
    temporary window which gets keyboard focus while the active top level window is
    unchanged.

    A top-level window also has an optional drop-shadow.

    @see ResizableWindow, DocumentWindow, DialogWindow

    @tags{GUI}
*/
class HIGGS_API  TopLevelWindow  : public Component
{
public:
    //==============================================================================
    /** Creates a TopLevelWindow.

        @param name                 the name to give the component
        @param addToDesktop         if true, the window will be automatically added to the
                                    desktop; if false, you can use it as a child component
    */
    TopLevelWindow (const String& name, bool addToDesktop);

    /** Destructor. */
    ~TopLevelWindow() override;

    //==============================================================================
    /** True if this is currently the TopLevelWindow that is actively being used.

        This isn't quite the same as having keyboard focus, because the focus may be
        on a child component or a temporary pop-up menu, etc, while this window is
        still considered to be active.

        @see activeWindowStatusChanged
    */
    bool isActiveWindow() const noexcept                    { return isCurrentlyActive; }

    //==============================================================================
    /** This will set the bounds of the window so that it's centred in front of another
        window.

        If your app has a few windows open and want to pop up a dialog box for one of
        them, you can use this to show it in front of the relevant parent window, which
        is a bit neater than just having it appear in the middle of the screen.

        If componentToCentreAround is nullptr, then the currently active TopLevelWindow will
        be used instead. If no window is focused, it'll just default to the middle of the
        screen.
    */
    void centreAroundComponent (Component* componentToCentreAround,
                                int width, int height);

    //==============================================================================
    /** Turns the drop-shadow on and off. */
    void setDropShadowEnabled (bool useShadow);

    /** True if drop-shadowing is enabled. */
    bool isDropShadowEnabled() const noexcept               { return useDropShadow; }

    /** Sets whether an OS-native title bar will be used, or a HIGGS one.
        @see isUsingNativeTitleBar
    */
    void setUsingNativeTitleBar (bool useNativeTitleBar);

    /** Returns true if the window is currently using an OS-native title bar.
        @see setUsingNativeTitleBar
    */
    bool isUsingNativeTitleBar() const noexcept;

    /** Sets the component that this window is opened from.

        This window is then kept above the window containing that component without being
        kept above other applications, and is minimised and restored with it. Where a window
        can position itself, it is also moved when that window is moved. Use this for a tool
        window that floats over a main window. The window is also left off the taskbar,
        since it is reached through the window it floats over.

        If the window is already on the desktop, changing this to a component in a different
        window recreates its native window. To avoid creating it twice, construct the window
        with addToDesktop set to false, call this, and then call addToDesktop().

        @see getAssociatedComponent, DesktopWindowOptions::withAssociatedComponent
    */
    void setAssociatedComponent (Component* component);

    /** Returns the component that this window is opened from, or nullptr if none was set.
        @see setAssociatedComponent
    */
    Component* getAssociatedComponent() const noexcept      { return associatedComponent.get(); }

    /** Sets whether the window can handle multi-touch events on Windows. The default value is false.

        On Windows, enabling multi-touch events disables the OS provided gesture recognition
        features, and you won't receive callbacks to Component::mouseMagnify().
    */
    void setUsingWindowsMultiTouch (bool useMultiTouch);

    /** Returns true if the window can handle multi-touch events on Windows.

        @see setUsingWindowsMultiTouch
    */
    bool isUsingWindowsMultiTouch() const noexcept;

    //==============================================================================
    /** Returns the number of TopLevelWindow objects currently in use.
        @see getTopLevelWindow
    */
    static int getNumTopLevelWindows() noexcept;

    /** Returns one of the TopLevelWindow objects currently in use.
        The index is 0 to (getNumTopLevelWindows() - 1).
    */
    static TopLevelWindow* getTopLevelWindow (int index) noexcept;

    /** Returns the currently-active top level window.
        There might not be one, of course, so this can return nullptr.
    */
    static TopLevelWindow* getActiveTopLevelWindow() noexcept;

    /** Adds the window to the desktop using the default flags. */
    void addToDesktop();

    using Component::addToDesktop;

    //==============================================================================
    /** @internal */
    std::unique_ptr<AccessibilityHandler> createAccessibilityHandler() override;

protected:
    //==============================================================================
    /** This callback happens when this window becomes active or inactive.
        @see isActiveWindow
    */
    virtual void activeWindowStatusChanged();


    //==============================================================================
    /** @internal */
    void focusOfChildComponentChanged (FocusChangeType) override;
    /** @internal */
    void parentHierarchyChanged() override;
    /** @internal */
    virtual int getDesktopWindowStyleFlags() const;
    /** @internal */
    void recreateDesktopWindow();
    /** @internal */
    void createDesktopWindow (const DesktopWindowOptions& options) override;
    /** @internal */
    void visibilityChanged() override;

private:
    friend class detail::TopLevelWindowManager;
    friend class ResizableWindow;
    bool useDropShadow = true, useNativeTitleBar = false, isCurrentlyActive = false;
    bool canUseWindowsMultiTouch = false;
    std::unique_ptr<DropShadower> shadower;
    WeakReference<Component> associatedComponent;

    void setWindowActive (bool);
    DesktopWindowOptions getDesktopWindowOptions() const;

    HIGGS_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (TopLevelWindow)
};

} // namespace higgs
