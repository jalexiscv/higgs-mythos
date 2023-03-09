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
#if (defined (MAC_OS_VERSION_11_0) || defined (__IPHONE_14_0))
 #define HIGGS_AUDIOWORKGROUP_TYPES_AVAILABLE    1
#else
 #define HIGGS_AUDIOWORKGROUP_TYPES_AVAILABLE    0
#endif

#if HIGGS_AUDIOWORKGROUP_TYPES_AVAILABLE
 AudioWorkgroup makeRealAudioWorkgroup (os_workgroup_t handle);
#endif

}
