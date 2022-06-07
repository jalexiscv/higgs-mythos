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
class UIAInvokeProvider  : public UIAProviderBase,
                           public ComBaseClassHelper<IInvokeProvider>
{
public:
    using UIAProviderBase::UIAProviderBase;

    //==============================================================================
    HIGGS_COMRESULT Invoke() override
    {
        if (! isElementValid())
            return (HRESULT) UIA_E_ELEMENTNOTAVAILABLE;

        const auto& handler = getHandler();

        if (handler.getActions().invoke (AccessibilityActionType::press))
        {
            if (isElementValid())
                sendAccessibilityAutomationEvent (handler, UIA_Invoke_InvokedEventId);

            return S_OK;
        }

        return (HRESULT) UIA_E_NOTSUPPORTED;
    }

    //==============================================================================
    HIGGS_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (UIAInvokeProvider)
};

} // namespace higgs
