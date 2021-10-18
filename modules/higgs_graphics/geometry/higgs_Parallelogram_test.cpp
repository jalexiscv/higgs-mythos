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

class ParallelogramTest : public UnitTest
{
public:
    ParallelogramTest() : UnitTest ("Parallelogram", UnitTestCategories::graphics) {}

    void runTest() override
    {
        beginTest ("isEmpty");
        {
            expect (! Parallelogram (Rectangle<int> (10, 10, 20, 20)).isEmpty());
            expect (Parallelogram (Rectangle<int> (10, 10, 0, 20)).isEmpty());
            expect (Parallelogram (Rectangle<int> (10, 10, 20, 0)).isEmpty());

            expect (! Parallelogram (Point<int> (0, 0), Point<int> (10, 10), Point<int> (20, 0)).isEmpty());
            expect (Parallelogram (Point<int> (0, 0), Point<int> (0, 0), Point<int> (20, 0)).isEmpty());
            expect (Parallelogram (Point<int> (0, 0), Point<int> (10, 10), Point<int> (10, 10)).isEmpty());
            expect (Parallelogram (Point<int> (20, 0), Point<int> (10, 10), Point<int> (20, 0)).isEmpty());
        }

        beginTest ("operators");
        {
            Parallelogram p (Rectangle<int> (10, 10, 20, 20));
            p += Point<int> (5, 10);
            expect (p.topLeft == Point<int> (15, 20));
            expect (p.topRight == Point<int> (35, 20));
            expect (p.bottomLeft == Point<int> (15, 40));

            p -= Point<int> (10, 5);
            expect (p.topLeft == Point<int> (5, 15));
            expect (p.topRight == Point<int> (25, 15));
            expect (p.bottomLeft == Point<int> (5, 35));
        }
    }
};

static ParallelogramTest parallelogramTest;

} // namespace higgs
