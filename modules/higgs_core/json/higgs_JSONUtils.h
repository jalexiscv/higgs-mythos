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

/**
    A mini namespace to hold utility functions for working with higgs::vars.

    @tags{Core}
*/
struct JSONUtils
{
    /** No constructor. */
    JSONUtils() = delete;

    /** Given a JSON array/object 'v' and a string representing a JSON pointer
        this returns a copy of the value referenced by 'pointer'.

        If the pointer cannot be followed, due to referencing missing array indices
        or fields, then this returns nullopt.

        For more details, check the JSON Pointer RFC 6901:
        https://datatracker.ietf.org/doc/html/rfc6901
    */
    static std::optional<var> getPointer (const var& v, String pointer);

    /** Given a JSON array/object 'v', a string representing a JSON pointer,
        and a new property value 'newValue', returns a copy of 'v' where the
        property or array index referenced by the pointer has been set to 'newValue'.

        If the pointer cannot be followed, due to referencing missing array indices
        or fields, then this returns nullopt.

        For more details, check the JSON Pointer RFC 6901:
        https://datatracker.ietf.org/doc/html/rfc6901
    */
    static std::optional<var> setPointer (const var& v, String pointer, const var& newValue);

    /** Converts the provided key/value pairs into a JSON object. */
    static var makeObject (const std::map<Identifier, var>& source);

    /** Converts the provided key/value pairs into a JSON object with the provided
        key at the first position in the object.

        This is useful because the MIDI-CI spec requires that certain fields (e.g.
        status) should be placed at the beginning of a MIDI-CI header.
    */
    static var makeObjectWithKeyFirst (const std::map<Identifier, var>& source, Identifier key);

    /** Returns true if and only if the contents of a match the contents of b.

        Unlike var::operator==, this will recursively check that contained DynamicObject and Array
        instances compare equal.
    */
    static bool deepEqual (const var& a, const var& b);
};

} // namespace higgs
