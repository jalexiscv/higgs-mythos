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

namespace FloatVectorHelpers
{
    #define HIGGS_INCREMENT_SRC_DEST         dest += (16 / sizeof (*dest)); src += (16 / sizeof (*dest));
    #define HIGGS_INCREMENT_SRC1_SRC2_DEST   dest += (16 / sizeof (*dest)); src1 += (16 / sizeof (*dest)); src2 += (16 / sizeof (*dest));
    #define HIGGS_INCREMENT_DEST             dest += (16 / sizeof (*dest));

   #if HIGGS_USE_SSE_INTRINSICS
    static bool isAligned (const void* p) noexcept
    {
        return (((pointer_sized_int) p) & 15) == 0;
    }

    struct BasicOps32
    {
        using Type = float;
        using ParallelType = __m128;
        using IntegerType  = __m128;
        enum { numParallel = 4 };

        // Integer and parallel types are the same for SSE. On neon they have different types
        static forcedinline IntegerType toint (ParallelType v) noexcept                 { return v; }
        static forcedinline ParallelType toflt (IntegerType v) noexcept                 { return v; }

        static forcedinline ParallelType load1 (Type v) noexcept                        { return _mm_load1_ps (&v); }
        static forcedinline ParallelType loadA (const Type* v) noexcept                 { return _mm_load_ps (v); }
        static forcedinline ParallelType loadU (const Type* v) noexcept                 { return _mm_loadu_ps (v); }
        static forcedinline void storeA (Type* dest, ParallelType a) noexcept           { _mm_store_ps (dest, a); }
        static forcedinline void storeU (Type* dest, ParallelType a) noexcept           { _mm_storeu_ps (dest, a); }

        static forcedinline ParallelType add (ParallelType a, ParallelType b) noexcept  { return _mm_add_ps (a, b); }
        static forcedinline ParallelType sub (ParallelType a, ParallelType b) noexcept  { return _mm_sub_ps (a, b); }
        static forcedinline ParallelType mul (ParallelType a, ParallelType b) noexcept  { return _mm_mul_ps (a, b); }
        static forcedinline ParallelType max (ParallelType a, ParallelType b) noexcept  { return _mm_max_ps (a, b); }
        static forcedinline ParallelType min (ParallelType a, ParallelType b) noexcept  { return _mm_min_ps (a, b); }

        static forcedinline ParallelType bit_and (ParallelType a, ParallelType b) noexcept  { return _mm_and_ps (a, b); }
        static forcedinline ParallelType bit_not (ParallelType a, ParallelType b) noexcept  { return _mm_andnot_ps (a, b); }
        static forcedinline ParallelType bit_or  (ParallelType a, ParallelType b) noexcept  { return _mm_or_ps (a, b); }
        static forcedinline ParallelType bit_xor (ParallelType a, ParallelType b) noexcept  { return _mm_xor_ps (a, b); }

        static forcedinline Type max (ParallelType a) noexcept { Type v[numParallel]; storeU (v, a); return jmax (v[0], v[1], v[2], v[3]); }
        static forcedinline Type min (ParallelType a) noexcept { Type v[numParallel]; storeU (v, a); return jmin (v[0], v[1], v[2], v[3]); }
    };

    struct BasicOps64
    {
        using Type = double;
        using ParallelType = __m128d;
        using IntegerType  = __m128d;
        enum { numParallel = 2 };

        // Integer and parallel types are the same for SSE. On neon they have different types
        static forcedinline IntegerType toint (ParallelType v) noexcept                 { return v; }
        static forcedinline ParallelType toflt (IntegerType v) noexcept                 { return v; }

        static forcedinline ParallelType load1 (Type v) noexcept                        { return _mm_load1_pd (&v); }
        static forcedinline ParallelType loadA (const Type* v) noexcept                 { return _mm_load_pd (v); }
        static forcedinline ParallelType loadU (const Type* v) noexcept                 { return _mm_loadu_pd (v); }
        static forcedinline void storeA (Type* dest, ParallelType a) noexcept           { _mm_store_pd (dest, a); }
        static forcedinline void storeU (Type* dest, ParallelType a) noexcept           { _mm_storeu_pd (dest, a); }

        static forcedinline ParallelType add (ParallelType a, ParallelType b) noexcept  { return _mm_add_pd (a, b); }
        static forcedinline ParallelType sub (ParallelType a, ParallelType b) noexcept  { return _mm_sub_pd (a, b); }
        static forcedinline ParallelType mul (ParallelType a, ParallelType b) noexcept  { return _mm_mul_pd (a, b); }
        static forcedinline ParallelType max (ParallelType a, ParallelType b) noexcept  { return _mm_max_pd (a, b); }
        static forcedinline ParallelType min (ParallelType a, ParallelType b) noexcept  { return _mm_min_pd (a, b); }

        static forcedinline ParallelType bit_and (ParallelType a, ParallelType b) noexcept  { return _mm_and_pd (a, b); }
        static forcedinline ParallelType bit_not (ParallelType a, ParallelType b) noexcept  { return _mm_andnot_pd (a, b); }
        static forcedinline ParallelType bit_or  (ParallelType a, ParallelType b) noexcept  { return _mm_or_pd (a, b); }
        static forcedinline ParallelType bit_xor (ParallelType a, ParallelType b) noexcept  { return _mm_xor_pd (a, b); }

        static forcedinline Type max (ParallelType a) noexcept  { Type v[numParallel]; storeU (v, a); return jmax (v[0], v[1]); }
        static forcedinline Type min (ParallelType a) noexcept  { Type v[numParallel]; storeU (v, a); return jmin (v[0], v[1]); }
    };



    #define HIGGS_BEGIN_VEC_OP \
        using Mode = FloatVectorHelpers::ModeType<sizeof(*dest)>::Mode; \
        { \
            const auto numLongOps = num / Mode::numParallel;

    #define HIGGS_FINISH_VEC_OP(normalOp) \
            num &= (Mode::numParallel - 1); \
            if (num == 0) return; \
        } \
        for (auto i = (decltype (num)) 0; i < num; ++i) normalOp;

    #define HIGGS_PERFORM_VEC_OP_DEST(normalOp, vecOp, locals, setupOp) \
        HIGGS_BEGIN_VEC_OP \
        setupOp \
        if (FloatVectorHelpers::isAligned (dest))   HIGGS_VEC_LOOP (vecOp, dummy, Mode::loadA, Mode::storeA, locals, HIGGS_INCREMENT_DEST) \
        else                                        HIGGS_VEC_LOOP (vecOp, dummy, Mode::loadU, Mode::storeU, locals, HIGGS_INCREMENT_DEST) \
        HIGGS_FINISH_VEC_OP (normalOp)

    #define HIGGS_PERFORM_VEC_OP_SRC_DEST(normalOp, vecOp, locals, increment, setupOp) \
        HIGGS_BEGIN_VEC_OP \
        setupOp \
        if (FloatVectorHelpers::isAligned (dest)) \
        { \
            if (FloatVectorHelpers::isAligned (src)) HIGGS_VEC_LOOP (vecOp, Mode::loadA, Mode::loadA, Mode::storeA, locals, increment) \
            else                                     HIGGS_VEC_LOOP (vecOp, Mode::loadU, Mode::loadA, Mode::storeA, locals, increment) \
        }\
        else \
        { \
            if (FloatVectorHelpers::isAligned (src)) HIGGS_VEC_LOOP (vecOp, Mode::loadA, Mode::loadU, Mode::storeU, locals, increment) \
            else                                     HIGGS_VEC_LOOP (vecOp, Mode::loadU, Mode::loadU, Mode::storeU, locals, increment) \
        } \
        HIGGS_FINISH_VEC_OP (normalOp)

    #define HIGGS_PERFORM_VEC_OP_SRC1_SRC2_DEST(normalOp, vecOp, locals, increment, setupOp) \
        HIGGS_BEGIN_VEC_OP \
        setupOp \
        if (FloatVectorHelpers::isAligned (dest)) \
        { \
            if (FloatVectorHelpers::isAligned (src1)) \
            { \
                if (FloatVectorHelpers::isAligned (src2))   HIGGS_VEC_LOOP_TWO_SOURCES (vecOp, Mode::loadA, Mode::loadA, Mode::storeA, locals, increment) \
                else                                        HIGGS_VEC_LOOP_TWO_SOURCES (vecOp, Mode::loadA, Mode::loadU, Mode::storeA, locals, increment) \
            } \
            else \
            { \
                if (FloatVectorHelpers::isAligned (src2))   HIGGS_VEC_LOOP_TWO_SOURCES (vecOp, Mode::loadU, Mode::loadA, Mode::storeA, locals, increment) \
                else                                        HIGGS_VEC_LOOP_TWO_SOURCES (vecOp, Mode::loadU, Mode::loadU, Mode::storeA, locals, increment) \
            } \
        } \
        else \
        { \
            if (FloatVectorHelpers::isAligned (src1)) \
            { \
                if (FloatVectorHelpers::isAligned (src2))   HIGGS_VEC_LOOP_TWO_SOURCES (vecOp, Mode::loadA, Mode::loadA, Mode::storeU, locals, increment) \
                else                                        HIGGS_VEC_LOOP_TWO_SOURCES (vecOp, Mode::loadA, Mode::loadU, Mode::storeU, locals, increment) \
            } \
            else \
            { \
                if (FloatVectorHelpers::isAligned (src2))   HIGGS_VEC_LOOP_TWO_SOURCES (vecOp, Mode::loadU, Mode::loadA, Mode::storeU, locals, increment) \
                else                                        HIGGS_VEC_LOOP_TWO_SOURCES (vecOp, Mode::loadU, Mode::loadU, Mode::storeU, locals, increment) \
            } \
        } \
        HIGGS_FINISH_VEC_OP (normalOp)

