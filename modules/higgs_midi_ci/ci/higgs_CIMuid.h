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
    A 28-bit ID that uniquely identifies a device taking part in a series of
    MIDI-CI transactions.

    @tags{Audio}
*/
class MUID
{
    constexpr explicit MUID (uint32_t v) : value (v) {}

    // 0x0fffff00 to 0x0ffffffe are reserved, 0x0fffffff is 'broadcast'
    static constexpr uint32_t userMuidEnd = 0x0fffff00;
    static constexpr uint32_t mask = 0x0fffffff;
    uint32_t value{};

public:
    /** Returns the ID as a plain integer. */
    constexpr uint32_t get() const { return value; }

    /** Converts the provided integer to a MUID without validation that it
        is within the allowed range.
    */
    static MUID makeUnchecked (uint32_t v)
    {
        // If this is hit, the MUID has too many bits set!
        jassert ((v & mask) == v);
        return MUID (v);
    }

    /** Returns a MUID if the provided value is within the valid range for
        MUID values; otherwise returns nullopt.
    */
    static std::optional<MUID> make (uint32_t v)
    {
        if ((v & mask) == v)
            return makeUnchecked (v);

        return {};
    }

    /** Makes a random MUID using the provided random engine. */
    static MUID makeRandom (Random& r)
    {
        return makeUnchecked ((uint32_t) r.nextInt (userMuidEnd));
    }

    bool operator== (const MUID other) const { return value == other.value; }
    bool operator!= (const MUID other) const { return value != other.value; }
    bool operator<  (const MUID other) const { return value <  other.value; }

    /** Returns the special MUID representing the broadcast address. */
    static constexpr MUID getBroadcast() { return MUID { mask }; }
};

} // namespace higgs::midi_ci
