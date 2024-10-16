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

SupportedAndActive ChannelProfileStates::get (const Profile& profile) const
{
    const auto iter = std::lower_bound (entries.begin(), entries.end(), profile);

    if (iter != entries.end() && iter->profile == profile)
        return iter->state;

    return {};
}

std::vector<Profile> ChannelProfileStates::getActive() const
{
    std::vector<Profile> result;

    for (const auto& item : entries)
        if (item.state.isActive())
            result.push_back (item.profile);

    return result;
}

std::vector<Profile> ChannelProfileStates::getInactive() const
{
    std::vector<Profile> result;

    for (const auto& item : entries)
        if (item.state.isSupported())
            result.push_back (item.profile);

    return result;
}

void ChannelProfileStates::set (const Profile& profile, SupportedAndActive state)
{
    const auto iter = std::lower_bound (entries.begin(), entries.end(), profile);

    if (iter != entries.end() && iter->profile == profile)
    {
        if (state != SupportedAndActive{})
            iter->state = state;
        else
            entries.erase (iter);
    }
    else if (state != SupportedAndActive{})
    {
        entries.insert (iter, { profile, state });
    }
}

void ChannelProfileStates::erase (const Profile& profile)
{
    const auto iter = std::lower_bound (entries.begin(), entries.end(), profile);

    if (iter != entries.end() && iter->profile == profile)
        entries.erase (iter);
}

} // namespace higgs::midi_ci
