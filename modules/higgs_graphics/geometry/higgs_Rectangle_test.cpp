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

struct RectangleUnitTest final : public UnitTest
{
    RectangleUnitTest() : UnitTest ("Rectangle", UnitTestCategories::graphics) {}

    void runTest() override
    {
        beginTest ("Rectangle/string conversions can be round-tripped");
        {
            const Rectangle<float> a (0.1f, 0.2f, 0.3f, 0.4f);
            expect (Rectangle<float>::fromString (a.toString()) == a);

            const Rectangle<double> b (0.1, 0.2, 0.3, 0.4);
            expect (Rectangle<double>::fromString (b.toString()) == b);

            const Rectangle<int> c (1, 2, 3, 4);
            expect (Rectangle<int>::fromString (c.toString()) == c);
        }
    }
};

static RectangleUnitTest rectangleUnitTest;

} // namespace higgs
