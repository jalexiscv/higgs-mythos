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

bool HiddenMessageWindow::setDPIAwareness()
{
    static const auto didSetDpiAwareness = std::invoke ([]
    {
        // The docs for SetProcessDpiAwarenessContext specify that the call will fail if the DPI
        // context has already been set. Separately, the documentation page "Setting the default DPI
        // awareness for a process" [1] says that "Once a window (an HWND) has been created in your
        // process, changing the DPI awareness mode is no longer supported".
        // This is the behaviour we want; unfortunately, we see that SetProcessDpiAwarenessContext
        // may change the dpi awareness mode and return true, even after the process has created a
        // window, at least on Windows 11 25H2.
        // As a workaround, we only attempt to set a new dpi awareness mode if the process does not
        // have any windows in existence. The assumption here is that applications are written
        // according to the _documented_ behaviour, and therefore are guaranteed to have set the
        // desired process dpi awareness mode before any window is created.
        //
        // [1]: https://learn.microsoft.com/en-us/windows/win32/hidpi/setting-the-default-dpi-awareness-for-a-process

        const WNDENUMPROC callback = [] (HWND h, LPARAM ptr) -> BOOL
        {
            DWORD procOut{};
            GetWindowThreadProcessId (h, &procOut);

            if (procOut != GetCurrentProcessId())
            {
                return TRUE;
            }

            *reinterpret_cast<int*> (ptr) += 1;
            // We just want to know if at least one window exists.
            // Short-circuit so we don't unnecessarily visit every window.
            return FALSE;
        };

        int counter = 0;
        EnumWindows (callback, reinterpret_cast<LPARAM> (&counter));

        if (counter != 0)
            return false;

        for (auto* moduleName : { "SHCore.dll", "User32.dll" })
        {
            LoadLibraryA (moduleName);
            const auto module = GetModuleHandleA (moduleName);

            if (module == nullptr)
                continue;

            using SetProcessDpiAwarenessContextFunc = BOOL (WINAPI*) (DPI_AWARENESS_CONTEXT);

            const auto setProcessDpiAwarenessContext = (SetProcessDpiAwarenessContextFunc) GetProcAddress (module, "SetProcessDpiAwarenessContext");

            if (setProcessDpiAwarenessContext == nullptr)
                continue;

            if (setProcessDpiAwarenessContext (DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2))
                return true;

            const auto error = GetLastError();

            if (error == ERROR_ACCESS_DENIED)
                return false;

            break;
        }

        if (SUCCEEDED (SetProcessDpiAwareness (PROCESS_PER_MONITOR_DPI_AWARE)))
            return true;

        if (SUCCEEDED (SetProcessDpiAwareness (PROCESS_SYSTEM_DPI_AWARE)))
            return true;

        return SetProcessDPIAware() != 0;
    });

    return didSetDpiAwareness;
}

} // namespace higgs
