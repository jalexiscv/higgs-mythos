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

/** Kinds of MIDI message transport.
*/
enum class Transport : uint8_t
{
    bytestream, ///< A stream of variable-length messages. Suitable for MIDI 1.0.
    ump,        ///< A stream of 32-bit words. Suitable for MIDI-1UP and MIDI 2.0.
};

/** The kinds of MIDI protocol that can be formatted into Universal MIDI Packets.
*/
enum class PacketProtocol : uint8_t
{
    MIDI_1_0,
    MIDI_2_0,
};

/** All kinds of MIDI protocol understood by HIGGS.
*/
enum class MidiProtocol : uint8_t
{
    bytestream,
    UMP_MIDI_1_0,
    UMP_MIDI_2_0,
};

} // namespace higgs::universal_midi_packets
/** @endcond */
