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
    void sendAccessibilityAutomationEvent (const AccessibilityHandler&, EVENTID);
    void sendAccessibilityPropertyChangedEvent (const AccessibilityHandler&, PROPERTYID, VARIANT);
} // namespace higgs

#include "higgs_UIAProviderBase_windows.h"
#include "higgs_UIAExpandCollapseProvider_windows.h"
#include "higgs_UIAGridItemProvider_windows.h"
#include "higgs_UIAGridProvider_windows.h"
#include "higgs_UIAInvokeProvider_windows.h"
#include "higgs_UIARangeValueProvider_windows.h"
#include "higgs_UIASelectionProvider_windows.h"
#include "higgs_UIATextProvider_windows.h"
#include "higgs_UIAToggleProvider_windows.h"
#include "higgs_UIATransformProvider_windows.h"
#include "higgs_UIAValueProvider_windows.h"
#include "higgs_UIAWindowProvider_windows.h"
