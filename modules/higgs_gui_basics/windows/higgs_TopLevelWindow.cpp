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
TopLevelWindow::TopLevelWindow (const String& name, const bool shouldAddToDesktop)
    : Component (name)
{
    setTitle (name);

    setOpaque (true);

    if (shouldAddToDesktop)
        Component::createDesktopWindow (DesktopWindowOptions().withStyleFlags (TopLevelWindow::getDesktopWindowStyleFlags()));
    else
        setDropShadowEnabled (true);

    setWantsKeyboardFocus (true);
    setBroughtToFrontOnMouseClick (true);
    isCurrentlyActive = detail::TopLevelWindowManager::getInstance()->addWindow (this);
}

TopLevelWindow::~TopLevelWindow()
{
    shadower = nullptr;
    detail::TopLevelWindowManager::getInstance()->removeWindow (this);
}

//==============================================================================
void TopLevelWindow::focusOfChildComponentChanged (FocusChangeType)
{
    auto* wm = detail::TopLevelWindowManager::getInstance();

    if (hasKeyboardFocus (true))
        wm->checkFocus();
    else
        wm->checkFocusAsync();
}

void TopLevelWindow::setWindowActive (const bool isNowActive)
{
    if (isCurrentlyActive != isNowActive)
    {
        isCurrentlyActive = isNowActive;
        activeWindowStatusChanged();
    }
}

void TopLevelWindow::activeWindowStatusChanged()
{
}

bool TopLevelWindow::isUsingNativeTitleBar() const noexcept
{
    return useNativeTitleBar && (isOnDesktop() || ! isShowing());
}

bool TopLevelWindow::isUsingWindowsMultiTouch() const noexcept
{
    return canUseWindowsMultiTouch && (isOnDesktop() || ! isShowing());
}

void TopLevelWindow::visibilityChanged()
{
    if (isShowing())
        if (auto* p = getPeer())
            if ((p->getStyleFlags() & (ComponentPeer::windowIsTemporary
                                        | ComponentPeer::windowIgnoresKeyPresses)) == 0)
                toFront (true);
}

void TopLevelWindow::parentHierarchyChanged()
{
    setDropShadowEnabled (useDropShadow);
}

int TopLevelWindow::getDesktopWindowStyleFlags() const
{
    // A window that floats over another window is reached through that window, not the taskbar.
    int styleFlags = associatedComponent != nullptr ? 0 : ComponentPeer::windowAppearsOnTaskbar;

    if (useDropShadow)       styleFlags |= ComponentPeer::windowHasDropShadow;
    if (useNativeTitleBar)   styleFlags |= ComponentPeer::windowHasTitleBar;

    return styleFlags;
}

void TopLevelWindow::setDropShadowEnabled (const bool useShadow)
{
    useDropShadow = useShadow;

    if (isOnDesktop())
    {
        shadower = nullptr;
        Component::createDesktopWindow (getDesktopWindowOptions());
    }
    else
    {
        if (useShadow && isOpaque())
        {
            if (shadower == nullptr)
            {
                shadower = getLookAndFeel().createDropShadowerForComponent (*this);

                if (shadower != nullptr)
                    shadower->setOwner (this);
            }
        }
        else
        {
            shadower = nullptr;
        }
    }
}

void TopLevelWindow::setUsingNativeTitleBar (const bool shouldUseNativeTitleBar)
{
    if (useNativeTitleBar != shouldUseNativeTitleBar)
    {
        detail::FocusRestorer focusRestorer;
        useNativeTitleBar = shouldUseNativeTitleBar;
        recreateDesktopWindow();
        sendLookAndFeelChange();
    }
}

void TopLevelWindow::setUsingWindowsMultiTouch (bool shouldUseMultiTouch)
{
    canUseWindowsMultiTouch = shouldUseMultiTouch;

    if (auto* peer = getPeer())
        peer->setWindowsCanUseMultiTouch (canUseWindowsMultiTouch);
}

void TopLevelWindow::setAssociatedComponent (Component* component)
{
    if (associatedComponent == component)
        return;

    associatedComponent = component;
    recreateDesktopWindow();
}

DesktopWindowOptions TopLevelWindow::getDesktopWindowOptions() const
{
    return DesktopWindowOptions().withStyleFlags (getDesktopWindowStyleFlags())
                                 .withAssociatedComponent (associatedComponent.get());
}

