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
class UIATransformProvider  : public UIAProviderBase,
                              public ComBaseClassHelper<ITransformProvider>
{
public:
    using UIAProviderBase::UIAProviderBase;

    //==============================================================================
    HIGGS_COMRESULT Move (double x, double y) override
    {
        if (! isElementValid())
            return (HRESULT) UIA_E_ELEMENTNOTAVAILABLE;

        if (auto* peer = getPeer())
        {
            RECT rect;
            GetWindowRect ((HWND) peer->getNativeHandle(), &rect);

            rect.left = roundToInt (x);
            rect.top  = roundToInt (y);

            auto bounds = Rectangle<int>::leftTopRightBottom (rect.left, rect.top, rect.right, rect.bottom);

            peer->setBounds (Desktop::getInstance().getDisplays().physicalToLogical (bounds),
                             peer->isFullScreen());
        }

        return S_OK;
    }

    HIGGS_COMRESULT Resize (double width, double height) override
    {
        if (! isElementValid())
            return (HRESULT) UIA_E_ELEMENTNOTAVAILABLE;

        if (auto* peer = getPeer())
        {
            auto scale = peer->getPlatformScaleFactor();

            peer->getComponent().setSize (roundToInt (width  / scale),
                                          roundToInt (height / scale));
        }

        return S_OK;
    }

    HIGGS_COMRESULT Rotate (double) override
    {
        if (! isElementValid())
            return (HRESULT) UIA_E_ELEMENTNOTAVAILABLE;

        return (HRESULT) UIA_E_NOTSUPPORTED;
    }

    HIGGS_COMRESULT get_CanMove (BOOL* pRetVal) override
    {
        return withCheckedComArgs (pRetVal, *this, [&]
        {
            *pRetVal = true;
            return S_OK;
        });
    }

    HIGGS_COMRESULT get_CanResize (BOOL* pRetVal) override
    {
        return withCheckedComArgs (pRetVal, *this, [&]
        {
            if (auto* peer = getPeer())
                *pRetVal = ((peer->getStyleFlags() & ComponentPeer::windowIsResizable) != 0);

            return S_OK;
        });
    }

    HIGGS_COMRESULT get_CanRotate (BOOL* pRetVal) override
    {
        return withCheckedComArgs (pRetVal, *this, [&]
        {
            *pRetVal = false;
            return S_OK;
        });
    }

private:
    ComponentPeer* getPeer() const
    {
        return getHandler().getComponent().getPeer();
    }

    //==============================================================================
    HIGGS_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (UIATransformProvider)
};

} // namespace higgs
