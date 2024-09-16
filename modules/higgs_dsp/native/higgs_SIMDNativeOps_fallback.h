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

/** A template specialisation to find corresponding mask type for primitives. */
namespace SIMDInternal
{
    template <typename Primitive> struct MaskTypeFor        { using type = Primitive; };
    template <> struct MaskTypeFor <float>                  { using type = uint32_t; };
    template <> struct MaskTypeFor <double>                 { using type = uint64_t; };
    template <> struct MaskTypeFor <char>                   { using type = uint8_t; };
    template <> struct MaskTypeFor <int8_t>                 { using type = uint8_t; };
    template <> struct MaskTypeFor <int16_t>                { using type = uint16_t; };
    template <> struct MaskTypeFor <int32_t>                { using type = uint32_t; };
    template <> struct MaskTypeFor <int64_t>                { using type = uint64_t; };
    template <> struct MaskTypeFor <std::complex<float>>    { using type = uint32_t; };
    template <> struct MaskTypeFor <std::complex<double>>   { using type = uint64_t; };

    template <typename Primitive> using MaskType = typename MaskTypeFor<Primitive>::type;

    template <typename Primitive> struct PrimitiveType                           { using type = std::remove_cv_t<Primitive>; };
    template <typename Primitive> struct PrimitiveType<std::complex<Primitive>>  { using type = std::remove_cv_t<Primitive>; };

    template <int n>    struct Log2Helper    { enum { value = Log2Helper<n/2>::value + 1 }; };
    template <>         struct Log2Helper<1> { enum { value = 0 }; };
}

/**
    Useful fallback routines to use if the native SIMD op is not supported. You
    should never need to use this directly. Use higgs_SIMDRegister instead.

    @tags{DSP}
*/
template <typename ScalarType, typename vSIMDType>
struct SIMDFallbackOps
{
    static constexpr size_t n    =  sizeof (vSIMDType) / sizeof (ScalarType);
    static constexpr size_t mask = (sizeof (vSIMDType) / sizeof (ScalarType)) - 1;
    static constexpr size_t bits = SIMDInternal::Log2Helper<(int) n>::value;

    // helper types
    using MaskType = SIMDInternal::MaskType<ScalarType>;
    union UnionType     { vSIMDType v; ScalarType s[n]; };
    union UnionMaskType { vSIMDType v; MaskType   m[n]; };


    // fallback methods
    static forcedinline vSIMDType add (vSIMDType a, vSIMDType b) noexcept        { return apply<ScalarAdd> (a, b); }
    static forcedinline vSIMDType sub (vSIMDType a, vSIMDType b) noexcept        { return apply<ScalarSub> (a, b); }
    static forcedinline vSIMDType mul (vSIMDType a, vSIMDType b) noexcept        { return apply<ScalarMul> (a, b); }
    static forcedinline vSIMDType bit_and (vSIMDType a, vSIMDType b) noexcept    { return bitapply<ScalarAnd> (a, b); }
    static forcedinline vSIMDType bit_or  (vSIMDType a, vSIMDType b) noexcept    { return bitapply<ScalarOr > (a, b); }
    static forcedinline vSIMDType bit_xor (vSIMDType a, vSIMDType b) noexcept    { return bitapply<ScalarXor> (a, b); }
    static forcedinline vSIMDType bit_notand (vSIMDType a, vSIMDType b) noexcept { return bitapply<ScalarNot> (a, b); }

    static forcedinline vSIMDType min (vSIMDType a, vSIMDType b) noexcept                { return apply<ScalarMin> (a, b); }
    static forcedinline vSIMDType max (vSIMDType a, vSIMDType b) noexcept                { return apply<ScalarMax> (a, b); }
    static forcedinline vSIMDType equal (vSIMDType a, vSIMDType b) noexcept              { return cmp<ScalarEq > (a, b); }
    static forcedinline vSIMDType notEqual (vSIMDType a, vSIMDType b) noexcept           { return cmp<ScalarNeq> (a, b); }
    static forcedinline vSIMDType greaterThan (vSIMDType a, vSIMDType b) noexcept        { return cmp<ScalarGt > (a, b); }
    static forcedinline vSIMDType greaterThanOrEqual (vSIMDType a, vSIMDType b) noexcept { return cmp<ScalarGeq> (a, b); }

    static forcedinline ScalarType get (vSIMDType v, size_t i) noexcept
    {
        UnionType u {v};
        return u.s[i];
    }

    static forcedinline vSIMDType set (vSIMDType v, size_t i, ScalarType s) noexcept
    {
        UnionType u {v};

        u.s[i] = s;
        return u.v;
    }

    static forcedinline vSIMDType bit_not (vSIMDType av) noexcept
    {
        UnionMaskType a {av};

        for (size_t i = 0; i < n; ++i)
            a.m[i] = ~a.m[i];

        return a.v;
    }

    static forcedinline ScalarType sum (vSIMDType av) noexcept
    {
        UnionType a {av};
        auto retval = static_cast<ScalarType> (0);

        for (size_t i = 0; i < n; ++i)
            retval = static_cast<ScalarType> (retval + a.s[i]);

        return retval;
    }

    static forcedinline vSIMDType truncate (vSIMDType av) noexcept
    {
        UnionType a {av};

        for (size_t i = 0; i < n; ++i)
            a.s[i] = static_cast<ScalarType> (static_cast<int> (a.s[i]));

        return a.v;
    }

