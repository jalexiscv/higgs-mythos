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
class UIAExpandCollapseProvider  : public UIAProviderBase,
                                   public ComBaseClassHelper<IExpandCollapseProvider>
{
public:
    using UIAProviderBase::UIAProviderBase;

    //==============================================================================
    HIGGS_COMRESULT Expand() override
    {
        return invokeShowMenu();
    }

    HIGGS_COMRESULT Collapse() override
    {
        return invokeShowMenu();
    }

    HIGGS_COMRESULT get_ExpandCollapseState (ExpandCollapseState* pRetVal) override
    {
        return withCheckedComArgs (pRetVal, *this, [&]
        {
            *pRetVal = getHandler().getCurrentState().isExpanded()
                           ? ExpandCollapseState_Expanded
                           : ExpandCollapseState_Collapsed;

            return S_OK;
        });
    }

private:
    HIGGS_COMRESULT invokeShowMenu()
    {
        if (! isElementValid())
            return (HRESULT) UIA_E_ELEMENTNOTAVAILABLE;

        const auto& handler = getHandler();

        if (handler.getActions().invoke (AccessibilityActionType::showMenu))
        {
            sendAccessibilityAutomationEvent (handler, handler.getCurrentState().isExpanded()
                                                           ? UIA_MenuOpenedEventId
                                                           : UIA_MenuClosedEventId);

            return S_OK;
        }

        return (HRESULT) UIA_E_NOTSUPPORTED;
    }

    //==============================================================================
    HIGGS_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (UIAExpandCollapseProvider)
};

} // namespace higgs
