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
    Contains some static methods for converting between binary and the
    standard base-64 encoding format.

    @tags{Core}
*/
struct HIGGS_API Base64
{
    /** Converts a binary block of data into a base-64 string.
        This will write the resulting string data to the given stream.
        If a write error occurs with the stream, the method will terminate and return false.
    */
    static bool convertToBase64 (OutputStream& base64Result, const void* sourceData, size_t sourceDataSize);

    /** Converts a base-64 string back to its binary representation.
        This will write the decoded binary data to the given stream.
        If the string is not valid base-64, the method will terminate and return false.
    */
    static bool convertFromBase64 (OutputStream& binaryOutput, StringRef base64TextInput);

    /** Converts a block of binary data to a base-64 string. */
    static String toBase64 (const void* sourceData, size_t sourceDataSize);

    /** Converts a string's UTF-8 representation to a base-64 string. */
    static String toBase64 (const String& textToEncode);
};

} // namespace higgs
