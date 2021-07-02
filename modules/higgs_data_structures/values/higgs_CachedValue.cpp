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

#if HIGGS_UNIT_TESTS

class CachedValueTests final : public UnitTest
{
public:
    CachedValueTests()
        : UnitTest ("CachedValues", UnitTestCategories::values)
    {}

    void runTest() override
    {
        beginTest ("default constructor");
        {
            CachedValue<String> cv;
            expect (cv.isUsingDefault());
            expect (cv.get() == String());
        }

        beginTest ("without default value");
        {
            ValueTree t ("root");
            t.setProperty ("testkey", "testvalue", nullptr);

            CachedValue<String> cv (t, "testkey", nullptr);

            expect (! cv.isUsingDefault());
            expect (cv.get() == "testvalue");

            cv.resetToDefault();

            expect (cv.isUsingDefault());
            expect (cv.get() == String());
        }

        beginTest ("with default value");
        {
            ValueTree t ("root");
            t.setProperty ("testkey", "testvalue", nullptr);

            CachedValue<String> cv (t, "testkey", nullptr, "defaultvalue");

            expect (! cv.isUsingDefault());
            expect (cv.get() == "testvalue");

            cv.resetToDefault();

            expect (cv.isUsingDefault());
            expect (cv.get() == "defaultvalue");
        }

        beginTest ("with default value (int)");
        {
            ValueTree t ("root");
            t.setProperty ("testkey", 23, nullptr);

            CachedValue<int> cv (t, "testkey", nullptr, 34);

            expect (! cv.isUsingDefault());
            expect (cv == 23);
            expectEquals (cv.get(), 23);

            cv.resetToDefault();

            expect (cv.isUsingDefault());
            expect (cv == 34);
        }

        beginTest ("with void value");
        {
            ValueTree t ("root");
            t.setProperty ("testkey", var(), nullptr);

            CachedValue<String> cv (t, "testkey", nullptr, "defaultvalue");

            expect (! cv.isUsingDefault());
            expect (cv == "");
            expectEquals (cv.get(), String());
        }

        beginTest ("with non-existent value");
        {
            ValueTree t ("root");

            CachedValue<String> cv (t, "testkey", nullptr, "defaultvalue");

            expect (cv.isUsingDefault());
            expect (cv == "defaultvalue");
            expect (cv.get() == "defaultvalue");
        }

        beginTest ("with value changing");
        {
            ValueTree t ("root");
            t.setProperty ("testkey", "oldvalue", nullptr);

            CachedValue<String> cv (t, "testkey", nullptr, "defaultvalue");
            expect (cv == "oldvalue");

            t.setProperty ("testkey", "newvalue", nullptr);
            expect (cv != "oldvalue");
            expect (cv == "newvalue");
        }

        beginTest ("set value");
        {
            ValueTree t ("root");
            t.setProperty ("testkey", 23, nullptr);

            CachedValue<int> cv (t, "testkey", nullptr, 45);
            cv = 34;

            expectEquals ((int) t["testkey"], 34);

            cv.resetToDefault();
            expect (cv == 45);
            expectEquals (cv.get(), 45);

            expect (t["testkey"] == var());
        }
    }
};

static CachedValueTests cachedValueTests;

#endif

} // namespace higgs
