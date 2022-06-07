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
class UIAGridItemProvider  : public UIAProviderBase,
                             public ComBaseClassHelper<IGridItemProvider, ITableItemProvider>
{
public:
    using UIAProviderBase::UIAProviderBase;

    //==============================================================================
    HIGGS_COMRESULT get_Row (int* pRetVal) override
    {
        return withTableSpan (pRetVal,
                              &AccessibilityTableInterface::getRowSpan,
                              &AccessibilityTableInterface::Span::begin);
    }

    HIGGS_COMRESULT get_Column (int* pRetVal) override
    {
        return withTableSpan (pRetVal,
                              &AccessibilityTableInterface::getColumnSpan,
                              &AccessibilityTableInterface::Span::begin);
    }

    HIGGS_COMRESULT get_RowSpan (int* pRetVal) override
    {
        return withTableSpan (pRetVal,
                              &AccessibilityTableInterface::getRowSpan,
                              &AccessibilityTableInterface::Span::num);
    }

    HIGGS_COMRESULT get_ColumnSpan (int* pRetVal) override
    {
        return withTableSpan (pRetVal,
                              &AccessibilityTableInterface::getColumnSpan,
                              &AccessibilityTableInterface::Span::num);
    }

    HIGGS_COMRESULT get_ContainingGrid (IRawElementProviderSimple** pRetVal) override
    {
        return withTableInterface (pRetVal, [&] (const AccessibilityHandler& tableHandler)
        {
            HIGGS_BEGIN_IGNORE_WARNINGS_GCC_LIKE ("-Wlanguage-extension-token")
            tableHandler.getNativeImplementation()->QueryInterface (IID_PPV_ARGS (pRetVal));
            HIGGS_END_IGNORE_WARNINGS_GCC_LIKE

            return true;
        });
    }

    HIGGS_COMRESULT GetRowHeaderItems (SAFEARRAY**) override
    {
        return (HRESULT) UIA_E_NOTSUPPORTED;
    }

    HIGGS_COMRESULT GetColumnHeaderItems (SAFEARRAY** pRetVal) override
    {
        return withTableInterface (pRetVal, [&] (const AccessibilityHandler& tableHandler)
        {
            if (auto* tableInterface = tableHandler.getTableInterface())
            {
                if (const auto column = tableInterface->getColumnSpan (getHandler()))
                {
                    if (auto* header = tableInterface->getHeaderHandler())
                    {
                        const auto children = header->getChildren();

                        if (isPositiveAndBelow (column->begin, children.size()))
                        {
                            ComSmartPtr<IRawElementProviderSimple> provider;

                            if (auto* child = children[(size_t) column->begin])
                            {
                                HIGGS_BEGIN_IGNORE_WARNINGS_GCC_LIKE ("-Wlanguage-extension-token")
                                if (child->getNativeImplementation()->QueryInterface (IID_PPV_ARGS (provider.resetAndGetPointerAddress())) == S_OK && provider != nullptr)
                                {
                                    SafeArrayHandle result { SafeArrayCreateVector (VT_UNKNOWN, 0, 1) };

                                    if (result == nullptr)
                                        return false;

                                    if (LONG index = 0; FAILED (SafeArrayPutElement (result.get(), &index, provider)))
                                        return false;

                                    *pRetVal = result.release();
                                    return true;
                                }
                                HIGGS_END_IGNORE_WARNINGS_GCC_LIKE
                            }
                        }
                    }
                }
            }

            return false;
        });
    }
private:
    template <typename Value, typename Callback>
    HIGGS_COMRESULT withTableInterface (Value* pRetVal, Callback&& callback) const
    {
        return withCheckedComArgs (pRetVal, *this, [&]() -> HRESULT
        {
            if (auto* handler = detail::AccessibilityHelpers::getEnclosingHandlerWithInterface (&getHandler(), &AccessibilityHandler::getTableInterface))
                if (handler->getTableInterface() != nullptr && callback (*handler))
                    return S_OK;

            return (HRESULT) UIA_E_NOTSUPPORTED;
        });
    }

    HIGGS_COMRESULT withTableSpan (int* pRetVal,
                                  Optional<AccessibilityTableInterface::Span> (AccessibilityTableInterface::* getSpan) (const AccessibilityHandler&) const,
                                  int AccessibilityTableInterface::Span::* spanMember) const
    {
        return withTableInterface (pRetVal, [&] (const AccessibilityHandler& handler)
        {
            if (const auto span = ((handler.getTableInterface())->*getSpan) (getHandler()))
            {
                *pRetVal = (*span).*spanMember;
                return true;
            }

            return false;
        });
    }

    //==============================================================================
    HIGGS_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (UIAGridItemProvider)
};

} // namespace higgs
