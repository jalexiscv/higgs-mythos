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

namespace higgs::midi_ci
{

/**
    An interface with methods that can be overridden to customise how a Device
    implementing profiles responds to profile inquiries.

    @tags{Audio}
*/
struct ProfileDelegate
{
    ProfileDelegate() = default;
    virtual ~ProfileDelegate() = default;
    ProfileDelegate (const ProfileDelegate&) = default;
    ProfileDelegate (ProfileDelegate&&) = default;
    ProfileDelegate& operator= (const ProfileDelegate&) = default;
    ProfileDelegate& operator= (ProfileDelegate&&) = default;

    /** Called when a remote device requests that a profile is enabled or disabled.

        Old MIDI-CI implementations on remote devices may request that a profile
        is enabled with zero channels active - in this situation, it is
        recommended that you use ProfileHost::enableProfile to enable the
        default number of channels for that profile.

        Additionally, profiles for entire groups or function blocks may be enabled with zero
        active channels. In this case, the profile should be enabled on the entire group or
        function block.
    */
    virtual void profileEnablementRequested ([[maybe_unused]] MUID x,
                                             [[maybe_unused]] ProfileAtAddress profileAtAddress,
                                             [[maybe_unused]] int numChannels,
                                             [[maybe_unused]] bool enabled) = 0;
};

} // namespace higgs::midi_ci