    #define HIGGS_PERFORM_VEC_OP_SRC1_SRC2_DEST_DEST(normalOp, vecOp, locals, increment, setupOp) \
        HIGGS_BEGIN_VEC_OP \
        setupOp \
        if (FloatVectorHelpers::isAligned (dest)) \
        { \
            if (FloatVectorHelpers::isAligned (src1)) \
            { \
                if (FloatVectorHelpers::isAligned (src2))   HIGGS_VEC_LOOP_TWO_SOURCES_WITH_DEST_LOAD (vecOp, Mode::loadA, Mode::loadA, Mode::loadA, Mode::storeA, locals, increment) \
                else                                        HIGGS_VEC_LOOP_TWO_SOURCES_WITH_DEST_LOAD (vecOp, Mode::loadA, Mode::loadU, Mode::loadA, Mode::storeA, locals, increment) \
            } \
            else \
            { \
                if (FloatVectorHelpers::isAligned (src2))   HIGGS_VEC_LOOP_TWO_SOURCES_WITH_DEST_LOAD (vecOp, Mode::loadU, Mode::loadA, Mode::loadA, Mode::storeA, locals, increment) \
                else                                        HIGGS_VEC_LOOP_TWO_SOURCES_WITH_DEST_LOAD (vecOp, Mode::loadU, Mode::loadU, Mode::loadA, Mode::storeA, locals, increment) \
            } \
        } \
        else \
        { \
            if (FloatVectorHelpers::isAligned (src1)) \
            { \
                if (FloatVectorHelpers::isAligned (src2))   HIGGS_VEC_LOOP_TWO_SOURCES_WITH_DEST_LOAD (vecOp, Mode::loadA, Mode::loadA, Mode::loadU, Mode::storeU, locals, increment) \
                else                                        HIGGS_VEC_LOOP_TWO_SOURCES_WITH_DEST_LOAD (vecOp, Mode::loadA, Mode::loadU, Mode::loadU, Mode::storeU, locals, increment) \
            } \
            else \
            { \
                if (FloatVectorHelpers::isAligned (src2))   HIGGS_VEC_LOOP_TWO_SOURCES_WITH_DEST_LOAD (vecOp, Mode::loadU, Mode::loadA, Mode::loadU, Mode::storeU, locals, increment) \
                else                                        HIGGS_VEC_LOOP_TWO_SOURCES_WITH_DEST_LOAD (vecOp, Mode::loadU, Mode::loadU, Mode::loadU, Mode::storeU, locals, increment) \
            } \
        } \
        HIGGS_FINISH_VEC_OP (normalOp)


    //==============================================================================
   #elif HIGGS_USE_ARM_NEON

    struct BasicOps32
    {
        using Type = float;
        using ParallelType = float32x4_t;
        using IntegerType = uint32x4_t;
        union signMaskUnion { ParallelType f; IntegerType i; };
        enum { numParallel = 4 };

        static forcedinline IntegerType toint (ParallelType v) noexcept                 { signMaskUnion u; u.f = v; return u.i; }
        static forcedinline ParallelType toflt (IntegerType v) noexcept                 { signMaskUnion u; u.i = v; return u.f; }

        static forcedinline ParallelType load1 (Type v) noexcept                        { return vld1q_dup_f32 (&v); }
        static forcedinline ParallelType loadA (const Type* v) noexcept                 { return vld1q_f32 (v); }
        static forcedinline ParallelType loadU (const Type* v) noexcept                 { return vld1q_f32 (v); }
        static forcedinline void storeA (Type* dest, ParallelType a) noexcept           { vst1q_f32 (dest, a); }
        static forcedinline void storeU (Type* dest, ParallelType a) noexcept           { vst1q_f32 (dest, a); }

        static forcedinline ParallelType add (ParallelType a, ParallelType b) noexcept  { return vaddq_f32 (a, b); }
        static forcedinline ParallelType sub (ParallelType a, ParallelType b) noexcept  { return vsubq_f32 (a, b); }
        static forcedinline ParallelType mul (ParallelType a, ParallelType b) noexcept  { return vmulq_f32 (a, b); }
        static forcedinline ParallelType max (ParallelType a, ParallelType b) noexcept  { return vmaxq_f32 (a, b); }
        static forcedinline ParallelType min (ParallelType a, ParallelType b) noexcept  { return vminq_f32 (a, b); }

        static forcedinline ParallelType bit_and (ParallelType a, ParallelType b) noexcept  {  return toflt (vandq_u32 (toint (a), toint (b))); }
        static forcedinline ParallelType bit_not (ParallelType a, ParallelType b) noexcept  {  return toflt (vbicq_u32 (toint (a), toint (b))); }
        static forcedinline ParallelType bit_or  (ParallelType a, ParallelType b) noexcept  {  return toflt (vorrq_u32 (toint (a), toint (b))); }
        static forcedinline ParallelType bit_xor (ParallelType a, ParallelType b) noexcept  {  return toflt (veorq_u32 (toint (a), toint (b))); }

        static forcedinline Type max (ParallelType a) noexcept { Type v[numParallel]; storeU (v, a); return jmax (v[0], v[1], v[2], v[3]); }
        static forcedinline Type min (ParallelType a) noexcept { Type v[numParallel]; storeU (v, a); return jmin (v[0], v[1], v[2], v[3]); }
    };

    struct BasicOps64
    {
        using Type = double;
        using ParallelType = double;
        using IntegerType = uint64;
        union signMaskUnion { ParallelType f; IntegerType i; };
        enum { numParallel = 1 };

        static forcedinline IntegerType toint (ParallelType v) noexcept                 { signMaskUnion u; u.f = v; return u.i; }
        static forcedinline ParallelType toflt (IntegerType v) noexcept                 { signMaskUnion u; u.i = v; return u.f; }

        static forcedinline ParallelType load1 (Type v) noexcept                        { return v; }
        static forcedinline ParallelType loadA (const Type* v) noexcept                 { return *v; }
        static forcedinline ParallelType loadU (const Type* v) noexcept                 { return *v; }
        static forcedinline void storeA (Type* dest, ParallelType a) noexcept           { *dest = a; }
        static forcedinline void storeU (Type* dest, ParallelType a) noexcept           { *dest = a; }

        static forcedinline ParallelType add (ParallelType a, ParallelType b) noexcept  { return a + b; }
        static forcedinline ParallelType sub (ParallelType a, ParallelType b) noexcept  { return a - b; }
        static forcedinline ParallelType mul (ParallelType a, ParallelType b) noexcept  { return a * b; }
        static forcedinline ParallelType max (ParallelType a, ParallelType b) noexcept  { return jmax (a, b); }
        static forcedinline ParallelType min (ParallelType a, ParallelType b) noexcept  { return jmin (a, b); }

        static forcedinline ParallelType bit_and (ParallelType a, ParallelType b) noexcept  {  return toflt (toint (a) & toint (b)); }
        static forcedinline ParallelType bit_not (ParallelType a, ParallelType b) noexcept  {  return toflt ((~toint (a)) & toint (b)); }
        static forcedinline ParallelType bit_or  (ParallelType a, ParallelType b) noexcept  {  return toflt (toint (a) | toint (b)); }
        static forcedinline ParallelType bit_xor (ParallelType a, ParallelType b) noexcept  {  return toflt (toint (a) ^ toint (b)); }

        static forcedinline Type max (ParallelType a) noexcept  { return a; }
        static forcedinline Type min (ParallelType a) noexcept  { return a; }
    };

    #define HIGGS_BEGIN_VEC_OP \
        using Mode = FloatVectorHelpers::ModeType<sizeof(*dest)>::Mode; \
        if (Mode::numParallel > 1) \
        { \
            const auto numLongOps = num / Mode::numParallel;

    #define HIGGS_FINISH_VEC_OP(normalOp) \
            num &= (Mode::numParallel - 1); \
            if (num == 0) return; \
        } \
        for (auto i = (decltype (num)) 0; i < num; ++i) normalOp;

    #define HIGGS_PERFORM_VEC_OP_DEST(normalOp, vecOp, locals, setupOp) \
        HIGGS_BEGIN_VEC_OP \
        setupOp \
        HIGGS_VEC_LOOP (vecOp, dummy, Mode::loadU, Mode::storeU, locals, HIGGS_INCREMENT_DEST) \
        HIGGS_FINISH_VEC_OP (normalOp)

    #define HIGGS_PERFORM_VEC_OP_SRC_DEST(normalOp, vecOp, locals, increment, setupOp) \
        HIGGS_BEGIN_VEC_OP \
        setupOp \
        HIGGS_VEC_LOOP (vecOp, Mode::loadU, Mode::loadU, Mode::storeU, locals, increment) \
        HIGGS_FINISH_VEC_OP (normalOp)

    #define HIGGS_PERFORM_VEC_OP_SRC1_SRC2_DEST(normalOp, vecOp, locals, increment, setupOp) \
        HIGGS_BEGIN_VEC_OP \
        setupOp \
        HIGGS_VEC_LOOP_TWO_SOURCES (vecOp, Mode::loadU, Mode::loadU, Mode::storeU, locals, increment) \
        HIGGS_FINISH_VEC_OP (normalOp)

    #define HIGGS_PERFORM_VEC_OP_SRC1_SRC2_DEST_DEST(normalOp, vecOp, locals, increment, setupOp) \
        HIGGS_BEGIN_VEC_OP \
        setupOp \
        HIGGS_VEC_LOOP_TWO_SOURCES_WITH_DEST_LOAD (vecOp, Mode::loadU, Mode::loadU, Mode::loadU, Mode::storeU, locals, increment) \
        HIGGS_FINISH_VEC_OP (normalOp)


