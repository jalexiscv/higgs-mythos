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
    Parses CI messages.

    @tags{Audio}
*/
class Parser
{
public:
    Parser() = delete;

    enum class Status
    {
        noError,                ///< Parsing was successful
        mismatchedMUID,         ///< The message destination MUID doesn't match the provided MUID
        collidingMUID,          ///< The message source MUID matches the provided MUID
        unrecognisedMessage,    ///< The message ID doesn't correspond to a known message
        reservedVersion,        ///< The MIDI CI version uses an unrecognised major version
        malformed,              ///< The message (whole message, or just body) could not be parsed
    };

    /** Parses the provided message;

        Call this with a full CI message. Don't include any "extra" bytes such as
        the leading/trailing 0xf0/0xf7 for messages that were originally in bytestream midi format,
        or the packet-header bytes from UMP-formatted sysex messages.

        Returns nullopt if the message doesn't need to be acknowledged by the entity with the provided MUID,
        or if the message is malformed.
        Otherwise, returns a parsed header, and optionally a body.
        If the body is std::monostate, then something went wrong while parsing. For example, the body
        may be malformed, or the CI version might be unrecognised.
    */
    static std::optional<Message::Parsed> parse (MUID ourMUID, Span<const std::byte> message, Status* = nullptr);

    /** Parses the provided message;

        Call this with a full CI message. Don't include any "extra" bytes such as
        the leading/trailing 0xf0/0xf7 for messages that were originally in bytestream midi format,
        or the packet-header bytes from UMP-formatted sysex messages.

        Returns nullopt if the message is malformed.
        Otherwise, returns a parsed header, and optionally a body.
        If the body is std::monostate, then something went wrong while parsing. For example, the body
        may be malformed, or the CI version might be unrecognised.
    */
    static std::optional<Message::Parsed> parse (Span<const std::byte> message, Status* = nullptr);

    /** Returns a human-readable string describing the message. */
    static String getMessageDescription (const Message::Parsed& message);
};

} // namespace higgs::midi_ci
