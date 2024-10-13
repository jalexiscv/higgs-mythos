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
    Utility functions for working with data formats used by property exchange
    messages.

    @tags{Audio}
*/
struct Encodings
{
    /** Text in ACK and NAK messages can't be utf-8 or ASCII because each byte only has 7 usable bits.
        The encoding rules are in section 5.10.4 of the CI spec.
    */
    static String stringFrom7BitText (Span<const std::byte> bytes);

    /** Text in ACK and NAK messages can't be utf-8 or ASCII because each byte only has 7 usable bits.
        The encoding rules are in section 5.10.4 of the CI spec.
    */
    static std::vector<std::byte> stringTo7BitText (const String& text);

    /** Converts a list of bytes representing a 7-bit ASCII string to JSON. */
    static var jsonFrom7BitText (Span<const std::byte> bytes)
    {
        return JSON::parse (stringFrom7BitText (bytes));
    }

    /** Converts a JSON object to a list of bytes in 7-bit ASCII format. */
    static std::vector<std::byte> jsonTo7BitText (const var& v)
    {
        return stringTo7BitText (JSON::toString (v, JSON::FormatOptions{}.withSpacing (JSON::Spacing::none)));
    }

    /** Each group of seven stored bytes is transmitted as eight bytes.
        First, the sign bits of the seven bytes are sent, followed by the low-order 7 bits of each byte.
    */
    static std::vector<std::byte> toMcoded7 (Span<const std::byte> bytes);

    /** Each group of seven stored bytes is transmitted as eight bytes.
        First, the sign bits of the seven bytes are sent, followed by the low-order 7 bits of each byte.
    */
    static std::vector<std::byte> fromMcoded7 (Span<const std::byte> bytes);

    /** Attempts to encode the provided byte span using the specified encoding.

        The ASCII encoding does not make any changes to the input stream, but
        encoding will fail if any byte has its most significant bit set.
    */
    static std::optional<std::vector<std::byte>> tryEncode (Span<const std::byte> bytes,
                                                            Encoding mutualEncoding);

    /** Decodes the provided byte span using the specified encoding.

        All bytes of the input must be 7-bit values, i.e. all most-significant bits
        are unset.
    */
    static std::vector<std::byte> decode (Span<const std::byte> bytes, Encoding mutualEncoding);

    Encodings() = delete;
};

} // namespace higgs::midi_ci