    //==============================================================================
   #else
    #define HIGGS_PERFORM_VEC_OP_DEST(normalOp, vecOp, locals, setupOp) \
        for (auto i = (decltype (num)) 0; i < num; ++i) normalOp;

    #define HIGGS_PERFORM_VEC_OP_SRC_DEST(normalOp, vecOp, locals, increment, setupOp) \
        for (auto i = (decltype (num)) 0; i < num; ++i) normalOp;

    #define HIGGS_PERFORM_VEC_OP_SRC1_SRC2_DEST(normalOp, vecOp, locals, increment, setupOp) \
        for (auto i = (decltype (num)) 0; i < num; ++i) normalOp;

    #define HIGGS_PERFORM_VEC_OP_SRC1_SRC2_DEST_DEST(normalOp, vecOp, locals, increment, setupOp) \
        for (auto i = (decltype (num)) 0; i < num; ++i) normalOp;

   #endif

    //==============================================================================
    #define HIGGS_VEC_LOOP(vecOp, srcLoad, dstLoad, dstStore, locals, increment) \
        for (auto i = (decltype (numLongOps)) 0; i < numLongOps; ++i) \
        { \
            locals (srcLoad, dstLoad); \
            dstStore (dest, vecOp); \
            increment; \
        }

    #define HIGGS_VEC_LOOP_TWO_SOURCES(vecOp, src1Load, src2Load, dstStore, locals, increment) \
        for (auto i = (decltype (numLongOps)) 0; i < numLongOps; ++i) \
        { \
            locals (src1Load, src2Load); \
            dstStore (dest, vecOp); \
            increment; \
        }

    #define HIGGS_VEC_LOOP_TWO_SOURCES_WITH_DEST_LOAD(vecOp, src1Load, src2Load, dstLoad, dstStore, locals, increment) \
        for (auto i = (decltype (numLongOps)) 0; i < numLongOps; ++i) \
        { \
            locals (src1Load, src2Load, dstLoad); \
            dstStore (dest, vecOp); \
            increment; \
        }

    #define HIGGS_LOAD_NONE(srcLoad, dstLoad)
    #define HIGGS_LOAD_DEST(srcLoad, dstLoad)                        const Mode::ParallelType d = dstLoad (dest);
    #define HIGGS_LOAD_SRC(srcLoad, dstLoad)                         const Mode::ParallelType s = srcLoad (src);
    #define HIGGS_LOAD_SRC1_SRC2(src1Load, src2Load)                 const Mode::ParallelType s1 = src1Load (src1), s2 = src2Load (src2);
    #define HIGGS_LOAD_SRC1_SRC2_DEST(src1Load, src2Load, dstLoad)   const Mode::ParallelType d = dstLoad (dest), s1 = src1Load (src1), s2 = src2Load (src2);
    #define HIGGS_LOAD_SRC_DEST(srcLoad, dstLoad)                    const Mode::ParallelType d = dstLoad (dest), s = srcLoad (src);

    union signMask32 { float  f; uint32 i; };
    union signMask64 { double d; uint64 i; };

   #if HIGGS_USE_SSE_INTRINSICS || HIGGS_USE_ARM_NEON
    template <int typeSize> struct ModeType    { using Mode = BasicOps32; };
    template <>             struct ModeType<8> { using Mode = BasicOps64; };

    template <typename Mode>
    struct MinMax
    {
        using Type = typename Mode::Type;
        using ParallelType = typename Mode::ParallelType;

        template <typename Size>
        static Type findMinOrMax (const Type* src, Size num, const bool isMinimum) noexcept
        {
            auto numLongOps = num / Mode::numParallel;

            if (numLongOps > 1)
            {
                ParallelType val;

               #if ! HIGGS_USE_ARM_NEON
                if (isAligned (src))
                {
                    val = Mode::loadA (src);

                    if (isMinimum)
                    {
                        while (--numLongOps > 0)
                        {
                            src += Mode::numParallel;
                            val = Mode::min (val, Mode::loadA (src));
                        }
                    }
                    else
                    {
                        while (--numLongOps > 0)
                        {
                            src += Mode::numParallel;
                            val = Mode::max (val, Mode::loadA (src));
                        }
                    }
                }
                else
               #endif
                {
                    val = Mode::loadU (src);

                    if (isMinimum)
                    {
                        while (--numLongOps > 0)
                        {
                            src += Mode::numParallel;
                            val = Mode::min (val, Mode::loadU (src));
                        }
                    }
                    else
                    {
                        while (--numLongOps > 0)
                        {
                            src += Mode::numParallel;
                            val = Mode::max (val, Mode::loadU (src));
                        }
                    }
                }

                Type result = isMinimum ? Mode::min (val)
                                        : Mode::max (val);

                num &= (Mode::numParallel - 1);
                src += Mode::numParallel;

                for (auto i = (decltype (num)) 0; i < num; ++i)
                    result = isMinimum ? jmin (result, src[i])
                                       : jmax (result, src[i]);

                return result;
            }

            if (num <= 0)
                return 0;

            return isMinimum ? *std::min_element (src, src + num)
                             : *std::max_element (src, src + num);
        }

        template <typename Size>
        static Range<Type> findMinAndMax (const Type* src, Size num) noexcept
        {
            auto numLongOps = num / Mode::numParallel;

            if (numLongOps > 1)
            {
                ParallelType mn, mx;

               #if ! HIGGS_USE_ARM_NEON
                if (isAligned (src))
                {
                    mn = Mode::loadA (src);
                    mx = mn;

                    while (--numLongOps > 0)
                    {
                        src += Mode::numParallel;
                        const ParallelType v = Mode::loadA (src);
                        mn = Mode::min (mn, v);
                        mx = Mode::max (mx, v);
                    }
                }
                else
               #endif
                {
                    mn = Mode::loadU (src);
                    mx = mn;

                    while (--numLongOps > 0)
                    {
                        src += Mode::numParallel;
                        const ParallelType v = Mode::loadU (src);
                        mn = Mode::min (mn, v);
                        mx = Mode::max (mx, v);
                    }
                }

                Range<Type> result (Mode::min (mn),
                                    Mode::max (mx));

                num &= (Mode::numParallel - 1);
                src += Mode::numParallel;

                for (auto i = (decltype (num)) 0; i < num; ++i)
                    result = result.getUnionWith (src[i]);

                return result;
            }

            return Range<Type>::findMinAndMax (src, num);
        }
    };
   #endif

//==============================================================================
namespace
{
    template <typename Size>
    void clear (float* dest, Size num) noexcept
    {
       #if HIGGS_USE_VDSP_FRAMEWORK
        vDSP_vclr (dest, 1, (vDSP_Length) num);
       #else
        zeromem (dest, (size_t) num * sizeof (float));
       #endif
    }

    template <typename Size>
    void clear (double* dest, Size num) noexcept
    {
       #if HIGGS_USE_VDSP_FRAMEWORK
        vDSP_vclrD (dest, 1, (vDSP_Length) num);
       #else
        zeromem (dest, (size_t) num * sizeof (double));
       #endif
    }

    template <typename Size>
    void fill (float* dest, float valueToFill, Size num) noexcept
    {
       #if HIGGS_USE_VDSP_FRAMEWORK
        vDSP_vfill (&valueToFill, dest, 1, (vDSP_Length) num);
       #else
        HIGGS_PERFORM_VEC_OP_DEST (dest[i] = valueToFill,
                                  val,
                                  HIGGS_LOAD_NONE,
                                  const Mode::ParallelType val = Mode::load1 (valueToFill);)
       #endif
    }

    template <typename Size>
    void fill (double* dest, double valueToFill, Size num) noexcept
    {
       #if HIGGS_USE_VDSP_FRAMEWORK
        vDSP_vfillD (&valueToFill, dest, 1, (vDSP_Length) num);
       #else
        HIGGS_PERFORM_VEC_OP_DEST (dest[i] = valueToFill,
                                  val,
                                  HIGGS_LOAD_NONE,
                                  const Mode::ParallelType val = Mode::load1 (valueToFill);)
       #endif
    }

    template <typename Size>
    void copyWithMultiply (float* dest, const float* src, float multiplier, Size num) noexcept
    {
       #if HIGGS_USE_VDSP_FRAMEWORK
        vDSP_vsmul (src, 1, &multiplier, dest, 1, (vDSP_Length) num);
       #else
        HIGGS_PERFORM_VEC_OP_SRC_DEST (dest[i] = src[i] * multiplier,
                                      Mode::mul (mult, s),
                                      HIGGS_LOAD_SRC,
                                      HIGGS_INCREMENT_SRC_DEST,
                                      const Mode::ParallelType mult = Mode::load1 (multiplier);)
       #endif
    }

    template <typename Size>
    void copyWithMultiply (double* dest, const double* src, double multiplier, Size num) noexcept
    {
       #if HIGGS_USE_VDSP_FRAMEWORK
        vDSP_vsmulD (src, 1, &multiplier, dest, 1, (vDSP_Length) num);
       #else
        HIGGS_PERFORM_VEC_OP_SRC_DEST (dest[i] = src[i] * multiplier,
                                      Mode::mul (mult, s),
                                      HIGGS_LOAD_SRC,
                                      HIGGS_INCREMENT_SRC_DEST,
                                      const Mode::ParallelType mult = Mode::load1 (multiplier);)
       #endif
    }

    template <typename Size>
    void add (float* dest, float amount, Size num) noexcept
    {
       #if HIGGS_USE_VDSP_FRAMEWORK
        vDSP_vsadd (dest, 1, &amount, dest, 1, (vDSP_Length) num);
       #else
        HIGGS_PERFORM_VEC_OP_DEST (dest[i] += amount,
                                  Mode::add (d, amountToAdd),
                                  HIGGS_LOAD_DEST,
                                  const Mode::ParallelType amountToAdd = Mode::load1 (amount);)
       #endif
    }

