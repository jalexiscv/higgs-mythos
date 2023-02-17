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

/** @cond */
namespace higgs::universal_midi_packets
{

/**
    This struct acts as a single-file namespace for Universal MIDI Packet
    functionality related to 7-bit SysEx.

    @tags{Audio}
*/
struct SysEx7
{
    /** Returns the number of 64-bit packets required to hold a series of
        SysEx bytes.

        The number passed to this function should exclude the leading/trailing
        SysEx bytes used in an old midi bytestream, as these are not required
        when using Universal MIDI Packets.
    */
    static uint32_t getNumPacketsRequiredForDataSize (uint32_t);

    /** The different kinds of UMP SysEx-7 message. */
    enum class Kind : uint8_t
    {
        /** The whole message fits in a single 2-word packet. */
        complete     = 0,

        /** The packet begins a SysEx message that will continue in subsequent packets. */
        begin        = 1,

        /** The packet is a continuation of an ongoing SysEx message. */
        continuation = 2,

        /** The packet terminates an ongoing SysEx message. */
        end          = 3
    };

    /** Holds the bytes from a single SysEx-7 packet. */
    struct PacketBytes
    {
        std::array<std::byte, 6> data;
        uint8_t size;
    };

    /** Extracts the data bytes from a 64-bit data message. */
    static PacketBytes getDataBytes (const PacketX2& packet);
};

} // namespace higgs::universal_midi_packets
/** @endcond */
