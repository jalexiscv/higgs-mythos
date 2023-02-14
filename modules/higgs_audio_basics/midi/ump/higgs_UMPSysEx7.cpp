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

uint32_t SysEx7::getNumPacketsRequiredForDataSize (uint32_t size)
{
    constexpr auto denom = 6;
    return (size / denom) + ((size % denom) != 0);
}

SysEx7::PacketBytes SysEx7::getDataBytes (const PacketX2& packet)
{
    const auto numBytes = Utils::getChannel (packet[0]);
    constexpr uint8_t maxBytes = 6;
    jassert (numBytes <= maxBytes);

    return
    {
        { { std::byte { packet.getU8<2>() },
            std::byte { packet.getU8<3>() },
            std::byte { packet.getU8<4>() },
            std::byte { packet.getU8<5>() },
            std::byte { packet.getU8<6>() },
            std::byte { packet.getU8<7>() } } },
        jmin (numBytes, maxBytes)
    };
}

} // namespace higgs::universal_midi_packets
