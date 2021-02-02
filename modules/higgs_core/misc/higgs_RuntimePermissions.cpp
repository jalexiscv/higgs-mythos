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

#if ! HIGGS_ANDROID  // We currently don't request runtime permissions on any other platform
                    // than Android, so this file contains a dummy implementation for those.
                    // This may change in the future.

void RuntimePermissions::request (PermissionID, Callback callback)   { callback (true); }
bool RuntimePermissions::isRequired (PermissionID) { return false; }
bool RuntimePermissions::isGranted (PermissionID) { return true; }

#endif

} // namespace higgs
