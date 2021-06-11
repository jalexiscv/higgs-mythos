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

#if HIGGS_MAC

class ScopedLowPowerModeDisabler::Pimpl
{
public:
    Pimpl()
    {
        activity = [[NSProcessInfo processInfo] beginActivityWithOptions: NSActivityUserInitiatedAllowingIdleSystemSleep
                                                                  reason: @"App must remain in high-power mode"];
    }

    ~Pimpl()
    {
        [[NSProcessInfo processInfo] endActivity: activity];
    }

private:
    id activity;

    HIGGS_DECLARE_NON_COPYABLE (Pimpl)
    HIGGS_DECLARE_NON_MOVEABLE (Pimpl)
};

#else

class ScopedLowPowerModeDisabler::Pimpl {};

#endif

//==============================================================================
ScopedLowPowerModeDisabler::ScopedLowPowerModeDisabler()
    : pimpl (std::make_unique<Pimpl>()) {}

ScopedLowPowerModeDisabler::~ScopedLowPowerModeDisabler() = default;

} // namespace higgs
