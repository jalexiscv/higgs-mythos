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

//==============================================================================
/**
    Macro to enable bitwise operations for scoped enums (enum struct/class).

    To use this, add the line HIGGS_DECLARE_SCOPED_ENUM_BITWISE_OPERATORS (MyEnum)
    after your enum declaration at file scope level.

    e.g. @code

    enum class MyEnum
    {
        one     = 1 << 0,
        two     = 1 << 1,
        three   = 1 << 2
    };

    HIGGS_DECLARE_SCOPED_ENUM_BITWISE_OPERATORS (MyEnum)

    MyEnum e = MyEnum::one | MyEnum::two;

    bool hasTwo = (e & MyEnum::two) != MyEnum{}; // true
    bool hasTwo = hasBitValueSet (e, MyEnum::two); // true

    e = withBitValueCleared (e, MyEnum::two);

    bool hasTwo = hasBitValueSet (e, MyEnum::two); // false

    @endcode
*/
#define HIGGS_DECLARE_SCOPED_ENUM_BITWISE_OPERATORS(EnumType)               \
    static_assert (std::is_enum_v<EnumType>,                               \
                   "HIGGS_DECLARE_SCOPED_ENUM_BITWISE_OPERATORS "           \
                   "should only be used with enum types");                 \
    constexpr auto operator& (EnumType a, EnumType b)                      \
    {                                                                      \
        using base_type = std::underlying_type<EnumType>::type;            \
        return static_cast<EnumType> (base_type (a) & base_type (b));      \
    }                                                                      \
    constexpr auto operator| (EnumType a, EnumType b)                      \
    {                                                                      \
        using base_type = std::underlying_type<EnumType>::type;            \
        return static_cast<EnumType> (base_type (a) | base_type (b));      \
    }                                                                      \
    constexpr auto operator~ (EnumType a)                                  \
    {                                                                      \
        using base_type = std::underlying_type<EnumType>::type;            \
        return static_cast<EnumType> (~base_type (a));                     \
    }                                                                      \
    constexpr auto& operator|= (EnumType& a, EnumType b)                   \
    {                                                                      \
        a = (a | b);                                                       \
        return a;                                                          \
    }                                                                      \
    constexpr auto& operator&= (EnumType& a, EnumType b)                   \
    {                                                                      \
        a = (a & b);                                                       \
        return a;                                                          \
    }


namespace higgs
{

template <typename EnumType, std::enable_if_t<std::is_enum_v<EnumType>, int> = 0>
constexpr bool hasBitValueSet (EnumType enumValue, EnumType valueToLookFor) noexcept
{
    return (enumValue & valueToLookFor) != EnumType{};
}

template <typename EnumType, std::enable_if_t<std::is_enum_v<EnumType>, int> = 0>
constexpr EnumType withBitValueSet (EnumType enumValue, EnumType valueToAdd) noexcept
{
    return enumValue | valueToAdd;
}

template <typename EnumType, std::enable_if_t<std::is_enum_v<EnumType>, int> = 0>
constexpr EnumType withBitValueCleared (EnumType enumValue, EnumType valueToRemove) noexcept
{
    return enumValue & ~valueToRemove;
}
}
