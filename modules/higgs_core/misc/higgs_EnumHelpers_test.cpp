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

namespace detail
{
enum class TestEnum
{
    one    = 1 << 0,
    four   = 1 << 1,
    other  = 1 << 2
};

HIGGS_DECLARE_SCOPED_ENUM_BITWISE_OPERATORS (TestEnum)
}

class EnumHelperTest final : public UnitTest
{
public:
    EnumHelperTest() : UnitTest ("EnumHelpers", UnitTestCategories::containers) {}

    void runTest() override
    {
        using detail::TestEnum;

        TestEnum e = {};

        beginTest ("Default initialised enum is 'none'");
        {
            expect (e == TestEnum{});
            expect (! hasBitValueSet (e, TestEnum{}));
        }

        beginTest ("withBitValueSet sets correct bit on empty enum");
        {
            e = withBitValueSet (e, TestEnum::other);
            expect (e == TestEnum::other);
            expect (hasBitValueSet (e, TestEnum::other));
        }

        beginTest ("withBitValueSet sets correct bit on non-empty enum");
        {
            e = withBitValueSet (e, TestEnum::one);
            expect (hasBitValueSet (e, TestEnum::one));
        }

        beginTest ("withBitValueCleared clears correct bit");
        {
            e = withBitValueCleared (e, TestEnum::one);
            expect (e != TestEnum::one);
            expect (hasBitValueSet (e, TestEnum::other));
            expect (! hasBitValueSet (e, TestEnum::one));
        }

        beginTest ("operators work as expected");
        {
            e = TestEnum::one;
            expect ((e & TestEnum::one) != TestEnum{});
            e |= TestEnum::other;
            expect ((e & TestEnum::other) != TestEnum{});

            e &= ~TestEnum::one;
            expect ((e & TestEnum::one) == TestEnum{});
            expect ((e & TestEnum::other) != TestEnum{});
        }
    }
};

static EnumHelperTest enumHelperTest;

} // namespace higgs
