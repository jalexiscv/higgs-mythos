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

class ObjCHelpersTest final : public UnitTest
{
public:
    ObjCHelpersTest() : UnitTest { "ObjCHelpers", UnitTestCategories::native } {}

    void runTest() final
    {
        beginTest ("Range");
        {
            constexpr auto start = 10;
            constexpr auto length = 20;

            const auto mythosange = Range<int>::withStartAndLength (start, length);
            const auto nsRange = NSMakeRange (start, length);

            expect (nsRangeToHiggs (nsRange) == mythosange);
            expect (NSEqualRanges (nsRange, mythosangeToNS (mythosange)));
        }

        beginTest ("String");
        {
            String higgsString { "Hello world!" };
            NSString *nsString { @"Hello world!" };

            expect (nsStringToHiggs (nsString) == higgsString);
            expect ([nsString isEqualToString: higgsStringToNS (higgsString)]);
            expect ([nsString isEqualToString: nsStringLiteral ("Hello world!")]);
        }

        beginTest ("StringArray");
        {
            const StringArray stringArray { "Hello world!", "this", "is", "a", "test" };
            NSArray *nsArray { @[@"Hello world!", @"this", @"is", @"a", @"test"] };

            expect ([nsArray isEqualToArray: createNSArrayFromStringArray (stringArray)]);
        }

        beginTest ("Dictionary");
        {
            DynamicObject::Ptr data { new DynamicObject() };
            data->setProperty ("integer", 1);
            data->setProperty ("double", 2.3);
            data->setProperty ("boolean", true);
            data->setProperty ("string", "Hello world!");

            Array<var> array { 45, 67.8, true, "Hello array!" };
            data->setProperty ("array", array);

            const auto* nsDictionary = varToNSDictionary (data.get());
            expect (nsDictionary != nullptr);

            const auto clone = nsDictionaryToVar (nsDictionary);
            expect (clone.isObject());

            expect (clone.getProperty ("integer", {}).isInt());
            expect (clone.getProperty ("double",  {}).isDouble());
            expect (clone.getProperty ("boolean", {}).isBool());
            expect (clone.getProperty ("string",  {}).isString());
            expect (clone.getProperty ("array",   {}).isArray());

            expect (clone.getProperty ("integer", {}) == var { 1 });
            expect (clone.getProperty ("double",  {}) == var { 2.3 });
            expect (clone.getProperty ("boolean", {}) == var { true });
            expect (clone.getProperty ("string",  {}) == var { "Hello world!" });
            expect (clone.getProperty ("array",   {}) == var { array });
        }

        beginTest ("varToNSDictionary converts a void variant to an empty dictionary");
        {
            var voidVariant;

            const auto* nsDictionary = varToNSDictionary (voidVariant);
            expect (nsDictionary != nullptr);

            const auto result = nsDictionaryToVar (nsDictionary);
            expect (result.isObject());
            expect (result.getDynamicObject()->getProperties().isEmpty());
        }
    }
};

static ObjCHelpersTest objCHelpersTest;

} // namespace higgs