    template <typename Size>
    void add (double* dest, double amount, Size num) noexcept
    {
        HIGGS_PERFORM_VEC_OP_DEST (dest[i] += amount,
                                  Mode::add (d, amountToAdd),
                                  HIGGS_LOAD_DEST,
                                  const Mode::ParallelType amountToAdd = Mode::load1 (amount);)
    }

    template <typename Size>
    void add (float* dest, const float* src, float amount, Size num) noexcept
    {
       #if HIGGS_USE_VDSP_FRAMEWORK
        vDSP_vsadd (src, 1, &amount, dest, 1, (vDSP_Length) num);
       #else
        HIGGS_PERFORM_VEC_OP_SRC_DEST (dest[i] = src[i] + amount,
                                      Mode::add (am, s),
                                      HIGGS_LOAD_SRC,
                                      HIGGS_INCREMENT_SRC_DEST,
                                      const Mode::ParallelType am = Mode::load1 (amount);)
       #endif
    }

    template <typename Size>
    void add (double* dest, const double* src, double amount, Size num) noexcept
    {
       #if HIGGS_USE_VDSP_FRAMEWORK
        vDSP_vsaddD (src, 1, &amount, dest, 1, (vDSP_Length) num);
       #else
        HIGGS_PERFORM_VEC_OP_SRC_DEST (dest[i] = src[i] + amount,
                                      Mode::add (am, s),
                                      HIGGS_LOAD_SRC,
                                      HIGGS_INCREMENT_SRC_DEST,
                                      const Mode::ParallelType am = Mode::load1 (amount);)
       #endif
    }

    template <typename Size>
    void add (float* dest, const float* src, Size num) noexcept
    {
       #if HIGGS_USE_VDSP_FRAMEWORK
        vDSP_vadd (src, 1, dest, 1, dest, 1, (vDSP_Length) num);
       #else
        HIGGS_PERFORM_VEC_OP_SRC_DEST (dest[i] += src[i],
                                      Mode::add (d, s),
                                      HIGGS_LOAD_SRC_DEST,
                                      HIGGS_INCREMENT_SRC_DEST, )
       #endif
    }

    template <typename Size>
    void add (double* dest, const double* src, Size num) noexcept
    {
       #if HIGGS_USE_VDSP_FRAMEWORK
        vDSP_vaddD (src, 1, dest, 1, dest, 1, (vDSP_Length) num);
       #else
        HIGGS_PERFORM_VEC_OP_SRC_DEST (dest[i] += src[i],
                                      Mode::add (d, s),
                                      HIGGS_LOAD_SRC_DEST,
                                      HIGGS_INCREMENT_SRC_DEST, )
       #endif
    }

    template <typename Size>
    void add (float* dest, const float* src1, const float* src2, Size num) noexcept
    {
       #if HIGGS_USE_VDSP_FRAMEWORK
        vDSP_vadd (src1, 1, src2, 1, dest, 1, (vDSP_Length) num);
       #else
        HIGGS_PERFORM_VEC_OP_SRC1_SRC2_DEST (dest[i] = src1[i] + src2[i],
                                            Mode::add (s1, s2),
                                            HIGGS_LOAD_SRC1_SRC2,
                                            HIGGS_INCREMENT_SRC1_SRC2_DEST, )
       #endif
    }

    template <typename Size>
    void add (double* dest, const double* src1, const double* src2, Size num) noexcept
    {
       #if HIGGS_USE_VDSP_FRAMEWORK
        vDSP_vaddD (src1, 1, src2, 1, dest, 1, (vDSP_Length) num);
       #else
        HIGGS_PERFORM_VEC_OP_SRC1_SRC2_DEST (dest[i] = src1[i] + src2[i],
                                            Mode::add (s1, s2),
                                            HIGGS_LOAD_SRC1_SRC2,
                                            HIGGS_INCREMENT_SRC1_SRC2_DEST, )
       #endif
    }

    template <typename Size>
    void subtract (float* dest, const float* src, Size num) noexcept
    {
       #if HIGGS_USE_VDSP_FRAMEWORK
        vDSP_vsub (src, 1, dest, 1, dest, 1, (vDSP_Length) num);
       #else
        HIGGS_PERFORM_VEC_OP_SRC_DEST (dest[i] -= src[i],
                                      Mode::sub (d, s),
                                      HIGGS_LOAD_SRC_DEST,
                                      HIGGS_INCREMENT_SRC_DEST, )
       #endif
    }

    template <typename Size>
    void subtract (double* dest, const double* src, Size num) noexcept
    {
       #if HIGGS_USE_VDSP_FRAMEWORK
        vDSP_vsubD (src, 1, dest, 1, dest, 1, (vDSP_Length) num);
       #else
        HIGGS_PERFORM_VEC_OP_SRC_DEST (dest[i] -= src[i],
                                      Mode::sub (d, s),
                                      HIGGS_LOAD_SRC_DEST,
                                      HIGGS_INCREMENT_SRC_DEST, )
       #endif
    }

    template <typename Size>
    void subtract (float* dest, const float* src1, const float* src2, Size num) noexcept
    {
       #if HIGGS_USE_VDSP_FRAMEWORK
        vDSP_vsub (src2, 1, src1, 1, dest, 1, (vDSP_Length) num);
       #else
        HIGGS_PERFORM_VEC_OP_SRC1_SRC2_DEST (dest[i] = src1[i] - src2[i],
                                            Mode::sub (s1, s2),
                                            HIGGS_LOAD_SRC1_SRC2,
                                            HIGGS_INCREMENT_SRC1_SRC2_DEST, )
       #endif
    }

    template <typename Size>
    void subtract (double* dest, const double* src1, const double* src2, Size num) noexcept
    {
       #if HIGGS_USE_VDSP_FRAMEWORK
        vDSP_vsubD (src2, 1, src1, 1, dest, 1, (vDSP_Length) num);
       #else
        HIGGS_PERFORM_VEC_OP_SRC1_SRC2_DEST (dest[i] = src1[i] - src2[i],
                                            Mode::sub (s1, s2),
                                            HIGGS_LOAD_SRC1_SRC2,
                                            HIGGS_INCREMENT_SRC1_SRC2_DEST, )
       #endif
    }

    template <typename Size>
    void addWithMultiply (float* dest, const float* src, float multiplier, Size num) noexcept
    {
       #if HIGGS_USE_VDSP_FRAMEWORK
        vDSP_vsma (src, 1, &multiplier, dest, 1, dest, 1, (vDSP_Length) num);
       #else
        HIGGS_PERFORM_VEC_OP_SRC_DEST (dest[i] += src[i] * multiplier,
                                      Mode::add (d, Mode::mul (mult, s)),
                                      HIGGS_LOAD_SRC_DEST,
                                      HIGGS_INCREMENT_SRC_DEST,
                                      const Mode::ParallelType mult = Mode::load1 (multiplier);)
       #endif
    }

    template <typename Size>
    void addWithMultiply (double* dest, const double* src, double multiplier, Size num) noexcept
    {
       #if HIGGS_USE_VDSP_FRAMEWORK
        vDSP_vsmaD (src, 1, &multiplier, dest, 1, dest, 1, (vDSP_Length) num);
       #else
        HIGGS_PERFORM_VEC_OP_SRC_DEST (dest[i] += src[i] * multiplier,
                                      Mode::add (d, Mode::mul (mult, s)),
                                      HIGGS_LOAD_SRC_DEST,
                                      HIGGS_INCREMENT_SRC_DEST,
                                      const Mode::ParallelType mult = Mode::load1 (multiplier);)
       #endif
    }

    template <typename Size>
    void addWithMultiply (float* dest, const float* src1, const float* src2, Size num) noexcept
    {
       #if HIGGS_USE_VDSP_FRAMEWORK
        vDSP_vma ((float*) src1, 1, (float*) src2, 1, dest, 1, dest, 1, (vDSP_Length) num);
       #else
        HIGGS_PERFORM_VEC_OP_SRC1_SRC2_DEST_DEST (dest[i] += src1[i] * src2[i],
                                                 Mode::add (d, Mode::mul (s1, s2)),
                                                 HIGGS_LOAD_SRC1_SRC2_DEST,
                                                 HIGGS_INCREMENT_SRC1_SRC2_DEST, )
       #endif
    }

    template <typename Size>
    void addWithMultiply (double* dest, const double* src1, const double* src2, Size num) noexcept
    {
       #if HIGGS_USE_VDSP_FRAMEWORK
        vDSP_vmaD ((double*) src1, 1, (double*) src2, 1, dest, 1, dest, 1, (vDSP_Length) num);
       #else
        HIGGS_PERFORM_VEC_OP_SRC1_SRC2_DEST_DEST (dest[i] += src1[i] * src2[i],
                                                 Mode::add (d, Mode::mul (s1, s2)),
                                                 HIGGS_LOAD_SRC1_SRC2_DEST,
                                                 HIGGS_INCREMENT_SRC1_SRC2_DEST, )
       #endif
    }

    template <typename Size>
    void subtractWithMultiply (float* dest, const float* src, float multiplier, Size num) noexcept
    {
        HIGGS_PERFORM_VEC_OP_SRC_DEST (dest[i] -= src[i] * multiplier,
                                      Mode::sub (d, Mode::mul (mult, s)),
                                      HIGGS_LOAD_SRC_DEST,
                                      HIGGS_INCREMENT_SRC_DEST,
                                      const Mode::ParallelType mult = Mode::load1 (multiplier);)
    }

