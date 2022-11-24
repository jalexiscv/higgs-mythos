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

class HWNDComponent::Pimpl  : public ComponentMovementWatcher
{
public:
    Pimpl (HWND h, Component& comp)
        : ComponentMovementWatcher (&comp),
          hwnd (h),
          owner (comp)
    {
        if (owner.isShowing())
            componentPeerChanged();
    }

    ~Pimpl() override
    {
        removeFromParent();
        DestroyWindow (hwnd);
    }

    void componentMovedOrResized (bool wasMoved, bool wasResized) override
    {
        if (auto* peer = owner.getTopLevelComponent()->getPeer())
        {
            auto area = (peer->getAreaCoveredBy (owner).toFloat() * peer->getPlatformScaleFactor()).getSmallestIntegerContainer();

            UINT flagsToSend =  SWP_NOACTIVATE | SWP_NOZORDER | SWP_NOOWNERZORDER;

            if (! wasMoved)   flagsToSend |= SWP_NOMOVE;
            if (! wasResized) flagsToSend |= SWP_NOSIZE;

            ScopedThreadDPIAwarenessSetter threadDpiAwarenessSetter { hwnd };

            SetWindowPos (hwnd, nullptr, area.getX(), area.getY(), area.getWidth(), area.getHeight(), flagsToSend);

            invalidateHWNDAndChildren();
        }
    }

    using ComponentMovementWatcher::componentMovedOrResized;

    void componentPeerChanged() override
    {
        auto* peer = owner.getPeer();

        if (currentPeer != peer)
        {
            removeFromParent();
            currentPeer = peer;

            addToParent();
        }

        auto isShowing = owner.isShowing();

        ShowWindow (hwnd, isShowing ? SW_SHOWNA : SW_HIDE);

        if (isShowing)
            InvalidateRect (hwnd, nullptr, TRUE);
     }

    void componentVisibilityChanged() override
    {
        componentPeerChanged();
    }

    using ComponentMovementWatcher::componentVisibilityChanged;

    void componentBroughtToFront (Component& comp) override
    {
        ComponentMovementWatcher::componentBroughtToFront (comp);
    }

    Rectangle<int> getHWNDBounds() const
    {
        if (auto* peer = owner.getPeer())
        {
            ScopedThreadDPIAwarenessSetter threadDpiAwarenessSetter { hwnd };

            RECT r;
            GetWindowRect (hwnd, &r);
            Rectangle<int> windowRectangle (r.right - r.left, r.bottom - r.top);

            return (windowRectangle.toFloat() / peer->getPlatformScaleFactor()).toNearestInt();
        }

        return {};
    }

    void invalidateHWNDAndChildren()
    {
        EnumChildWindows (hwnd, invalidateHwndCallback, 0);
    }

    static BOOL WINAPI invalidateHwndCallback (HWND hwnd, LPARAM)
    {
        InvalidateRect (hwnd, nullptr, TRUE);
        return TRUE;
    }

    HWND hwnd;

private:
    void addToParent()
    {
        if (currentPeer != nullptr)
        {
            auto windowFlags = GetWindowLongPtr (hwnd, GWL_STYLE);

            using FlagType = decltype (windowFlags);

            windowFlags &= ~(FlagType) WS_POPUP;
            windowFlags |= (FlagType) WS_CHILD;

            SetWindowLongPtr (hwnd, GWL_STYLE, windowFlags);
            SetParent (hwnd, (HWND) currentPeer->getNativeHandle());

            componentMovedOrResized (true, true);
        }
    }

    void removeFromParent()
    {
        ShowWindow (hwnd, SW_HIDE);
        SetParent (hwnd, nullptr);
    }

    Component& owner;
    ComponentPeer* currentPeer = nullptr;

    HIGGS_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Pimpl)
};

//==============================================================================
HWNDComponent::HWNDComponent()  {}
HWNDComponent::~HWNDComponent() {}

void HWNDComponent::paint (Graphics&) {}

void HWNDComponent::setHWND (void* hwnd)
{
    if (hwnd != getHWND())
    {
        pimpl.reset();

        if (hwnd != nullptr)
            pimpl.reset (new Pimpl ((HWND) hwnd, *this));
    }
}

void* HWNDComponent::getHWND() const
{
    return pimpl == nullptr ? nullptr : (void*) pimpl->hwnd;
}

void HWNDComponent::resizeToFit()
{
    if (pimpl != nullptr)
        setBounds (pimpl->getHWNDBounds());
}

void HWNDComponent::updateHWNDBounds()
{
    if (pimpl != nullptr)
        pimpl->componentMovedOrResized (true, true);
}

} // namespace higgs
