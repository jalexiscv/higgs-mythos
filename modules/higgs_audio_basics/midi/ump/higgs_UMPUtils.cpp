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

namespace higgs::universal_midi_packets
{

uint32_t Utils::getNumWordsForMessageType (uint32_t mt)
{
    switch ((uint8_t) Utils::getMessageType (mt))
    {
        case 0x0:
        case 0x1:
        case 0x2:
        case 0x6:
        case 0x7:
            return 1;
        case 0x3:
        case 0x4:
        case 0x8:
        case 0x9:
        case 0xa:
            return 2;
        case 0xb:
        case 0xc:
            return 3;
        case 0x5:
        case 0xd:
        case 0xe:
        case 0xf:
            return 4;
    }

    jassertfalse;
    return 1;
}

} // namespace higgs::universal_midi_packets
