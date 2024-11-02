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

namespace higgs
{

//==============================================================================
/**
    An OSC time tag.

    OSC time tags are part of OSCBundle objects.

    In accordance with the OSC 1.0 specification, the internal timestamp stored in
    OSCTimeTag uses the same binary format as NTP timestamps. The representation
    is by a 64 bit fixed point number. The first 32 bits specify the number of
    seconds since midnight on January 1, 1900, and the last 32 bits specify
    fractional parts of a second to a precision of about 200 picoseconds.

    The time tag value consisting of 63 zero bits followed by a one in the least
    significant bit is a special case meaning "immediately".

    For a more user-friendly time format, convert OSCTimeTag to a higgs::Time object
    using toTime().

    @tags{OSC}
*/
class HIGGS_API  OSCTimeTag
{
public:
    //==============================================================================
    /** Default constructor.
        Constructs an OSCTimeTag object with the special value representing "immediately".
    */
    OSCTimeTag() noexcept;

    /** Constructs an OSCTimeTag object from a raw binary OSC time tag. */
    OSCTimeTag (uint64 rawTimeTag) noexcept;

    /** Constructs an OSCTimeTag object from a higgs::Time object. */
    OSCTimeTag (Time time) noexcept;

    /** Returns a higgs::Time object representing the same time as the OSCTimeTag.

        If the OSCTimeTag has the special value representing "immediately", the
        resulting higgs::Time object will represent an arbitrary point of time (but
        guaranteed to be in the past), since higgs::Time does not have such a special value.
    */
    Time toTime() const noexcept;

    /** Returns true if the OSCTimeTag object has the special value representing "immediately". */
    bool isImmediately() const noexcept;

    /** Returns the raw binary OSC time tag representation. */
    uint64 getRawTimeTag() const noexcept               { return rawTimeTag; }

    /** The special value representing "immediately". */
    static const OSCTimeTag immediately;

private:
    //==============================================================================
    uint64 rawTimeTag;
};

} // namespace higgs