    static forcedinline vSIMDType multiplyAdd (vSIMDType av, vSIMDType bv, vSIMDType cv) noexcept
    {
        UnionType a {av}, b {bv}, c {cv};

        for (size_t i = 0; i < n; ++i)
            a.s[i] += b.s[i] * c.s[i];

        return a.v;
    }

    //==============================================================================
    static forcedinline bool allEqual (vSIMDType av, vSIMDType bv) noexcept
    {
        UnionType a {av}, b {bv};

        for (size_t i = 0; i < n; ++i)
            if (! exactlyEqual (a.s[i], b.s[i]))
                return false;

        return true;
    }

    //==============================================================================
    static forcedinline vSIMDType cmplxmul (vSIMDType av, vSIMDType bv) noexcept
    {
        UnionType a {av}, b {bv}, r;

        const int m = n >> 1;
        for (int i = 0; i < m; ++i)
        {
            std::complex<ScalarType> result
                  = std::complex<ScalarType> (a.s[i<<1], a.s[(i<<1)|1])
                  * std::complex<ScalarType> (b.s[i<<1], b.s[(i<<1)|1]);

            r.s[i<<1]     = result.real();
            r.s[(i<<1)|1] = result.imag();
        }

        return r.v;
    }

    struct ScalarAdd { static forcedinline ScalarType   op (ScalarType a, ScalarType b)   noexcept { return a + b; } };
    struct ScalarSub { static forcedinline ScalarType   op (ScalarType a, ScalarType b)   noexcept { return a - b; } };
    struct ScalarMul { static forcedinline ScalarType   op (ScalarType a, ScalarType b)   noexcept { return a * b; } };
    struct ScalarMin { static forcedinline ScalarType   op (ScalarType a, ScalarType b)   noexcept { return jmin (a, b); } };
    struct ScalarMax { static forcedinline ScalarType   op (ScalarType a, ScalarType b)   noexcept { return jmax (a, b); } };
    struct ScalarAnd { static forcedinline MaskType     op (MaskType a,   MaskType b)     noexcept { return a & b; } };
    struct ScalarOr  { static forcedinline MaskType     op (MaskType a,   MaskType b)     noexcept { return a | b; } };
    struct ScalarXor { static forcedinline MaskType     op (MaskType a,   MaskType b)     noexcept { return a ^ b; } };
    struct ScalarNot { static forcedinline MaskType     op (MaskType a,   MaskType b)     noexcept { return (~a) & b; } };
    struct ScalarEq  { static forcedinline bool         op (ScalarType a, ScalarType b)   noexcept { return exactlyEqual (a, b); } };
    struct ScalarNeq { static forcedinline bool         op (ScalarType a, ScalarType b)   noexcept { return ! exactlyEqual (a, b); } };
    struct ScalarGt  { static forcedinline bool         op (ScalarType a, ScalarType b)   noexcept { return (a >  b); } };
    struct ScalarGeq { static forcedinline bool         op (ScalarType a, ScalarType b)   noexcept { return (a >= b); } };

    // generic apply routines for operations above
    template <typename Op>
    static forcedinline vSIMDType apply (vSIMDType av, vSIMDType bv) noexcept
    {
        UnionType a {av}, b {bv};

        for (size_t i = 0; i < n; ++i)
            a.s[i] = Op::op (a.s[i], b.s[i]);

        return a.v;
    }

    template <typename Op>
    static forcedinline vSIMDType cmp (vSIMDType av, vSIMDType bv) noexcept
    {
        UnionType a {av}, b {bv};
        UnionMaskType r;

        for (size_t i = 0; i < n; ++i)
            r.m[i] = Op::op (a.s[i], b.s[i]) ? static_cast<MaskType> (-1) : static_cast<MaskType> (0);

        return r.v;
    }

    template <typename Op>
    static forcedinline vSIMDType bitapply (vSIMDType av, vSIMDType bv) noexcept
    {
        UnionMaskType a {av}, b {bv};

        for (size_t i = 0; i < n; ++i)
            a.m[i] = Op::op (a.m[i], b.m[i]);

        return a.v;
    }

    static forcedinline vSIMDType expand (ScalarType s) noexcept
    {
        UnionType r;

        for (size_t i = 0; i < n; ++i)
            r.s[i] = s;

        return r.v;
    }

    static forcedinline vSIMDType load (const ScalarType* a) noexcept
    {
        UnionType r;

        for (size_t i = 0; i < n; ++i)
            r.s[i] = a[i];

        return r.v;
    }

    static forcedinline void store (vSIMDType av, ScalarType* dest) noexcept
    {
        UnionType a {av};

        for (size_t i = 0; i < n; ++i)
            dest[i] = a.s[i];
    }

    template <unsigned int shuffle_idx>
    static forcedinline vSIMDType shuffle (vSIMDType av) noexcept
    {
        UnionType a {av}, r;

        // the compiler will unroll this loop and the index can
        // be computed at compile-time, so this will be super fast
        for (size_t i = 0; i < n; ++i)
            r.s[i] = a.s[(shuffle_idx >> (bits * i)) & mask];

        return r.v;
    }
};

} // namespace higgs::dsp
