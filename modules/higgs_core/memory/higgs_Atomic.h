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
namespace AtomicHelpers
{
    template <typename T> struct DiffTypeHelper     { using Type = T; };
    template <typename T> struct DiffTypeHelper<T*> { using Type = std::ptrdiff_t; };
}
/** @endcond */

//==============================================================================
/**
    A simple wrapper around std::atomic.

    @tags{Core}
*/
template <typename Type>
struct Atomic  final
{
    using DiffType = typename AtomicHelpers::DiffTypeHelper<Type>::Type;

    /** Creates a new value, initialised to zero. */
    Atomic() noexcept  : value (Type()) {}

    /** Creates a new value, with a given initial value. */
    Atomic (Type initialValue) noexcept  : value (initialValue) {}

    /** Copies another value (atomically). */
    Atomic (const Atomic& other) noexcept  : value (other.get()) {}

    /** Destructor. */
    ~Atomic() noexcept
    {
       #if __cpp_lib_atomic_is_always_lock_free
        static_assert (std::atomic<Type>::is_always_lock_free,
                       "This class can only be used for lock-free types");
       #endif
    }

    /** Atomically reads and returns the current value. */
    Type get() const noexcept               { return value.load(); }

    /** Atomically sets the current value. */
    void set (Type newValue) noexcept       { value = newValue; }

    /** Atomically sets the current value, returning the value that was replaced. */
    Type exchange (Type newValue) noexcept  { return value.exchange (newValue); }

    /** Atomically compares this value with a target value, and if it is equal, sets
        this to be equal to a new value.

        This operation is the atomic equivalent of doing this:
        @code
        bool compareAndSetBool (Type newValue, Type valueToCompare)
        {
            if (get() == valueToCompare)
            {
                set (newValue);
                return true;
            }

            return false;
        }
        @endcode

        Internally, this method calls std::atomic::compare_exchange_strong with
        memory_order_seq_cst (the strictest std::memory_order).

        @returns true if the comparison was true and the value was replaced; false if
                 the comparison failed and the value was left unchanged.
        @see compareAndSetValue
    */
    bool compareAndSetBool (Type newValue, Type valueToCompare) noexcept
    {
        return value.compare_exchange_strong (valueToCompare, newValue);
    }

    /** Copies another value into this one (atomically). */
    Atomic<Type>& operator= (const Atomic& other) noexcept
    {
        value = other.value.load();
        return *this;
    }

    /** Copies another value into this one (atomically). */
    Atomic<Type>& operator= (Type newValue) noexcept
    {
        value = newValue;
        return *this;
    }

    /** Atomically adds a number to this value, returning the new value. */
    Type operator+= (DiffType amountToAdd) noexcept { return value += amountToAdd; }

    /** Atomically subtracts a number from this value, returning the new value. */
    Type operator-= (DiffType amountToSubtract) noexcept { return value -= amountToSubtract; }

    /** Atomically increments this value, returning the new value. */
    Type operator++() noexcept { return ++value; }

    /** Atomically decrements this value, returning the new value. */
    Type operator--() noexcept { return --value; }

    /** Implements a memory read/write barrier.

        Internally this calls std::atomic_thread_fence with
        memory_order_seq_cst (the strictest std::memory_order).
     */
    void memoryBarrier() noexcept          { atomic_thread_fence (std::memory_order_seq_cst); }

    /** The std::atomic object that this class operates on. */
    std::atomic<Type> value;

    //==============================================================================
    /** @cond */
    [[deprecated ("This method has been deprecated as there is no equivalent method in "
                 "std::atomic. Use compareAndSetBool instead.")]]
    Type compareAndSetValue (Type, Type) noexcept;
    /** @endcond */
};

} // namespace higgs
