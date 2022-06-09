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

class WindowsUIAWrapper  : public DeletedAtShutdown
{
public:
    bool isLoaded() const noexcept
    {
        return uiaReturnRawElementProvider            != nullptr
            && uiaHostProviderFromHwnd                != nullptr
            && uiaRaiseAutomationPropertyChangedEvent != nullptr
            && uiaRaiseAutomationEvent                != nullptr
            && uiaClientsAreListening                 != nullptr
            && uiaDisconnectProvider                  != nullptr
            && uiaDisconnectAllProviders              != nullptr;
    }

    //==============================================================================
    LRESULT returnRawElementProvider (HWND hwnd, WPARAM wParam, LPARAM lParam, IRawElementProviderSimple* provider)
    {
        return uiaReturnRawElementProvider != nullptr ? uiaReturnRawElementProvider (hwnd, wParam, lParam, provider)
                                                      : (LRESULT) nullptr;
    }

    HIGGS_COMRESULT hostProviderFromHwnd (HWND hwnd, IRawElementProviderSimple** provider)
    {
        return uiaHostProviderFromHwnd != nullptr ? uiaHostProviderFromHwnd (hwnd, provider)
                                                  : (HRESULT) UIA_E_NOTSUPPORTED;
    }

    HIGGS_COMRESULT raiseAutomationPropertyChangedEvent (IRawElementProviderSimple* provider, PROPERTYID propID, VARIANT oldValue, VARIANT newValue)
    {
        return uiaRaiseAutomationPropertyChangedEvent != nullptr ? uiaRaiseAutomationPropertyChangedEvent (provider, propID, oldValue, newValue)
                                                                 : (HRESULT) UIA_E_NOTSUPPORTED;
    }

    HIGGS_COMRESULT raiseAutomationEvent (IRawElementProviderSimple* provider, EVENTID eventID)
    {
        return uiaRaiseAutomationEvent != nullptr ? uiaRaiseAutomationEvent (provider, eventID)
                                                  : (HRESULT) UIA_E_NOTSUPPORTED;
    }

    BOOL clientsAreListening()
    {
        return uiaClientsAreListening != nullptr ? uiaClientsAreListening()
                                                 : false;
    }

    HIGGS_COMRESULT disconnectProvider (IRawElementProviderSimple* provider)
    {
        if (uiaDisconnectProvider != nullptr)
        {
            const ScopedValueSetter<IRawElementProviderSimple*> disconnectingProviderSetter (disconnectingProvider, provider);
            return uiaDisconnectProvider (provider);
        }

        return (HRESULT) UIA_E_NOTSUPPORTED;
    }

    HIGGS_COMRESULT disconnectAllProviders()
    {
        if (! HIGGSApplicationBase::isStandaloneApp())
            return (HRESULT) UIA_E_NOTSUPPORTED;

        if (uiaDisconnectAllProviders != nullptr)
        {
            const ScopedValueSetter<bool> disconnectingAllProvidersSetter (disconnectingAllProviders, true);
            return uiaDisconnectAllProviders();
        }

        return (HRESULT) UIA_E_NOTSUPPORTED;
    }

    //==============================================================================
    bool isProviderDisconnecting (IRawElementProviderSimple* provider)
    {
        return disconnectingProvider == provider || disconnectingAllProviders;
    }

    //==============================================================================
    HIGGS_DECLARE_SINGLETON_SINGLETHREADED_MINIMAL_INLINE (WindowsUIAWrapper)

private:
    //==============================================================================
    WindowsUIAWrapper()
    {
        // force UIA COM library initialisation here to prevent an exception when calling methods from SendMessage()
        if (isLoaded())
            returnRawElementProvider (nullptr, 0, 0, nullptr);
        else
            jassertfalse; // UIAutomationCore could not be loaded!
    }

    ~WindowsUIAWrapper()
    {
        disconnectAllProviders();

        if (uiaHandle != nullptr)
            ::FreeLibrary (uiaHandle);

        clearSingletonInstance();
    }

    //==============================================================================
    template <typename FuncType>
    static FuncType getUiaFunction (HMODULE module, LPCSTR funcName)
    {
        HIGGS_BEGIN_IGNORE_WARNINGS_GCC_LIKE ("-Wcast-function-type")
        return (FuncType) GetProcAddress (module, funcName);
        HIGGS_END_IGNORE_WARNINGS_GCC_LIKE
    }

    //==============================================================================
    using UiaReturnRawElementProviderFunc            = LRESULT (WINAPI*) (HWND, WPARAM, LPARAM, IRawElementProviderSimple*);
    using UiaHostProviderFromHwndFunc                = HRESULT (WINAPI*) (HWND, IRawElementProviderSimple**);
    using UiaRaiseAutomationPropertyChangedEventFunc = HRESULT (WINAPI*) (IRawElementProviderSimple*, PROPERTYID, VARIANT, VARIANT);
    using UiaRaiseAutomationEventFunc                = HRESULT (WINAPI*) (IRawElementProviderSimple*, EVENTID);
    using UiaClientsAreListeningFunc                 = BOOL    (WINAPI*) ();
    using UiaDisconnectProviderFunc                  = HRESULT (WINAPI*) (IRawElementProviderSimple*);
    using UiaDisconnectAllProvidersFunc              = HRESULT (WINAPI*) ();

    HMODULE uiaHandle = ::LoadLibraryA ("UIAutomationCore.dll");
    UiaReturnRawElementProviderFunc            uiaReturnRawElementProvider            = getUiaFunction<UiaReturnRawElementProviderFunc>            (uiaHandle, "UiaReturnRawElementProvider");
    UiaHostProviderFromHwndFunc                uiaHostProviderFromHwnd                = getUiaFunction<UiaHostProviderFromHwndFunc>                (uiaHandle, "UiaHostProviderFromHwnd");
    UiaRaiseAutomationPropertyChangedEventFunc uiaRaiseAutomationPropertyChangedEvent = getUiaFunction<UiaRaiseAutomationPropertyChangedEventFunc> (uiaHandle, "UiaRaiseAutomationPropertyChangedEvent");
    UiaRaiseAutomationEventFunc                uiaRaiseAutomationEvent                = getUiaFunction<UiaRaiseAutomationEventFunc>                (uiaHandle, "UiaRaiseAutomationEvent");
    UiaClientsAreListeningFunc                 uiaClientsAreListening                 = getUiaFunction<UiaClientsAreListeningFunc>                 (uiaHandle, "UiaClientsAreListening");
    UiaDisconnectProviderFunc                  uiaDisconnectProvider                  = getUiaFunction<UiaDisconnectProviderFunc>                  (uiaHandle, "UiaDisconnectProvider");
    UiaDisconnectAllProvidersFunc              uiaDisconnectAllProviders              = getUiaFunction<UiaDisconnectAllProvidersFunc>              (uiaHandle, "UiaDisconnectAllProviders");

    IRawElementProviderSimple* disconnectingProvider = nullptr;
    bool disconnectingAllProviders = false;

    //==============================================================================
    HIGGS_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (WindowsUIAWrapper)
};

} // namespace higgs
