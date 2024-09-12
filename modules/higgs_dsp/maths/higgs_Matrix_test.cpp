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

namespace higgs::dsp
{

struct LinearAlgebraUnitTest final : public UnitTest
{
    LinearAlgebraUnitTest()
        : UnitTest ("Linear Algebra UnitTests", UnitTestCategories::dsp)
    {}

    struct AdditionTest
    {
        template <typename ElementType>
        static void run (LinearAlgebraUnitTest& u)
        {
            const ElementType data1[] = { 1,  2, 3,  4,  5,  6,  7,  8 };
            const ElementType data2[] = { 1, -1, 3, -1,  5, -1,  7, -1 };
            const ElementType data3[] = { 2,  1, 6,  3, 10,  5, 14,  7 };

            Matrix<ElementType> mat1 (2, 4, data1);
            Matrix<ElementType> mat2 (2, 4, data2);
            Matrix<ElementType> mat3 (2, 4, data3);

            u.expect ((mat1 + mat2) == mat3);
        }
    };

    struct DifferenceTest
    {
        template <typename ElementType>
        static void run (LinearAlgebraUnitTest& u)
        {
            const ElementType data1[] = { 1,  2, 3,  4, 5,  6, 7,  8 };
            const ElementType data2[] = { 1, -1, 3, -1, 5, -1, 7, -1 };
            const ElementType data3[] = { 0,  3, 0,  5, 0,  7, 0,  9 };

            Matrix<ElementType> mat1 (2, 4, data1);
            Matrix<ElementType> mat2 (2, 4, data2);
            Matrix<ElementType> mat3 (2, 4, data3);

            u.expect ((mat1 - mat2) == mat3);
        }
    };

    struct ScalarMultiplicationTest
    {
        template <typename ElementType>
        static void run (LinearAlgebraUnitTest& u)
        {
            const ElementType data1[] = { 1,  2, 3,  4, 5,  6, 7,  8 };
            const ElementType scalar = 2.0;
            const ElementType data2[] = { 2, 4, 6, 8, 10, 12, 14, 16 };

            Matrix<ElementType> x (2, 4, data1);
            Matrix<ElementType> expected (2, 4, data2);

            u.expect ((x * scalar) == expected);
        }
    };

    struct HadamardProductTest
    {
        template <typename ElementType>
        static void run (LinearAlgebraUnitTest& u)
        {
            const ElementType data1[] = { 1,  2, 3,  4,  5,  6,  7,  8 };
            const ElementType data2[] = { 1, -1, 3, -1,  5, -1,  7, -1 };
            const ElementType data3[] = { 1, -2, 9, -4, 25, -6, 49, -8 };

            Matrix<ElementType> mat1 (2, 4, data1);
            Matrix<ElementType> mat2 (2, 4, data2);
            Matrix<ElementType> mat3 (2, 4, data3);

            u.expect (Matrix<ElementType>::hadarmard (mat1, mat2) == mat3);
        }
    };

    struct MultiplicationTest
    {
        template <typename ElementType>
        static void run (LinearAlgebraUnitTest& u)
        {
            const ElementType data1[] = { 1,  2, 3,  4,  5,  6,  7,  8 };
            const ElementType data2[] = { 1, -1, 3, -1,  5, -1,  7, -1 };
            const ElementType data3[] = { 50, -10, 114, -26 };

            Matrix<ElementType> mat1 (2, 4, data1);
            Matrix<ElementType> mat2 (4, 2, data2);
            Matrix<ElementType> mat3 (2, 2, data3);

            u.expect ((mat1 * mat2) == mat3);
        }
    };

    struct IdentityMatrixTest
    {
        template <typename ElementType>
        static void run (LinearAlgebraUnitTest& u)
        {
            const ElementType data1[] = { 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1};
            u.expect (Matrix<ElementType>::identity (4) == Matrix<ElementType> (4, 4, data1));
        }
    };

    struct SolvingTest
    {
        template <typename ElementType>
        static void run (LinearAlgebraUnitTest& u)
        {
            const ElementType data1[] = { 1, -1, 2, -2 };
            const ElementType data2[] = { -1, 0, -1, -7 };
            const ElementType data3[] = { 1, 4, 2, 1, -1, 1, 4, 3, -2, -1, 1, 1, -1, 0, 1, 4 };

            Matrix<ElementType> X (4, 1, data1);
            Matrix<ElementType> B (4, 1, data2);
            Matrix<ElementType> A (4, 4, data3);

            u.expect (A.solve (B));
            u.expect (Matrix<ElementType>::compare (X, B, (ElementType) 1e-4));
        }
    };

    template <class TheTest>
    void runTestForAllTypes (const char* unitTestName)
    {
        beginTest (unitTestName);

        TheTest::template run<float> (*this);
        TheTest::template run<double> (*this);
    }

    void runTest() override
    {
        runTestForAllTypes<AdditionTest> ("AdditionTest");
        runTestForAllTypes<DifferenceTest> ("DifferenceTest");
        runTestForAllTypes<ScalarMultiplicationTest> ("ScalarMultiplication");
        runTestForAllTypes<HadamardProductTest> ("HadamardProductTest");
        runTestForAllTypes<MultiplicationTest> ("MultiplicationTest");
        runTestForAllTypes<IdentityMatrixTest> ("IdentityMatrixTest");
        runTestForAllTypes<SolvingTest> ("SolvingTest");
    }
};

static LinearAlgebraUnitTest linearAlgebraUnitTest;

} // namespace higgs::dsp
