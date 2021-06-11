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

WinRTWrapper::WinRTWrapper()
{
    winRTHandle = ::LoadLibraryA ("api-ms-win-core-winrt-l1-1-0");

    if (winRTHandle == nullptr)
        return;

    HIGGS_BEGIN_IGNORE_WARNINGS_GCC_LIKE ("-Wcast-function-type")

    roInitialize           = (RoInitializeFuncPtr)              ::GetProcAddress (winRTHandle, "RoInitialize");
    createHString          = (WindowsCreateStringFuncPtr)       ::GetProcAddress (winRTHandle, "WindowsCreateString");
    deleteHString          = (WindowsDeleteStringFuncPtr)       ::GetProcAddress (winRTHandle, "WindowsDeleteString");
    getHStringRawBuffer    = (WindowsGetStringRawBufferFuncPtr) ::GetProcAddress (winRTHandle, "WindowsGetStringRawBuffer");
    roActivateInstance     = (RoActivateInstanceFuncPtr)        ::GetProcAddress (winRTHandle, "RoActivateInstance");
    roGetActivationFactory = (RoGetActivationFactoryFuncPtr)    ::GetProcAddress (winRTHandle, "RoGetActivationFactory");

    HIGGS_END_IGNORE_WARNINGS_GCC_LIKE

    if (roInitialize == nullptr || createHString == nullptr || deleteHString == nullptr
        || getHStringRawBuffer == nullptr || roActivateInstance == nullptr || roGetActivationFactory == nullptr)
        return;

    HRESULT status = roInitialize (1);
    initialised = ! (status != S_OK && status != S_FALSE && status != (HRESULT) 0x80010106L);
}

WinRTWrapper::~WinRTWrapper()
{
    if (winRTHandle != nullptr)
        ::FreeLibrary (winRTHandle);

    clearSingletonInstance();
}

WinRTWrapper::ScopedHString::ScopedHString (String str)
{
    if (WinRTWrapper::getInstance()->isInitialised())
        WinRTWrapper::getInstance()->createHString (str.toWideCharPointer(),
                                                    static_cast<uint32_t> (str.length()),
                                                    &hstr);
}

WinRTWrapper::ScopedHString::~ScopedHString()
{
    if (WinRTWrapper::getInstance()->isInitialised() && hstr != nullptr)
        WinRTWrapper::getInstance()->deleteHString (hstr);
}

String WinRTWrapper::hStringToString (HSTRING hstr)
{
    if (isInitialised())
        if (const wchar_t* str = getHStringRawBuffer (hstr, nullptr))
            return String (str);

    return {};
}

}
