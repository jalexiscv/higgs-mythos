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
class UIAGridProvider  : public UIAProviderBase,
                         public ComBaseClassHelper<IGridProvider, ITableProvider>
{
public:
    using UIAProviderBase::UIAProviderBase;

    //==============================================================================
    HIGGS_COMRESULT GetItem (int row, int column, IRawElementProviderSimple** pRetVal) override
    {
        return withTableInterface (pRetVal, [&] (const AccessibilityTableInterface& tableInterface)
        {
            if (! isPositiveAndBelow (row, tableInterface.getNumRows())
                || ! isPositiveAndBelow (column, tableInterface.getNumColumns()))
                return E_INVALIDARG;

            HIGGS_BEGIN_IGNORE_WARNINGS_GCC_LIKE ("-Wlanguage-extension-token")

            if (auto* cellHandler = tableInterface.getCellHandler (row, column))
            {
                cellHandler->getNativeImplementation()->QueryInterface (IID_PPV_ARGS (pRetVal));
                return S_OK;
            }

            if (auto* rowHandler = tableInterface.getRowHandler (row))
            {
                rowHandler->getNativeImplementation()->QueryInterface (IID_PPV_ARGS (pRetVal));
                return S_OK;
            }

            HIGGS_END_IGNORE_WARNINGS_GCC_LIKE

            return E_FAIL;
        });
    }

    HIGGS_COMRESULT get_RowCount (int* pRetVal) override
    {
        return withTableInterface (pRetVal, [&] (const AccessibilityTableInterface& tableInterface)
        {
            *pRetVal = tableInterface.getNumRows();
            return S_OK;
        });
    }

    HIGGS_COMRESULT get_ColumnCount (int* pRetVal) override
    {
        return withTableInterface (pRetVal, [&] (const AccessibilityTableInterface& tableInterface)
        {
            *pRetVal = tableInterface.getNumColumns();
            return S_OK;
        });
    }

    HIGGS_COMRESULT GetRowHeaders (SAFEARRAY**) override
    {
        return (HRESULT) UIA_E_NOTSUPPORTED;
    }

    HIGGS_COMRESULT GetColumnHeaders (SAFEARRAY** pRetVal) override
    {
        return withTableInterface (pRetVal, [&] (const AccessibilityTableInterface& tableInterface)
        {
            auto* header = tableInterface.getHeaderHandler();

            if (header == nullptr)
                return (HRESULT) UIA_E_NOTSUPPORTED;

            const auto children = header->getChildren();

            SafeArrayHandle result { SafeArrayCreateVector (VT_UNKNOWN, 0, (ULONG) children.size()) };

            if (result == nullptr)
                return E_FAIL;

            LONG index = 0;

            for (const auto& child : children)
            {
                ComSmartPtr<IRawElementProviderSimple> provider;

                HIGGS_BEGIN_IGNORE_WARNINGS_GCC_LIKE ("-Wlanguage-extension-token")
                if (child != nullptr)
                    child->getNativeImplementation()->QueryInterface (IID_PPV_ARGS (provider.resetAndGetPointerAddress()));
                HIGGS_END_IGNORE_WARNINGS_GCC_LIKE

                if (provider == nullptr)
                    return E_FAIL;

                if (FAILED (SafeArrayPutElement (result.get(), &index, provider)))
                    return E_FAIL;

                ++index;
            }

            *pRetVal = result.release();
            return S_OK;
        });
    }

    HIGGS_COMRESULT get_RowOrColumnMajor (RowOrColumnMajor* pRetVal) override
    {
        *pRetVal = RowOrColumnMajor_RowMajor;
        return S_OK;
    }

private:
    template <typename Value, typename Callback>
    HIGGS_COMRESULT withTableInterface (Value* pRetVal, Callback&& callback) const
    {
        return withCheckedComArgs (pRetVal, *this, [&]() -> HRESULT
        {
            if (auto* tableHandler = detail::AccessibilityHelpers::getEnclosingHandlerWithInterface (&getHandler(), &AccessibilityHandler::getTableInterface))
                if (auto* tableInterface = tableHandler->getTableInterface())
                    return callback (*tableInterface);

            return (HRESULT) UIA_E_NOTSUPPORTED;
        });
    }

    //==============================================================================
    HIGGS_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (UIAGridProvider)
};

} // namespace higgs
