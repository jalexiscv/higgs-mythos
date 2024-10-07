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
    Identifies a channel or set of channels in a multi-group MIDI endpoint.

    @tags{Audio}
*/
class ChannelAddress
{
private:
    uint8_t group{};              ///< A group within a MIDI endpoint, where 0 <= group && group < 16
    ChannelInGroup channel{};     ///< A set of channels related to specified group

    auto tie() const;

public:
    /** Returns a copy of this object with the specified group. */
    [[nodiscard]] ChannelAddress withGroup (int g) const
    {
        jassert (isPositiveAndBelow (g, 16));
        return withMember (*this, &ChannelAddress::group, (uint8_t) g);
    }

    /** Returns a copy of this object with the specified channel. */
    [[nodiscard]] ChannelAddress withChannel (ChannelInGroup c) const
    {
        return withMember (*this, &ChannelAddress::channel, c);
    }

    /** Returns the group. */
    [[nodiscard]] uint8_t getGroup()         const  { return group; }

    /** Returns the channel in the group. */
    [[nodiscard]] ChannelInGroup getChannel() const { return channel; }

    /** Returns true if this address refers to all channels in the function
        block containing the specified group.
    */
    bool isBlock()   const { return channel == ChannelInGroup::wholeBlock; }

    /** Returns true if this address refers to all channels in the specified
        group.
    */
    bool isGroup()   const { return channel == ChannelInGroup::wholeGroup; }

    /** Returns true if this address refers to a single channel. */
    bool isSingleChannel() const { return ! isBlock() && ! isGroup(); }

    bool operator<  (const ChannelAddress& other) const;
    bool operator<= (const ChannelAddress& other) const;
    bool operator>  (const ChannelAddress& other) const;
    bool operator>= (const ChannelAddress& other) const;
    bool operator== (const ChannelAddress& other) const;
    bool operator!= (const ChannelAddress& other) const;
};

} // namespace higgs::midi_ci
