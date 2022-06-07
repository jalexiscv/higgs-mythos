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
class UIAValueProvider  : public UIAProviderBase,
                          public ComBaseClassHelper<IValueProvider>
{
public:
    using UIAProviderBase::UIAProviderBase;

    //==============================================================================
    HIGGS_COMRESULT SetValue (LPCWSTR val) override
    {
        if (! isElementValid())
            return (HRESULT) UIA_E_ELEMENTNOTAVAILABLE;

        const auto& handler = getHandler();
        auto& valueInterface = *handler.getValueInterface();

        if (valueInterface.isReadOnly())
            return (HRESULT) UIA_E_NOTSUPPORTED;

        valueInterface.setValueAsString (String (val));

        VARIANT newValue;
        VariantHelpers::setString (valueInterface.getCurrentValueAsString(), &newValue);

        sendAccessibilityPropertyChangedEvent (handler, UIA_ValueValuePropertyId, newValue);

        return S_OK;
    }

    HIGGS_COMRESULT get_Value (BSTR* pRetVal) override
    {
        return withCheckedComArgs (pRetVal, *this, [&]
        {
            auto currentValueString = getHandler().getValueInterface()->getCurrentValueAsString();

            *pRetVal = SysAllocString ((const OLECHAR*) currentValueString.toWideCharPointer());
            return S_OK;
        });
    }

    HIGGS_COMRESULT get_IsReadOnly (BOOL* pRetVal) override
    {
        return withCheckedComArgs (pRetVal, *this, [&]
        {
            *pRetVal = getHandler().getValueInterface()->isReadOnly();
            return S_OK;
        });
    }

private:
    //==============================================================================
    HIGGS_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (UIAValueProvider)
};

} // namespace higgs