    template <typename Size>
    void subtractWithMultiply (double* dest, const double* src, double multiplier, Size num) noexcept
    {
        HIGGS_PERFORM_VEC_OP_SRC_DEST (dest[i] -= src[i] * multiplier,
                                      Mode::sub (d, Mode::mul (mult, s)),
                                      HIGGS_LOAD_SRC_DEST,
                                      HIGGS_INCREMENT_SRC_DEST,
                                      const Mode::ParallelType mult = Mode::load1 (multiplier);)
    }

    template <typename Size>
    void subtractWithMultiply (float* dest, const float* src1, const float* src2, Size num) noexcept
    {
        HIGGS_PERFORM_VEC_OP_SRC1_SRC2_DEST_DEST (dest[i] -= src1[i] * src2[i],
                                                 Mode::sub (d, Mode::mul (s1, s2)),
                                                 HIGGS_LOAD_SRC1_SRC2_DEST,
                                                 HIGGS_INCREMENT_SRC1_SRC2_DEST, )
    }

    template <typename Size>
    void subtractWithMultiply (double* dest, const double* src1, const double* src2, Size num) noexcept
    {
        HIGGS_PERFORM_VEC_OP_SRC1_SRC2_DEST_DEST (dest[i] -= src1[i] * src2[i],
                                                 Mode::sub (d, Mode::mul (s1, s2)),
                                                 HIGGS_LOAD_SRC1_SRC2_DEST,
                                                 HIGGS_INCREMENT_SRC1_SRC2_DEST, )
    }

    template <typename Size>
    void multiply (float* dest, const float* src, Size num) noexcept
    {
       #if HIGGS_USE_VDSP_FRAMEWORK
        vDSP_vmul (src, 1, dest, 1, dest, 1, (vDSP_Length) num);
       #else
        HIGGS_PERFORM_VEC_OP_SRC_DEST (dest[i] *= src[i],
                                      Mode::mul (d, s),
                                      HIGGS_LOAD_SRC_DEST,
                                      HIGGS_INCREMENT_SRC_DEST, )
       #endif
    }

    template <typename Size>
    void multiply (double* dest, const double* src, Size num) noexcept
    {
       #if HIGGS_USE_VDSP_FRAMEWORK
        vDSP_vmulD (src, 1, dest, 1, dest, 1, (vDSP_Length) num);
       #else
        HIGGS_PERFORM_VEC_OP_SRC_DEST (dest[i] *= src[i],
                                      Mode::mul (d, s),
                                      HIGGS_LOAD_SRC_DEST,
                                      HIGGS_INCREMENT_SRC_DEST, )
       #endif
    }

    template <typename Size>
    void multiply (float* dest, const float* src1, const float* src2, Size num) noexcept
    {
       #if HIGGS_USE_VDSP_FRAMEWORK
        vDSP_vmul (src1, 1, src2, 1, dest, 1, (vDSP_Length) num);
       #else
        HIGGS_PERFORM_VEC_OP_SRC1_SRC2_DEST (dest[i] = src1[i] * src2[i],
                                            Mode::mul (s1, s2),
                                            HIGGS_LOAD_SRC1_SRC2,
                                            HIGGS_INCREMENT_SRC1_SRC2_DEST, )
       #endif
    }

    template <typename Size>
    void multiply (double* dest, const double* src1, const double* src2, Size num) noexcept
    {
       #if HIGGS_USE_VDSP_FRAMEWORK
        vDSP_vmulD (src1, 1, src2, 1, dest, 1, (vDSP_Length) num);
       #else
        HIGGS_PERFORM_VEC_OP_SRC1_SRC2_DEST (dest[i] = src1[i] * src2[i],
                                            Mode::mul (s1, s2),
                                            HIGGS_LOAD_SRC1_SRC2,
                                            HIGGS_INCREMENT_SRC1_SRC2_DEST, )
       #endif
    }

    template <typename Size>
    void multiply (float* dest, float multiplier, Size num) noexcept
    {
       #if HIGGS_USE_VDSP_FRAMEWORK
        vDSP_vsmul (dest, 1, &multiplier, dest, 1, (vDSP_Length) num);
       #else
        HIGGS_PERFORM_VEC_OP_DEST (dest[i] *= multiplier,
                                  Mode::mul (d, mult),
                                  HIGGS_LOAD_DEST,
                                  const Mode::ParallelType mult = Mode::load1 (multiplier);)
       #endif
    }

    template <typename Size>
    void multiply (double* dest, double multiplier, Size num) noexcept
    {
       #if HIGGS_USE_VDSP_FRAMEWORK
        vDSP_vsmulD (dest, 1, &multiplier, dest, 1, (vDSP_Length) num);
       #else
        HIGGS_PERFORM_VEC_OP_DEST (dest[i] *= multiplier,
                                  Mode::mul (d, mult),
                                  HIGGS_LOAD_DEST,
                                  const Mode::ParallelType mult = Mode::load1 (multiplier);)
       #endif
    }

    template <typename Size>
    void multiply (float* dest, const float* src, float multiplier, Size num) noexcept
    {
        HIGGS_PERFORM_VEC_OP_SRC_DEST (dest[i] = src[i] * multiplier,
                                      Mode::mul (mult, s),
                                      HIGGS_LOAD_SRC,
                                      HIGGS_INCREMENT_SRC_DEST,
                                      const Mode::ParallelType mult = Mode::load1 (multiplier);)
    }

    template <typename Size>
    void multiply (double* dest, const double* src, double multiplier, Size num) noexcept
    {
        HIGGS_PERFORM_VEC_OP_SRC_DEST (dest[i] = src[i] * multiplier,
                                      Mode::mul (mult, s),
                                      HIGGS_LOAD_SRC,
                                      HIGGS_INCREMENT_SRC_DEST,
                                      const Mode::ParallelType mult = Mode::load1 (multiplier);)
    }

    template <typename Size>
    void negate (float* dest, const float* src, Size num) noexcept
    {
       #if HIGGS_USE_VDSP_FRAMEWORK
        vDSP_vneg ((float*) src, 1, dest, 1, (vDSP_Length) num);
       #else
        copyWithMultiply (dest, src, -1.0f, num);
       #endif
    }

    template <typename Size>
    void negate (double* dest, const double* src, Size num) noexcept
    {
       #if HIGGS_USE_VDSP_FRAMEWORK
        vDSP_vnegD ((double*) src, 1, dest, 1, (vDSP_Length) num);
       #else
        copyWithMultiply (dest, src, -1.0f, num);
       #endif
    }

    template <typename Size>
    void abs (float* dest, const float* src, Size num) noexcept
    {
       #if HIGGS_USE_VDSP_FRAMEWORK
        vDSP_vabs ((float*) src, 1, dest, 1, (vDSP_Length) num);
       #else
        [[maybe_unused]] FloatVectorHelpers::signMask32 signMask;
        signMask.i = 0x7fffffffUL;
        HIGGS_PERFORM_VEC_OP_SRC_DEST (dest[i] = std::abs (src[i]),
                                      Mode::bit_and (s, mask),
                                      HIGGS_LOAD_SRC,
                                      HIGGS_INCREMENT_SRC_DEST,
                                      const Mode::ParallelType mask = Mode::load1 (signMask.f);)
       #endif
    }

    template <typename Size>
    void abs (double* dest, const double* src, Size num) noexcept
    {
       #if HIGGS_USE_VDSP_FRAMEWORK
        vDSP_vabsD ((double*) src, 1, dest, 1, (vDSP_Length) num);
       #else
        [[maybe_unused]] FloatVectorHelpers::signMask64 signMask;
        signMask.i = 0x7fffffffffffffffULL;

        HIGGS_PERFORM_VEC_OP_SRC_DEST (dest[i] = std::abs (src[i]),
                                      Mode::bit_and (s, mask),
                                      HIGGS_LOAD_SRC,
                                      HIGGS_INCREMENT_SRC_DEST,
                                      const Mode::ParallelType mask = Mode::load1 (signMask.d);)
       #endif
    }

    template <typename Size>
    void min (float* dest, const float* src, float comp, Size num) noexcept
    {
        HIGGS_PERFORM_VEC_OP_SRC_DEST (dest[i] = jmin (src[i], comp),
                                      Mode::min (s, cmp),
                                      HIGGS_LOAD_SRC,
                                      HIGGS_INCREMENT_SRC_DEST,
                                      const Mode::ParallelType cmp = Mode::load1 (comp);)
    }

    template <typename Size>
    void min (double* dest, const double* src, double comp, Size num) noexcept
    {
        HIGGS_PERFORM_VEC_OP_SRC_DEST (dest[i] = jmin (src[i], comp),
                                      Mode::min (s, cmp),
                                      HIGGS_LOAD_SRC,
                                      HIGGS_INCREMENT_SRC_DEST,
                                      const Mode::ParallelType cmp = Mode::load1 (comp);)
    }

    template <typename Size>
    void min (float* dest, const float* src1, const float* src2, Size num) noexcept
    {
       #if HIGGS_USE_VDSP_FRAMEWORK
        vDSP_vmin ((float*) src1, 1, (float*) src2, 1, dest, 1, (vDSP_Length) num);
       #else
        HIGGS_PERFORM_VEC_OP_SRC1_SRC2_DEST (dest[i] = jmin (src1[i], src2[i]),
                                            Mode::min (s1, s2),
                                            HIGGS_LOAD_SRC1_SRC2,
                                            HIGGS_INCREMENT_SRC1_SRC2_DEST, )
       #endif
    }

    template <typename Size>
    void min (double* dest, const double* src1, const double* src2, Size num) noexcept
    {
       #if HIGGS_USE_VDSP_FRAMEWORK
        vDSP_vminD ((double*) src1, 1, (double*) src2, 1, dest, 1, (vDSP_Length) num);
       #else
        HIGGS_PERFORM_VEC_OP_SRC1_SRC2_DEST (dest[i] = jmin (src1[i], src2[i]),
                                            Mode::min (s1, s2),
                                            HIGGS_LOAD_SRC1_SRC2,
                                            HIGGS_INCREMENT_SRC1_SRC2_DEST, )
       #endif
    }

