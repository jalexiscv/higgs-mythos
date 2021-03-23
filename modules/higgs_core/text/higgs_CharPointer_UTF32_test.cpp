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

class CharPointer_UTF32Test final : public UnitTest
{
public:
    CharPointer_UTF32Test() : UnitTest { "CharPointer_UTF32", UnitTestCategories::text } {}

    void runTest() final
    {
        using Utf32Vector = std::vector<CharPointer_UTF32::CharType>;

        const auto getNumBytes = [] (const auto& str)
        {
            return (int) (sizeof (CharPointer_UTF32::CharType) * str.size());
        };

        beginTest ("String validation - empty string / null-terminator");
        {
            const Utf32Vector string { 0x0 };
            expect (CharPointer_UTF32::isValidString (string.data(), getNumBytes (string)));
        }

        beginTest ("String validation - ascii");
        {
            const Utf32Vector string { 0x54, 0x65, 0x73, 0x74, 0x21, 0x0 }; // Test!
            expect (CharPointer_UTF32::isValidString (string.data(), getNumBytes (string)));
        }

        beginTest ("String validation - 2-byte code points");
        {
            const Utf32Vector string { 0x54, 0x65, 0x73, 0x74, 0x20ac, 0x0 }; // Test€
            expect (CharPointer_UTF32::isValidString (string.data(), getNumBytes (string)));
        }

        beginTest ("String validation - maximum code point");
        {
            const Utf32Vector string1 { 0x54, 0x65, 0x73, 0x74, 0x10ffff, 0x0 };
            expect (CharPointer_UTF32::isValidString (string1.data(), getNumBytes (string1)));

            const Utf32Vector string2 { 0x54, 0x65, 0x73, 0x74, 0x110000, 0x0 };
            expect (! CharPointer_UTF32::isValidString (string2.data(), getNumBytes (string2)));
        }

        beginTest ("String validation - characters after a null terminator are ignored");
        {
            const Utf32Vector string { 0x54, 0x65, 0x73, 0x74, 0x0, 0x110000 };
            expect (CharPointer_UTF32::isValidString (string.data(), getNumBytes (string)));
        }

        beginTest ("String validation - characters exceeding max bytes are ignored");
        {
            const Utf32Vector string { 0x54, 0x65, 0x73, 0x74, 0x110000 };
            expect (CharPointer_UTF32::isValidString (string.data(), 8));
        }

        beginTest ("String validation - surrogate code points are invalid");
        {
            const Utf32Vector highSurrogate { 0xd800 };
            expect (! CharPointer_UTF32::isValidString (highSurrogate.data(), getNumBytes (highSurrogate)));

            const Utf32Vector lowSurrogate { 0xdfff };
            expect (! CharPointer_UTF32::isValidString (lowSurrogate.data(), getNumBytes (lowSurrogate)));
        }
    }
};


static CharPointer_UTF32Test charPointer_UTF32Test;

} // namespace higgs