void TopLevelWindow::recreateDesktopWindow()
{
    if (isOnDesktop())
    {
        Component::createDesktopWindow (getDesktopWindowOptions());

        if (auto* peer = getPeer())
            peer->setWindowsCanUseMultiTouch (canUseWindowsMultiTouch);

        toFront (true);
    }
}

void TopLevelWindow::addToDesktop()
{
    shadower = nullptr;
    Component::createDesktopWindow (getDesktopWindowOptions());

    if (auto* peer = getPeer())
        peer->setWindowsCanUseMultiTouch (canUseWindowsMultiTouch);

    setDropShadowEnabled (isDropShadowEnabled()); // force an update to clear away any fake shadows if necessary
}

void TopLevelWindow::createDesktopWindow (const DesktopWindowOptions& options)
{
    /* It's not recommended to change the desktop window flags directly for a TopLevelWindow,
       because this class needs to make sure its layout corresponds with settings like whether
       it's got a native title bar or not.

       If you need custom flags for your window, you can override the getDesktopWindowStyleFlags()
       method. If you do this, it's best to call the base class's getDesktopWindowStyleFlags()
       method, then add or remove whatever flags are necessary from this value before returning it.
    */
    const auto windowStyleFlags = options.getStyleFlags();

    jassert ((windowStyleFlags & ~ComponentPeer::windowIsSemiTransparent)
               == (getDesktopWindowStyleFlags() & ~ComponentPeer::windowIsSemiTransparent));

    // Subclasses recreate the window with only its style flags, so the association given here is
    // kept, and one given earlier is carried over. Clearing it is done with setAssociatedComponent().
    if (auto* newAssociatedComponent = options.getAssociatedComponent())
        associatedComponent = newAssociatedComponent;

    Component::createDesktopWindow (options.withAssociatedComponent (associatedComponent.get()));

    if (windowStyleFlags != getDesktopWindowStyleFlags())
        sendLookAndFeelChange();
}

std::unique_ptr<AccessibilityHandler> TopLevelWindow::createAccessibilityHandler()
{
    return std::make_unique<AccessibilityHandler> (*this, AccessibilityRole::window);
}

//==============================================================================
void TopLevelWindow::centreAroundComponent (Component* c, const int width, const int height)
{
    if (c == nullptr)
        c = TopLevelWindow::getActiveTopLevelWindow();

    if (c == nullptr || c->getBounds().isEmpty())
    {
        centreWithSize (width, height);
    }
    else
    {
        const auto scale = getDesktopScaleFactor() / Desktop::getInstance().getGlobalScaleFactor();

        const auto [targetCentre, parentArea] = [&]
        {
            const auto globalTargetCentre = c->localPointToGlobal (c->getLocalBounds().getCentre()) / scale;

            if (auto* parent = getParentComponent())
                return std::make_pair (parent->getLocalPoint (nullptr, globalTargetCentre), parent->getLocalBounds());

            return std::make_pair (globalTargetCentre, c->getParentMonitorArea() / scale);
        }();

        setBounds (Rectangle { width, height }
                     .withCentre (targetCentre)
                     .constrainedWithin (parentArea.reduced (12, 12)));
    }
}

//==============================================================================
int TopLevelWindow::getNumTopLevelWindows() noexcept
{
    return detail::TopLevelWindowManager::getInstance()->windows.size();
}

TopLevelWindow* TopLevelWindow::getTopLevelWindow (const int index) noexcept
{
    return detail::TopLevelWindowManager::getInstance()->windows [index];
}

TopLevelWindow* TopLevelWindow::getActiveTopLevelWindow() noexcept
{
    TopLevelWindow* best = nullptr;
    int bestNumTWLParents = -1;

    for (int i = TopLevelWindow::getNumTopLevelWindows(); --i >= 0;)
    {
        auto* tlw = TopLevelWindow::getTopLevelWindow (i);

        if (tlw->isActiveWindow())
        {
            int numTWLParents = 0;

            for (auto* c = tlw->getParentComponent(); c != nullptr; c = c->getParentComponent())
                if (dynamic_cast<const TopLevelWindow*> (c) != nullptr)
                    ++numTWLParents;

            if (bestNumTWLParents < numTWLParents)
            {
                best = tlw;
                bestNumTWLParents = numTWLParents;
            }
        }
    }

    return best;
}

} // namespace higgs