    template <typename Size>
    void max (float* dest, const float* src, float comp, Size num) noexcept
    {
        HIGGS_PERFORM_VEC_OP_SRC_DEST (dest[i] = jmax (src[i], comp),
                                      Mode::max (s, cmp),
                                      HIGGS_LOAD_SRC,
                                      HIGGS_INCREMENT_SRC_DEST,
                                      const Mode::ParallelType cmp = Mode::load1 (comp);)
    }

    template <typename Size>
    void max (double* dest, const double* src, double comp, Size num) noexcept
    {
        HIGGS_PERFORM_VEC_OP_SRC_DEST (dest[i] = jmax (src[i], comp),
                                      Mode::max (s, cmp),
                                      HIGGS_LOAD_SRC,
                                      HIGGS_INCREMENT_SRC_DEST,
                                      const Mode::ParallelType cmp = Mode::load1 (comp);)
    }

    template <typename Size>
    void max (float* dest, const float* src1, const float* src2, Size num) noexcept
    {
       #if HIGGS_USE_VDSP_FRAMEWORK
        vDSP_vmax ((float*) src1, 1, (float*) src2, 1, dest, 1, (vDSP_Length) num);
       #else
        HIGGS_PERFORM_VEC_OP_SRC1_SRC2_DEST (dest[i] = jmax (src1[i], src2[i]),
                                            Mode::max (s1, s2),
                                            HIGGS_LOAD_SRC1_SRC2,
                                            HIGGS_INCREMENT_SRC1_SRC2_DEST, )
       #endif
    }

    template <typename Size>
    void max (double* dest, const double* src1, const double* src2, Size num) noexcept
    {
       #if HIGGS_USE_VDSP_FRAMEWORK
        vDSP_vmaxD ((double*) src1, 1, (double*) src2, 1, dest, 1, (vDSP_Length) num);
       #else
        HIGGS_PERFORM_VEC_OP_SRC1_SRC2_DEST (dest[i] = jmax (src1[i], src2[i]),
                                            Mode::max (s1, s2),
                                            HIGGS_LOAD_SRC1_SRC2,
                                            HIGGS_INCREMENT_SRC1_SRC2_DEST, )
       #endif
    }

    template <typename Size>
    void clip (float* dest, const float* src, float low, float high, Size num) noexcept
    {
        jassert (high >= low);

       #if HIGGS_USE_VDSP_FRAMEWORK
        vDSP_vclip ((float*) src, 1, &low, &high, dest, 1, (vDSP_Length) num);
       #else
        HIGGS_PERFORM_VEC_OP_SRC_DEST (dest[i] = jmax (jmin (src[i], high), low),
                                      Mode::max (Mode::min (s, hi), lo),
                                      HIGGS_LOAD_SRC,
                                      HIGGS_INCREMENT_SRC_DEST,
                                      const Mode::ParallelType lo = Mode::load1 (low);
                                      const Mode::ParallelType hi = Mode::load1 (high);)
       #endif
    }

    template <typename Size>
    void clip (double* dest, const double* src, double low, double high, Size num) noexcept
    {
        jassert (high >= low);

       #if HIGGS_USE_VDSP_FRAMEWORK
        vDSP_vclipD ((double*) src, 1, &low, &high, dest, 1, (vDSP_Length) num);
       #else
        HIGGS_PERFORM_VEC_OP_SRC_DEST (dest[i] = jmax (jmin (src[i], high), low),
                                      Mode::max (Mode::min (s, hi), lo),
                                      HIGGS_LOAD_SRC,
                                      HIGGS_INCREMENT_SRC_DEST,
                                      const Mode::ParallelType lo = Mode::load1 (low);
                                      const Mode::ParallelType hi = Mode::load1 (high);)
       #endif
    }

    template <typename Size>
    Range<float> findMinAndMax (const float* src, Size num) noexcept
    {
       #if HIGGS_USE_SSE_INTRINSICS || HIGGS_USE_ARM_NEON
        return FloatVectorHelpers::MinMax<FloatVectorHelpers::BasicOps32>::findMinAndMax (src, num);
       #else
        return Range<float>::findMinAndMax (src, num);
       #endif
    }

    template <typename Size>
    Range<double> findMinAndMax (const double* src, Size num) noexcept
    {
       #if HIGGS_USE_SSE_INTRINSICS || HIGGS_USE_ARM_NEON
        return FloatVectorHelpers::MinMax<FloatVectorHelpers::BasicOps64>::findMinAndMax (src, num);
       #else
        return Range<double>::findMinAndMax (src, num);
       #endif
    }

    template <typename Size>
    float findMinimum (const float* src, Size num) noexcept
    {
       #if HIGGS_USE_SSE_INTRINSICS || HIGGS_USE_ARM_NEON
        return FloatVectorHelpers::MinMax<FloatVectorHelpers::BasicOps32>::findMinOrMax (src, num, true);
       #else
        return higgs::findMinimum (src, num);
       #endif
    }

    template <typename Size>
    double findMinimum (const double* src, Size num) noexcept
    {
       #if HIGGS_USE_SSE_INTRINSICS || HIGGS_USE_ARM_NEON
        return FloatVectorHelpers::MinMax<FloatVectorHelpers::BasicOps64>::findMinOrMax (src, num, true);
       #else
        return higgs::findMinimum (src, num);
       #endif
    }

    template <typename Size>
    float findMaximum (const float* src, Size num) noexcept
    {
       #if HIGGS_USE_SSE_INTRINSICS || HIGGS_USE_ARM_NEON
        return FloatVectorHelpers::MinMax<FloatVectorHelpers::BasicOps32>::findMinOrMax (src, num, false);
       #else
        return higgs::findMaximum (src, num);
       #endif
    }

    template <typename Size>
    double findMaximum (const double* src, Size num) noexcept
    {
       #if HIGGS_USE_SSE_INTRINSICS || HIGGS_USE_ARM_NEON
        return FloatVectorHelpers::MinMax<FloatVectorHelpers::BasicOps64>::findMinOrMax (src, num, false);
       #else
        return higgs::findMaximum (src, num);
       #endif
    }

    template <typename Size>
    void convertFixedToFloat (float* dest, const int* src, float multiplier, Size num) noexcept
    {
       #if HIGGS_USE_ARM_NEON
        HIGGS_PERFORM_VEC_OP_SRC_DEST (dest[i] = (float) src[i] * multiplier,
                                  vmulq_n_f32 (vcvtq_f32_s32 (vld1q_s32 (src)), multiplier),
                                  HIGGS_LOAD_NONE,
                                  HIGGS_INCREMENT_SRC_DEST, )
       #else
        HIGGS_PERFORM_VEC_OP_SRC_DEST (dest[i] = (float) src[i] * multiplier,
                                      Mode::mul (mult, _mm_cvtepi32_ps (_mm_loadu_si128 (reinterpret_cast<const __m128i*> (src)))),
                                      HIGGS_LOAD_NONE,
                                      HIGGS_INCREMENT_SRC_DEST,
                                      const Mode::ParallelType mult = Mode::load1 (multiplier);)
       #endif
    }

} // namespace
} // namespace FloatVectorHelpers

//==============================================================================
template <typename FloatType, typename CountType>
void HIGGS_CALLTYPE FloatVectorOperationsBase<FloatType, CountType>::clear (FloatType* dest,
                                                                           CountType numValues) noexcept
{
    FloatVectorHelpers::clear (dest, numValues);
}

template <typename FloatType, typename CountType>
void HIGGS_CALLTYPE FloatVectorOperationsBase<FloatType, CountType>::fill (FloatType* dest,
                                                                          FloatType valueToFill,
                                                                          CountType numValues) noexcept
{
    FloatVectorHelpers::fill (dest, valueToFill, numValues);
}

template <typename FloatType, typename CountType>
void HIGGS_CALLTYPE FloatVectorOperationsBase<FloatType, CountType>::copy (FloatType* dest,
                                                                          const FloatType* src,
                                                                          CountType numValues) noexcept
{
    memcpy (dest, src, (size_t) numValues * sizeof (FloatType));
}

template <typename FloatType, typename CountType>
void HIGGS_CALLTYPE FloatVectorOperationsBase<FloatType, CountType>::copyWithMultiply (FloatType* dest,
                                                                                      const FloatType* src,
                                                                                      FloatType multiplier,
                                                                                      CountType numValues) noexcept
{
    FloatVectorHelpers::copyWithMultiply (dest, src, multiplier, numValues);
}

template <typename FloatType, typename CountType>
void HIGGS_CALLTYPE FloatVectorOperationsBase<FloatType, CountType>::add (FloatType* dest,
                                                                         FloatType amountToAdd,
                                                                         CountType numValues) noexcept
{
    FloatVectorHelpers::add (dest, amountToAdd, numValues);
}

template <typename FloatType, typename CountType>
void HIGGS_CALLTYPE FloatVectorOperationsBase<FloatType, CountType>::add (FloatType* dest,
                                                                         const FloatType* src,
                                                                         FloatType amount,
                                                                         CountType numValues) noexcept
{
    FloatVectorHelpers::add (dest, src, amount, numValues);
}

template <typename FloatType, typename CountType>
void HIGGS_CALLTYPE FloatVectorOperationsBase<FloatType, CountType>::add (FloatType* dest,
                                                                         const FloatType* src,
                                                                         CountType numValues) noexcept
{
    FloatVectorHelpers::add (dest, src, numValues);
}

