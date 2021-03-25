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

class CharPointer_UTF8Test final : public UnitTest
{
public:
    CharPointer_UTF8Test() : UnitTest { "CharPointer_UTF8", UnitTestCategories::text } {}

    void runTest() final
    {
        using Utf8Vector = std::vector<CharPointer_UTF8::CharType>;

        testCase ("String validation - empty string / null-terminator", [&]
        {
            const Utf8Vector string { '\0' };
            expect (CharPointer_UTF8::isValidString (string.data(), (int) string.size()));
        });

        testCase ("String validation - ascii", [&]
        {
            const Utf8Vector string { 'T', 'e', 's', 'T', '!', '\0' }; // Test!
            expect (CharPointer_UTF8::isValidString (string.data(), (int) string.size()));
        });

        constexpr auto continuationCharacter = static_cast<char> (0x80);

        testCase ("String validation - continuation characters are invalid when not proceeded by the correct bytes", [&]
        {
            const Utf8Vector string { continuationCharacter };
            expect (! CharPointer_UTF8::isValidString (string.data(), (int) string.size()));
        });

        testCase ("String validation - characters after a null terminator are ignored", [&]
        {
            const Utf8Vector string { 'T', 'e', 's', 'T', '\0', continuationCharacter };
            expect (CharPointer_UTF8::isValidString (string.data(), (int) string.size()));
        });

        testCase ("String validation - characters exceeding max bytes are ignored", [&]
        {
            const Utf8Vector string { 'T', 'e', 's', 'T', continuationCharacter };
            expect (CharPointer_UTF8::isValidString (string.data(), 4));
        });

        testCase ("String validation - all unicode characters", [&]
        {
            for (uint32_t c = 0; c < 0x110000; ++c)
            {
                std::array<CharPointer_UTF8::CharType, 4> string = {};
                CharPointer_UTF8 utf8 { string.data() };
                utf8.write ((higgs_wchar) c);
                expect (CharPointer_UTF8::isValidString (string.data(), (int) string.size()) == CharPointer_UTF32::canRepresent ((higgs_wchar) c));
            }
        });

        testCase ("String validation - the top two bits of continuation characters must be '10'", [&]
        {
            const auto isValid = [&] (auto... x)
            {
                const char items[] { static_cast<char> (x)... };
                return CharPointer_UTF8::isValidString (items, (int) std::size (items));
            };

            expect (  isValid (0xc2, 0x80));
            expect (  isValid (0xdf, 0xbf));
            expect (! isValid (0xc2));
            expect (! isValid (0xdf));
            expect (! isValid (0xc2, 0x00));
            expect (! isValid (0xc2, 0xff));
            expect (! isValid (0xdf, 0x00));
            expect (! isValid (0xdf, 0xff));

            expect (  isValid (0xe1, 0x80, 0x80));
            expect (  isValid (0xef, 0xbf, 0xbf));
            expect (! isValid (0xe1, 0x80));
            expect (! isValid (0xef, 0xbf));
            expect (! isValid (0xe1, 0x00, 0x00));
            expect (! isValid (0xe1, 0x80, 0x00));
            expect (! isValid (0xe1, 0x00, 0x80));
            expect (! isValid (0xe1, 0xff, 0xff));
            expect (! isValid (0xe1, 0x80, 0xff));
            expect (! isValid (0xe1, 0xff, 0x80));

            expect (  isValid (0xf0, 0x90, 0x80, 0x80));
            expect (  isValid (0xf4, 0x8f, 0xbf, 0xbf));
            expect (! isValid (0xf0, 0x90));
            expect (! isValid (0xf0, 0x90, 0x80));
            expect (! isValid (0xf0, 0x90, 0x00, 0x00));
            expect (! isValid (0xf0, 0xff, 0xff, 0xff));
        });

        testCase ("Dereferencing a malformed string produces replacement character", [&]
        {
            const auto doDeref = [&] (auto... x)
            {
                const char items[] { static_cast<char> (x)... };
                return CharPointer_UTF8 { items }.getAndAdvance();
            };

            static constexpr higgs_wchar replacement = 0xfffd;

            expectEquals (doDeref (0x80), replacement);
            expectEquals (doDeref (0xbf), replacement);

            expectEquals (doDeref (0xe0, 0xa0, 0x00), replacement);
            expectEquals (doDeref (0xe0, 0x00, 0x80), replacement);

            expectEquals (doDeref (0xf0, 0x90, 0x80, 0x00), replacement);
            expectEquals (doDeref (0xf0, 0x90, 0x00, 0x80), replacement);
            expectEquals (doDeref (0xf0, 0x00, 0x80, 0x80), replacement);

            // overlong sequences
            expectEquals (doDeref (0xc0, 0x80), replacement);
            expectEquals (doDeref (0xe0, 0x80, 0x80), replacement);
            expectEquals (doDeref (0xf0, 0x80, 0x80, 0x80), replacement);

            // beyond upper range
            expectEquals (doDeref (0xf4, 0x90, 0x80, 0x80), replacement);
        });
    }
};


static CharPointer_UTF8Test charPointer_UTF8Test;

} // namespace higgs
