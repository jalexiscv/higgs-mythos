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

class AccessibilityNativeHandle  : public ComBaseClassHelper<IRawElementProviderSimple,
                                                             IRawElementProviderFragment,
                                                             IRawElementProviderFragmentRoot,
                                                             IRawElementProviderHwndOverride>
{
public:
    explicit AccessibilityNativeHandle (AccessibilityHandler& handler);

    //==============================================================================
    void invalidateElement() noexcept         { valid = false; }
    bool isElementValid() const noexcept      { return valid; }

    const AccessibilityHandler& getHandler()  { return accessibilityHandler; }

    //==============================================================================
    HIGGS_COMRESULT QueryInterface (REFIID refId, void** result) override;

    //==============================================================================
    HIGGS_COMRESULT get_HostRawElementProvider (IRawElementProviderSimple** provider) override;
    HIGGS_COMRESULT get_ProviderOptions (ProviderOptions* options) override;
    HIGGS_COMRESULT GetPatternProvider (PATTERNID pId, IUnknown** provider) override;
    HIGGS_COMRESULT GetPropertyValue (PROPERTYID propertyId, VARIANT* pRetVal) override;

    HIGGS_COMRESULT Navigate (NavigateDirection direction, IRawElementProviderFragment** pRetVal) override;
    HIGGS_COMRESULT GetRuntimeId (SAFEARRAY** pRetVal) override;
    HIGGS_COMRESULT get_BoundingRectangle (UiaRect* pRetVal) override;
    HIGGS_COMRESULT GetEmbeddedFragmentRoots (SAFEARRAY** pRetVal) override;
    HIGGS_COMRESULT SetFocus() override;
    HIGGS_COMRESULT get_FragmentRoot (IRawElementProviderFragmentRoot** pRetVal) override;

    HIGGS_COMRESULT ElementProviderFromPoint (double x, double y, IRawElementProviderFragment** pRetVal) override;
    HIGGS_COMRESULT GetFocus (IRawElementProviderFragment** pRetVal) override;

    HIGGS_COMRESULT GetOverrideProviderForHwnd (HWND hwnd, IRawElementProviderSimple** pRetVal) override;

private:
    //==============================================================================
    String getElementName() const;
    bool isFragmentRoot() const     { return accessibilityHandler.getComponent().isOnDesktop(); }

    //==============================================================================
    AccessibilityHandler& accessibilityHandler;

    static int idCounter;
    std::array<int, 2> rtid { UiaAppendRuntimeId, ++idCounter };
    bool valid = true;

    //==============================================================================
    HIGGS_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AccessibilityNativeHandle)
};

}
