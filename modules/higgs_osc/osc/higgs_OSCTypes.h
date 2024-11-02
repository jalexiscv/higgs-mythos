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
/** The type used for OSC type tags. */
using OSCType = char;


/** The type used for OSC type tag strings. */
using OSCTypeList = Array<OSCType>;

//==============================================================================
/** The definitions of supported OSC types and their associated OSC type tags,
    as defined in the OpenSoundControl 1.0 specification.

    Note: This implementation does not support any additional type tags that
    are not part of the specification.

    @tags{OSC}
*/
class HIGGS_API  OSCTypes
{
public:
    static const OSCType int32;
    static const OSCType float32;
    static const OSCType string;
    static const OSCType blob;
    static const OSCType colour;

    static bool isSupportedType (OSCType type) noexcept
    {
        return type == OSCTypes::int32
            || type == OSCTypes::float32
            || type == OSCTypes::string
            || type == OSCTypes::blob
            || type == OSCTypes::colour;
    }
};


//==============================================================================
/**
    Holds a 32-bit RGBA colour for passing to and from an OSCArgument.
    @see OSCArgument, OSCTypes::colour
    @tags{OSC}
*/
struct OSCColour
{
    uint8 red, green, blue, alpha;

    uint32 toInt32() const;
    static OSCColour fromInt32 (uint32);
};


//==============================================================================
/** Base class for exceptions that can be thrown by methods in the OSC module.

    @tags{OSC}
*/
struct OSCException  : public std::exception
{
    OSCException (const String& desc)
        : description (desc)
    {
       #if ! HIGGS_UNIT_TESTS
        DBG ("OSCFormatError: " + description);
       #endif
    }

    String description;
};

//==============================================================================
/** Exception type thrown when the OSC module fails to parse something because
    of a data format not compatible with the OpenSoundControl 1.0 specification.

    @tags{OSC}
*/
struct OSCFormatError : public OSCException
{
    OSCFormatError (const String& desc) : OSCException (desc) {}
};

//==============================================================================
/** Exception type thrown in cases of unexpected errors in the OSC module.

    Note: This should never happen, and all the places where this is thrown
    should have a preceding jassertfalse to facilitate debugging.

    @tags{OSC}
*/
struct OSCInternalError : public OSCException
{
    OSCInternalError (const String& desc) : OSCException (desc) {}
};

} // namespace higgs
