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

class ValueTreePropertyWithDefaultTests final : public UnitTest
{
public:
    ValueTreePropertyWithDefaultTests()
        : UnitTest ("ValueTreePropertyWithDefault", UnitTestCategories::values)
    {}

    void runTest() override
    {
        beginTest ("default constructor");
        {
            ValueTreePropertyWithDefault value;
            expect (value.isUsingDefault());
            expect (value.get() == var());
        }

        beginTest ("missing property");
        {
            ValueTree t ("root");
            ValueTreePropertyWithDefault value (t, "testKey", nullptr, "default");

            expect (value.isUsingDefault());
            expectEquals (value.get().toString(), String ("default"));
        }

        beginTest ("non-empty property");
        {
            ValueTree t ("root");
            t.setProperty ("testKey", "non-default", nullptr);

            ValueTreePropertyWithDefault value (t, "testKey", nullptr, "default");

            expect (! value.isUsingDefault());
            expectEquals (value.get().toString(), String ("non-default"));
        }

        beginTest ("set default");
        {
            ValueTree t ("root");

            ValueTreePropertyWithDefault value (t, "testkey", nullptr);
            value.setDefault ("default");

            expect (value.isUsingDefault());
            expectEquals (value.get().toString(), String ("default"));
        }

        beginTest ("set value");
        {
            ValueTree t ("root");
            t.setProperty ("testkey", "testvalue", nullptr);

            ValueTreePropertyWithDefault value (t, "testkey", nullptr, "default");
            value = "newvalue";

            expect (! value.isUsingDefault());
            expectEquals (t["testkey"].toString(), String ("newvalue"));

            value.resetToDefault();

            expect (value.isUsingDefault());
            expect (t["testkey"] == var());
        }
    }
};

static ValueTreePropertyWithDefaultTests valueTreePropertyWithDefaultTests;

} // namespace higgs