template <typename FloatType, typename CountType>
void HIGGS_CALLTYPE FloatVectorOperationsBase<FloatType, CountType>::add (FloatType* dest,
                                                                         const FloatType* src1,
                                                                         const FloatType* src2,
                                                                         CountType num) noexcept
{
    FloatVectorHelpers::add (dest, src1, src2, num);
}

template <typename FloatType, typename CountType>
void HIGGS_CALLTYPE FloatVectorOperationsBase<FloatType, CountType>::subtract (FloatType* dest,
                                                                              const FloatType* src,
                                                                              CountType numValues) noexcept
{
    FloatVectorHelpers::subtract (dest, src, numValues);
}

template <typename FloatType, typename CountType>
void HIGGS_CALLTYPE FloatVectorOperationsBase<FloatType, CountType>::subtract (FloatType* dest,
                                                                              const FloatType* src1,
                                                                              const FloatType* src2,
                                                                              CountType num) noexcept
{
    FloatVectorHelpers::subtract (dest, src1, src2, num);
}

template <typename FloatType, typename CountType>
void HIGGS_CALLTYPE FloatVectorOperationsBase<FloatType, CountType>::addWithMultiply (FloatType* dest,
                                                                                     const FloatType* src,
                                                                                     FloatType multiplier,
                                                                                     CountType numValues) noexcept
{
    FloatVectorHelpers::addWithMultiply (dest, src, multiplier, numValues);
}

template <typename FloatType, typename CountType>
void HIGGS_CALLTYPE FloatVectorOperationsBase<FloatType, CountType>::addWithMultiply (FloatType* dest,
                                                                                     const FloatType* src1,
                                                                                     const FloatType* src2,
                                                                                     CountType num) noexcept
{
    FloatVectorHelpers::addWithMultiply (dest, src1, src2, num);
}

template <typename FloatType, typename CountType>
void HIGGS_CALLTYPE FloatVectorOperationsBase<FloatType, CountType>::subtractWithMultiply (FloatType* dest,
                                                                                          const FloatType* src,
                                                                                          FloatType multiplier,
                                                                                          CountType numValues) noexcept
{
    FloatVectorHelpers::subtractWithMultiply (dest, src, multiplier, numValues);
}

template <typename FloatType, typename CountType>
void HIGGS_CALLTYPE FloatVectorOperationsBase<FloatType, CountType>::subtractWithMultiply (FloatType* dest,
                                                                                          const FloatType* src1,
                                                                                          const FloatType* src2,
                                                                                          CountType num) noexcept
{
    FloatVectorHelpers::subtractWithMultiply (dest, src1, src2, num);
}

template <typename FloatType, typename CountType>
void HIGGS_CALLTYPE FloatVectorOperationsBase<FloatType, CountType>::multiply (FloatType* dest,
                                                                              const FloatType* src,
                                                                              CountType numValues) noexcept
{
    FloatVectorHelpers::multiply (dest, src, numValues);
}

template <typename FloatType, typename CountType>
void HIGGS_CALLTYPE FloatVectorOperationsBase<FloatType, CountType>::multiply (FloatType* dest,
                                                                              const FloatType* src1,
                                                                              const FloatType* src2,
                                                                              CountType numValues) noexcept
{
    FloatVectorHelpers::multiply (dest, src1, src2, numValues);
}

template <typename FloatType, typename CountType>
void HIGGS_CALLTYPE FloatVectorOperationsBase<FloatType, CountType>::multiply (FloatType* dest,
                                                                              FloatType multiplier,
                                                                              CountType numValues) noexcept
{
    FloatVectorHelpers::multiply (dest, multiplier, numValues);
}

template <typename FloatType, typename CountType>
void HIGGS_CALLTYPE FloatVectorOperationsBase<FloatType, CountType>::multiply (FloatType* dest,
                                                                              const FloatType* src,
                                                                              FloatType multiplier,
                                                                              CountType num) noexcept
{
    FloatVectorHelpers::multiply (dest, src, multiplier, num);
}

template <typename FloatType, typename CountType>
void HIGGS_CALLTYPE FloatVectorOperationsBase<FloatType, CountType>::negate (FloatType* dest,
                                                                            const FloatType* src,
                                                                            CountType numValues) noexcept
{
    FloatVectorHelpers::negate (dest, src, numValues);
}

template <typename FloatType, typename CountType>
void HIGGS_CALLTYPE FloatVectorOperationsBase<FloatType, CountType>::abs (FloatType* dest,
                                                                         const FloatType* src,
                                                                         CountType numValues) noexcept
{
    FloatVectorHelpers::abs (dest, src, numValues);
}

template <typename FloatType, typename CountType>
void HIGGS_CALLTYPE FloatVectorOperationsBase<FloatType, CountType>::min (FloatType* dest,
                                                                         const FloatType* src,
                                                                         FloatType comp,
                                                                         CountType num) noexcept
{
    FloatVectorHelpers::min (dest, src, comp, num);
}

template <typename FloatType, typename CountType>
void HIGGS_CALLTYPE FloatVectorOperationsBase<FloatType, CountType>::min (FloatType* dest,
                                                                         const FloatType* src1,
                                                                         const FloatType* src2,
                                                                         CountType num) noexcept
{
    FloatVectorHelpers::min (dest, src1, src2, num);
}

template <typename FloatType, typename CountType>
void HIGGS_CALLTYPE FloatVectorOperationsBase<FloatType, CountType>::max (FloatType* dest,
                                                                         const FloatType* src,
                                                                         FloatType comp,
                                                                         CountType num) noexcept
{
    FloatVectorHelpers::max (dest, src, comp, num);
}

template <typename FloatType, typename CountType>
void HIGGS_CALLTYPE FloatVectorOperationsBase<FloatType, CountType>::max (FloatType* dest,
                                                                         const FloatType* src1,
                                                                         const FloatType* src2,
                                                                         CountType num) noexcept
{
    FloatVectorHelpers::max (dest, src1, src2, num);
}

template <typename FloatType, typename CountType>
void HIGGS_CALLTYPE FloatVectorOperationsBase<FloatType, CountType>::clip (FloatType* dest,
                                                                          const FloatType* src,
                                                                          FloatType low,
                                                                          FloatType high,
                                                                          CountType num) noexcept
{
    FloatVectorHelpers::clip (dest, src, low, high, num);
}

template <typename FloatType, typename CountType>
Range<FloatType> HIGGS_CALLTYPE FloatVectorOperationsBase<FloatType, CountType>::findMinAndMax (const FloatType* src,
                                                                                               CountType numValues) noexcept
{
    return FloatVectorHelpers::findMinAndMax (src, numValues);
}

template <typename FloatType, typename CountType>
FloatType HIGGS_CALLTYPE FloatVectorOperationsBase<FloatType, CountType>::findMinimum (const FloatType* src,
                                                                                      CountType numValues) noexcept
{
    return FloatVectorHelpers::findMinimum (src, numValues);
}

template <typename FloatType, typename CountType>
FloatType HIGGS_CALLTYPE FloatVectorOperationsBase<FloatType, CountType>::findMaximum (const FloatType* src,
                                                                                      CountType numValues) noexcept
{
    return FloatVectorHelpers::findMaximum (src, numValues);
}

template struct FloatVectorOperationsBase<float, int>;
template struct FloatVectorOperationsBase<float, size_t>;
template struct FloatVectorOperationsBase<double, int>;
template struct FloatVectorOperationsBase<double, size_t>;

void HIGGS_CALLTYPE FloatVectorOperations::convertFixedToFloat (float* dest, const int* src, float multiplier, size_t num) noexcept
{
   FloatVectorHelpers::convertFixedToFloat (dest, src, multiplier, num);
}

void HIGGS_CALLTYPE FloatVectorOperations::convertFixedToFloat (float* dest, const int* src, float multiplier, int num) noexcept
{
    FloatVectorHelpers::convertFixedToFloat (dest, src, multiplier, num);
}

intptr_t HIGGS_CALLTYPE FloatVectorOperations::getFpStatusRegister() noexcept
{
    intptr_t fpsr = 0;
 #if HIGGS_INTEL && HIGGS_USE_SSE_INTRINSICS
    fpsr = static_cast<intptr_t> (_mm_getcsr());
 #elif (HIGGS_64BIT && HIGGS_ARM) || HIGGS_USE_ARM_NEON
  #if _MSC_VER
    // _control87 returns static values for x86 bits that don't exist on arm
    // to emulate x86 behaviour. We are only ever interested in de-normal bits
    // so mask out only those.
    fpsr = (intptr_t) (_control87 (0, 0) & _MCW_DN);
  #else
   #if HIGGS_64BIT
    asm volatile("mrs %0, fpcr"
                 : "=r"(fpsr));
   #elif HIGGS_USE_ARM_NEON
    asm volatile("vmrs %0, fpscr"
                 : "=r"(fpsr));
   #endif
  #endif
 #else
  #if ! (defined (HIGGS_INTEL) || defined (HIGGS_ARM))
    jassertfalse; // No support for getting the floating point status register for your platform
  #endif
 #endif

    return fpsr;
}

void HIGGS_CALLTYPE FloatVectorOperations::setFpStatusRegister ([[maybe_unused]] intptr_t fpsr) noexcept
{
 #if HIGGS_INTEL && HIGGS_USE_SSE_INTRINSICS
    // the volatile keyword here is needed to workaround a bug in AppleClang 13.0
    // which aggressively optimises away the variable otherwise
    volatile auto fpsr_w = static_cast<uint32_t> (fpsr);
    _mm_setcsr (fpsr_w);
 #elif (HIGGS_64BIT && HIGGS_ARM) || HIGGS_USE_ARM_NEON
  #if _MSC_VER
    _control87 ((unsigned int) fpsr, _MCW_DN);
  #else
   #if HIGGS_64BIT
    asm volatile("msr fpcr, %0"
                 :
                 : "ri"(fpsr));
   #elif HIGGS_USE_ARM_NEON
    asm volatile("vmsr fpscr, %0"
                 :
                 : "ri"(fpsr));
   #endif
  #endif
 #else
  #if ! (defined (HIGGS_INTEL) || defined (HIGGS_ARM))
    jassertfalse; // No support for getting the floating point status register for your platform
  #endif
 #endif
}

