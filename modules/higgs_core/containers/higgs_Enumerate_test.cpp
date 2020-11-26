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

class EnumerateUnitTest : public UnitTest
{
public:
    EnumerateUnitTest() : UnitTest ("Enumerate", UnitTestCategories::containers) {}

    void runTest() override
    {
        beginTest ("enumeration works for bidirectional iterators");
        {
            const std::list<int> elements { 10, 20, 30, 40, 50 };
            std::vector<int> counts;

            for (const auto pair : enumerate (elements))
                counts.push_back ((int) pair.index);

            expect (counts == std::vector<int> { 0, 1, 2, 3, 4 });
        }

        beginTest ("enumeration works for random-access iterators");
        {
            const std::vector<std::string> strings { "a", "bb", "ccc", "dddd", "eeeee" };

            std::vector<int> counts;

            for (const auto [count, element] : enumerate (strings))
                counts.push_back ((int) ((size_t) count + element.size()));

            expect (counts == std::vector<int> { 1, 3, 5, 7, 9 });
        }

        beginTest ("enumeration works for mutable ranges");
        {
            std::vector<std::string> strings { "", "", "", "", "" };

            for (const auto [count, element] : enumerate (strings))
                element = std::to_string (count);

            expect (strings == std::vector<std::string> { "0", "1", "2", "3", "4" });
        }

        beginTest ("iterator can be incremented by more than one");
        {
            std::vector<int> ints (6);

            const auto enumerated = enumerate (ints);

            std::vector<int> counts;

            for (auto b = enumerated.begin(), e = enumerated.end(); b != e; b += 2)
                counts.push_back ((int) (*b).index);

            expect (counts == std::vector<int> { 0, 2, 4 });
        }

        beginTest ("iterator can be started at a non-zero value");
        {
            const std::vector<int> ints (6);

            std::vector<int> counts;

            for (const auto enumerated : enumerate (ints, 5))
                counts.push_back ((int) enumerated.index);

            expect (counts == std::vector<int> { 5, 6, 7, 8, 9, 10 });
        }

        beginTest ("subtracting two EnumerateIterators returns the difference between the base iterators");
        {
            const std::vector<int> ints (6);
            const auto enumerated = enumerate (ints);
            expect ((int) (enumerated.end() - enumerated.begin()) == (int) ints.size());
        }

        beginTest ("EnumerateIterator can be decremented");
        {
            const std::vector<int> ints (5);
            std::vector<int> counts;

            const auto enumerated = enumerate (std::as_const (ints));

            for (auto i = enumerated.end(), b = enumerated.begin(); i != b; --i)
                counts.push_back ((int) (*(i - 1)).index);

            expect (counts == std::vector<int> { -1, -2, -3, -4, -5 });
        }

        beginTest ("EnumerateIterator can be compared");
        {
            const std::vector<int> ints (6);
            const auto enumerated = enumerate (ints);
            expect (enumerated.begin() < enumerated.end());
            expect (enumerated.begin() <= enumerated.end());
            expect (enumerated.end() > enumerated.begin());
            expect (enumerated.end() >= enumerated.begin());
        }
    }
};

static EnumerateUnitTest enumerateUnitTest;

} // namespace higgs
