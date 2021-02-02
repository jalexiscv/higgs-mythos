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

/** @cond */
namespace detail
{
    template <typename...>
    using Void = void;

    template <typename, typename = void>
    constexpr auto equalityComparableToNullptr = false;

    template <typename T>
    constexpr auto equalityComparableToNullptr<T, Void<decltype (std::declval<T>() != nullptr)>> = true;

    template <typename> struct Tag {};
} // namespace detail
/** @endcond */

//==============================================================================
/** Some helper methods for checking a callable object before invoking with
    the specified arguments.

    If the object provides a comparison operator for nullptr it will check before
    calling. For other objects it will just invoke the function call operator.

    @tags{Core}
*/
struct NullCheckedInvocation
{
    template <typename Callable, typename... Args>
    static void invoke (Callable&& fn, Args&&... args)
    {
        if constexpr (detail::equalityComparableToNullptr<Callable>)
        {
            HIGGS_BEGIN_IGNORE_WARNINGS_GCC_LIKE ("-Waddress")

            if (fn != nullptr)
                fn (std::forward<Args> (args)...);

            HIGGS_END_IGNORE_WARNINGS_GCC_LIKE
        }
        else
        {
            fn (std::forward<Args> (args)...);
        }
    }

    template <typename... Args>
    static void invoke (std::nullptr_t, Args&&...) {}

    NullCheckedInvocation() = delete;
};

/** Can be used to disable template constructors that would otherwise cause ambiguity with
    compiler-generated copy and move constructors.

    Adapted from https://ericniebler.com/2013/08/07/universal-references-and-the-copy-constructo/
*/
template <typename A, typename B>
using DisableIfSameOrDerived = std::enable_if_t<! std::is_base_of_v<A, std::remove_reference_t<B>>>;

/** Copies an object, sets one of the copy's members to the specified value, and then returns the copy. */
template <typename Object, typename OtherObject, typename Member, typename Other>
[[nodiscard]] Object withMember (Object copy, Member OtherObject::* member, Other&& value)
{
    copy.*member = std::forward<Other> (value);
    return copy;
}

/** @cond */
namespace detail
{
template <typename Functor, typename Return, typename... Args>
static constexpr auto toFnPtr (Functor functor, Return (Functor::*) (Args...) const)
{
    return static_cast<Return (*) (Args...)> (functor);
}
} // namespace detail
/** @endcond */

/** Converts a captureless lambda to its equivalent function pointer type. */
template <typename Functor>
static constexpr auto toFnPtr (Functor functor) { return detail::toFnPtr (functor, &Functor::operator()); }

} // namespace higgs
