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
    Contains information about a MIDI 2.0 function block.

    @tags{Audio}
*/
struct FunctionBlock
{
    std::byte identifier { 0x7f }; ///< 0x7f == no function block
    uint8_t firstGroup = 0;        ///< The first group that is part of the block, 0-based
    uint8_t numGroups = 1;         ///< The number of groups contained in the block

    bool operator== (const FunctionBlock& other) const
    {
        const auto tie = [] (auto& x) { return std::tie (x.identifier, x.firstGroup, x.numGroups); };
        return tie (*this) == tie (other);
    }

    bool operator!= (const FunctionBlock& other) const { return ! operator== (other); }
};

} // namespace higgs::midi_ci