void HIGGS_CALLTYPE FloatVectorOperations::enableFlushToZeroMode ([[maybe_unused]] bool shouldEnable) noexcept
{
  #if HIGGS_USE_SSE_INTRINSICS || (HIGGS_USE_ARM_NEON || (HIGGS_64BIT && HIGGS_ARM))
   #if HIGGS_USE_SSE_INTRINSICS
    intptr_t mask = _MM_FLUSH_ZERO_MASK;
   #else /*HIGGS_USE_ARM_NEON*/
    intptr_t mask = (1 << 24 /* FZ */);
   #endif
    setFpStatusRegister ((getFpStatusRegister() & (~mask)) | (shouldEnable ? mask : 0));
  #else
   #if ! (defined (HIGGS_INTEL) || defined (HIGGS_ARM))
    jassertfalse; // No support for flush to zero mode on your platform
   #endif
  #endif
}

void HIGGS_CALLTYPE FloatVectorOperations::disableDenormalisedNumberSupport ([[maybe_unused]] bool shouldDisable) noexcept
{
  #if HIGGS_USE_SSE_INTRINSICS || (HIGGS_USE_ARM_NEON || (HIGGS_64BIT && HIGGS_ARM))
   #if HIGGS_USE_SSE_INTRINSICS
    intptr_t mask = 0x8040;
   #else /*HIGGS_USE_ARM_NEON*/
    intptr_t mask = (1 << 24 /* FZ */);
   #endif

    setFpStatusRegister ((getFpStatusRegister() & (~mask)) | (shouldDisable ? mask : 0));
  #else

   #if ! (defined (HIGGS_INTEL) || defined (HIGGS_ARM))
    jassertfalse; // No support for disable denormals mode on your platform
   #endif
  #endif
}

bool HIGGS_CALLTYPE FloatVectorOperations::areDenormalsDisabled() noexcept
{
  #if HIGGS_USE_SSE_INTRINSICS || (HIGGS_USE_ARM_NEON || (HIGGS_64BIT && HIGGS_ARM))
   #if HIGGS_USE_SSE_INTRINSICS
    intptr_t mask = 0x8040;
   #else /*HIGGS_USE_ARM_NEON*/
    intptr_t mask = (1 << 24 /* FZ */);
   #endif

    return ((getFpStatusRegister() & mask) == mask);
  #else
    return false;
  #endif
}

ScopedNoDenormals::ScopedNoDenormals() noexcept
{
  #if HIGGS_USE_SSE_INTRINSICS || (HIGGS_USE_ARM_NEON || (HIGGS_64BIT && HIGGS_ARM))
   #if HIGGS_USE_SSE_INTRINSICS
    intptr_t mask = 0x8040;
   #else /*HIGGS_USE_ARM_NEON*/
    intptr_t mask = (1 << 24 /* FZ */);
   #endif

    fpsr = FloatVectorOperations::getFpStatusRegister();
    FloatVectorOperations::setFpStatusRegister (fpsr | mask);
  #endif
}

ScopedNoDenormals::~ScopedNoDenormals() noexcept
{
   #if HIGGS_USE_SSE_INTRINSICS || (HIGGS_USE_ARM_NEON || (HIGGS_64BIT && HIGGS_ARM))
    FloatVectorOperations::setFpStatusRegister (fpsr);
   #endif
}


//==============================================================================
//==============================================================================
#if HIGGS_UNIT_TESTS

class FloatVectorOperationsTests final : public UnitTest
{
public:
    FloatVectorOperationsTests()
        : UnitTest ("FloatVectorOperations", UnitTestCategories::audio)
    {}

    template <typename ValueType>
    struct TestRunner
    {
        static void runTest (UnitTest& u, Random random)
        {
            const int range = random.nextBool() ? 500 : 10;
            const int num = random.nextInt (range) + 1;

            HeapBlock<ValueType> buffer1 (num + 16), buffer2 (num + 16);
            HeapBlock<int> buffer3 (num + 16, true);

           #if HIGGS_ARM
            ValueType* const data1 = buffer1;
            ValueType* const data2 = buffer2;
            int* const int1 = buffer3;
           #else
            // These tests deliberately operate on misaligned memory and will be flagged up by
            // checks for undefined behavior!
            ValueType* const data1 = addBytesToPointer (buffer1.get(), random.nextInt (16));
            ValueType* const data2 = addBytesToPointer (buffer2.get(), random.nextInt (16));
            int* const int1 = addBytesToPointer (buffer3.get(), random.nextInt (16));
           #endif

            fillRandomly (random, data1, num);
            fillRandomly (random, data2, num);

            Range<ValueType> minMax1 (FloatVectorOperations::findMinAndMax (data1, num));
            Range<ValueType> minMax2 (Range<ValueType>::findMinAndMax (data1, num));
            u.expect (minMax1 == minMax2);

            u.expect (valuesMatch (FloatVectorOperations::findMinimum (data1, num), higgs::findMinimum (data1, num)));
            u.expect (valuesMatch (FloatVectorOperations::findMaximum (data1, num), higgs::findMaximum (data1, num)));

            u.expect (valuesMatch (FloatVectorOperations::findMinimum (data2, num), higgs::findMinimum (data2, num)));
            u.expect (valuesMatch (FloatVectorOperations::findMaximum (data2, num), higgs::findMaximum (data2, num)));

            FloatVectorOperations::clear (data1, num);
            u.expect (areAllValuesEqual (data1, num, 0));

            FloatVectorOperations::fill (data1, (ValueType) 2, num);
            u.expect (areAllValuesEqual (data1, num, (ValueType) 2));

            FloatVectorOperations::add (data1, (ValueType) 2, num);
            u.expect (areAllValuesEqual (data1, num, (ValueType) 4));

            FloatVectorOperations::copy (data2, data1, num);
            u.expect (areAllValuesEqual (data2, num, (ValueType) 4));

            FloatVectorOperations::add (data2, data1, num);
            u.expect (areAllValuesEqual (data2, num, (ValueType) 8));

            FloatVectorOperations::copyWithMultiply (data2, data1, (ValueType) 4, num);
            u.expect (areAllValuesEqual (data2, num, (ValueType) 16));

            FloatVectorOperations::addWithMultiply (data2, data1, (ValueType) 4, num);
            u.expect (areAllValuesEqual (data2, num, (ValueType) 32));

            FloatVectorOperations::multiply (data1, (ValueType) 2, num);
            u.expect (areAllValuesEqual (data1, num, (ValueType) 8));

            FloatVectorOperations::multiply (data1, data2, num);
            u.expect (areAllValuesEqual (data1, num, (ValueType) 256));

            FloatVectorOperations::negate (data2, data1, num);
            u.expect (areAllValuesEqual (data2, num, (ValueType) -256));

            FloatVectorOperations::subtract (data1, data2, num);
            u.expect (areAllValuesEqual (data1, num, (ValueType) 512));

            FloatVectorOperations::abs (data1, data2, num);
            u.expect (areAllValuesEqual (data1, num, (ValueType) 256));

            FloatVectorOperations::abs (data2, data1, num);
            u.expect (areAllValuesEqual (data2, num, (ValueType) 256));

            fillRandomly (random, int1, num);
            doConversionTest (u, data1, data2, int1, num);

            FloatVectorOperations::fill (data1, (ValueType) 2, num);
            FloatVectorOperations::fill (data2, (ValueType) 3, num);
            FloatVectorOperations::addWithMultiply (data1, data1, data2, num);
            u.expect (areAllValuesEqual (data1, num, (ValueType) 8));
        }

        static void doConversionTest (UnitTest& u, float* data1, float* data2, int* const int1, int num)
        {
            FloatVectorOperations::convertFixedToFloat (data1, int1, 2.0f, num);
            convertFixed (data2, int1, 2.0f, num);
            u.expect (buffersMatch (data1, data2, num));
        }

        static void doConversionTest (UnitTest&, double*, double*, int*, int) {}

        static void fillRandomly (Random& random, ValueType* d, int num)
        {
            while (--num >= 0)
                *d++ = (ValueType) (random.nextDouble() * 1000.0);
        }

        static void fillRandomly (Random& random, int* d, int num)
        {
            while (--num >= 0)
                *d++ = random.nextInt();
        }

        static void convertFixed (float* d, const int* s, ValueType multiplier, int num)
        {
            while (--num >= 0)
                *d++ = (float) *s++ * multiplier;
        }

        static bool areAllValuesEqual (const ValueType* d, int num, ValueType target)
        {
            while (--num >= 0)
                if (! exactlyEqual (*d++, target))
                    return false;

            return true;
        }

        static bool buffersMatch (const ValueType* d1, const ValueType* d2, int num)
        {
            while (--num >= 0)
                if (! valuesMatch (*d1++, *d2++))
                    return false;

            return true;
        }

        static bool valuesMatch (ValueType v1, ValueType v2)
        {
            return std::abs (v1 - v2) < std::numeric_limits<ValueType>::epsilon();
        }
    };

    void runTest() override
    {
        beginTest ("FloatVectorOperations");

        for (int i = 1000; --i >= 0;)
        {
            TestRunner<float>::runTest (*this, getRandom());
            TestRunner<double>::runTest (*this, getRandom());
        }
    }
};

static FloatVectorOperationsTests vectorOpTests;

#endif

} // namespace higgs
