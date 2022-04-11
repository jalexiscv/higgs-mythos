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

namespace higgs::detail
{

String AccessibilityHelpers::getApplicationOrPluginName()
{
   #if defined (HiggsPlugin_Name)
    return HiggsPlugin_Name;
   #else
    if (auto* app = HIGGSApplicationBase::getInstance())
        return app->getApplicationName();

    return "HIGGS Application";
   #endif
}

#if ! HIGGS_NATIVE_ACCESSIBILITY_INCLUDED
 void AccessibilityHelpers::notifyAccessibilityEvent (const AccessibilityHandler&, Event) {}
#endif

} // namespace higgs::detail
