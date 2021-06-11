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

class WinRTWrapper :   public DeletedAtShutdown
{
public:
    //==============================================================================
    ~WinRTWrapper();
    bool isInitialised() const noexcept  { return initialised; }

    HIGGS_DECLARE_SINGLETON_INLINE (WinRTWrapper, false)

    //==============================================================================
    template <class ComClass>
    ComSmartPtr<ComClass> activateInstance (const wchar_t* runtimeClassID, REFCLSID classUUID)
    {
        ComSmartPtr<ComClass> result;

        if (isInitialised())
        {
            ComSmartPtr<IInspectable> inspectable;
            ScopedHString runtimeClass (runtimeClassID);
            auto hr = roActivateInstance (runtimeClass.get(), inspectable.resetAndGetPointerAddress());

            if (SUCCEEDED (hr))
                inspectable->QueryInterface (classUUID, (void**) result.resetAndGetPointerAddress());
        }

        return result;
    }

    template <class ComClass>
    ComSmartPtr<ComClass> getWRLFactory (const wchar_t* runtimeClassID)
    {
        ComSmartPtr<ComClass> comPtr;

        if (isInitialised())
        {
            ScopedHString classID (runtimeClassID);

            if (classID.get() != nullptr)
                roGetActivationFactory (classID.get(), __uuidof (ComClass), (void**) comPtr.resetAndGetPointerAddress());
        }

        return comPtr;
    }

    //==============================================================================
    class ScopedHString
    {
    public:
        ScopedHString (String);
        ~ScopedHString();

        HSTRING get() const noexcept          { return hstr; }

    private:
        HSTRING hstr = nullptr;

        HIGGS_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ScopedHString)
    };

    String hStringToString (HSTRING);

private:
    WinRTWrapper();

    //==============================================================================
    HMODULE winRTHandle = nullptr;
    bool initialised = false;

    typedef HRESULT (WINAPI* RoInitializeFuncPtr) (int);
    typedef HRESULT (WINAPI* WindowsCreateStringFuncPtr) (LPCWSTR, UINT32, HSTRING*);
    typedef HRESULT (WINAPI* WindowsDeleteStringFuncPtr) (HSTRING);
    typedef PCWSTR  (WINAPI* WindowsGetStringRawBufferFuncPtr) (HSTRING, UINT32*);
    typedef HRESULT (WINAPI* RoActivateInstanceFuncPtr) (HSTRING, IInspectable**);
    typedef HRESULT (WINAPI* RoGetActivationFactoryFuncPtr) (HSTRING, REFIID, void**);

    RoInitializeFuncPtr roInitialize = nullptr;
    WindowsCreateStringFuncPtr createHString = nullptr;
    WindowsDeleteStringFuncPtr deleteHString = nullptr;
    WindowsGetStringRawBufferFuncPtr getHStringRawBuffer = nullptr;
    RoActivateInstanceFuncPtr roActivateInstance = nullptr;
    RoGetActivationFactoryFuncPtr roGetActivationFactory = nullptr;

    //==============================================================================
    HIGGS_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (WinRTWrapper)
};

} // namespace higgs
