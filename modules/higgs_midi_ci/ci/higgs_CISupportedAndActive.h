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
    Holds the maximum number of channels that may be activated for a MIDI-CI
    profile, along with the number of channels that are currently active.

    @tags{Audio}
*/
struct SupportedAndActive
{
    uint16_t supported{};   ///< The maximum number of member channels for a profile.
                            ///< 0 indicates that the profile is unsupported.
                            ///< For group/block profiles, 1/0 indicates that the
                            ///< profile is supported/unsupported respectively.

    uint16_t active{};      ///< The number of member channels currently active for a profile.
                            ///< 0 indicates that the profile is inactive.
                            ///< For group/block profiles, 1/0 indicates that the
                            ///< profile is supported/unsupported respectively.

    /** Returns true if supported is non-zero. */
    bool isSupported()  const { return supported != 0; }

    /** Returns true if active is non-zero. */
    bool isActive()     const { return active != 0; }

    bool operator== (const SupportedAndActive& other) const
    {
        const auto tie = [] (auto& x) { return std::tie (x.supported, x.active); };
        return tie (*this) == tie (other);
    }

    bool operator!= (const SupportedAndActive& other) const { return ! operator== (other); }
};

} // namespace higgs::midi_ci
