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


/*******************************************************************************
 The block below describes the properties of this module, and is read by
 the MythosStudio to automatically generate project code that uses it.
 For details about the syntax and how to create or use a module, see the
 Higgs Mythos Module Format.md file.


 BEGIN_HIGGS_MODULE_DECLARATION

  ID:                 higgs_dsp
  vendor:             higgs
  version:            9.0.5
  name:               Higgs Mythos DSP classes
  description:        Classes for audio buffer manipulation, digital audio processing, filtering, oversampling, fast math functions etc.
  website:            https://github.com/jalexiscv/higgs-mythos
  license:            AGPLv3
  minimumCppStandard: 17

  dependencies:       higgs_audio_formats
  OSXFrameworks:      Accelerate
  iOSFrameworks:      Accelerate

 END_HIGGS_MODULE_DECLARATION

*******************************************************************************/


#pragma once

#define HIGGS_DSP_H_INCLUDED

#include <higgs_audio_basics/higgs_audio_basics.h>

#if HIGGS_INTEL

 #if defined (_M_X64) || defined (__amd64__)
  #ifndef __SSE2__
   #define __SSE2__
  #endif
 #endif

 #ifndef HIGGS_USE_SIMD
  #define HIGGS_USE_SIMD 1
 #endif

 #if HIGGS_USE_SIMD
  #include <immintrin.h>
 #endif

#elif HIGGS_ARM

 #ifndef HIGGS_USE_SIMD
  #if HIGGS_USE_ARM_NEON
   #define HIGGS_USE_SIMD 1
  #else
   #define HIGGS_USE_SIMD 0
  #endif
 #endif

 #if HIGGS_USE_SIMD
  #if HIGGS_WINDOWS
   #if HIGGS_64BIT
    #if ! HIGGS_CLANG
     #include <arm64_neon.h>
    #endif
   #else
    #include <arm_neon.h>
   #endif
  #else
   #include <arm_neon.h>
  #endif
 #endif

#else

 // No SIMD Support
 #ifndef HIGGS_USE_SIMD
  #define HIGGS_USE_SIMD 0
 #endif

#endif

#ifndef HIGGS_VECTOR_CALLTYPE
 // __vectorcall does not work on 64-bit due to internal compiler error in
 // release mode VS2017. Re-enable when Microsoft fixes this
 #if _MSC_VER && HIGGS_USE_SIMD && ! (defined (_M_X64) || defined (__amd64__))
  #define HIGGS_VECTOR_CALLTYPE __vectorcall
 #else
  #define HIGGS_VECTOR_CALLTYPE
 #endif
#endif

#include <complex>


//==============================================================================
/** Config: HIGGS_ASSERTION_FIRFILTER

    When this flag is enabled, an assertion will be generated during the
    execution of DEBUG configurations if you use a FIRFilter class to process
    FIRCoefficients with a size higher than 128, to tell you that's it would be
    more efficient to use the Convolution class instead. It is enabled by
    default, but you may want to disable it if you really want to process such
    a filter in the time domain.
*/
#ifndef HIGGS_ASSERTION_FIRFILTER
 #define HIGGS_ASSERTION_FIRFILTER 1
#endif

/** Config: HIGGS_DSP_USE_INTEL_MKL

    If this flag is set, then HIGGS will use Intel's MKL for HIGGS's FFT and
    convolution classes.

    If you're using the MythosStudio's Visual Studio exporter, you should also set
    the "Use MKL Library (oneAPI)" option in the exporter settings to
    "Sequential" or "Parallel". If you're not using the Visual Studio exporter,
    the folder containing the mkl_dfti.h header must be in your header search
    paths, and you must link against all the necessary MKL libraries.
*/
#ifndef HIGGS_DSP_USE_INTEL_MKL
 #define HIGGS_DSP_USE_INTEL_MKL 0
#endif

/** Config: HIGGS_DSP_USE_SHARED_FFTW

    If this flag is set, then HIGGS will search for the fftw shared libraries
    at runtime and use the library for HIGGS's FFT and convolution classes.

    If the library is not found, then HIGGS's fallback FFT routines will be used.

    This is especially useful on linux as fftw often comes pre-installed on
    popular linux distros.

    You must respect the FFTW license when enabling this option.
*/
 #ifndef HIGGS_DSP_USE_SHARED_FFTW
 #define HIGGS_DSP_USE_SHARED_FFTW 0
#endif

/** Config: HIGGS_DSP_USE_STATIC_FFTW

    If this flag is set, then HIGGS will use the statically linked fftw libraries
    for HIGGS's FFT and convolution classes.

    You must add the fftw header/library folder to the extra header/library search
    paths of your HIGGS project. You also need to add the fftw library itself
    to the extra libraries supplied to your HIGGS project during linking.

    You must respect the FFTW license when enabling this option.
*/
#ifndef HIGGS_DSP_USE_STATIC_FFTW
 #define HIGGS_DSP_USE_STATIC_FFTW 0
#endif

/** Config: HIGGS_DSP_ENABLE_SNAP_TO_ZERO

    Enables code in the dsp module to avoid floating point denormals during the
    processing of some of the dsp module's filters.

    Enabling this will add a slight performance overhead to the DSP module's
    filters and algorithms. If your audio app already disables denormals altogether
    (for example, by using the ScopedNoDenormals class or the
    FloatVectorOperations::disableDenormalisedNumberSupport method), then you
    can safely disable this flag to shave off a few cpu cycles from the DSP module's
    filters and algorithms.
*/
#ifndef HIGGS_DSP_ENABLE_SNAP_TO_ZERO
 #define HIGGS_DSP_ENABLE_SNAP_TO_ZERO 1
#endif


//==============================================================================
#undef Complex  // apparently some C libraries actually define these symbols (!)
#undef Factor
#undef check

namespace higgs::dsp
{

template <typename Type>
using Complex = std::complex<Type>;

template <size_t len, typename T>
using FixedSizeFunction = higgs::FixedSizeFunction<len, T>;

//==============================================================================
namespace util
{
    /** Use this function to prevent denormals on intel CPUs.
        This function will work with both primitives and simple containers.
    */
   #if HIGGS_DSP_ENABLE_SNAP_TO_ZERO
    inline void snapToZero (float&       x) noexcept            { HIGGS_SNAP_TO_ZERO (x); }
    /** @cond */
    inline void snapToZero (double&      x) noexcept            { HIGGS_SNAP_TO_ZERO (x); }
    inline void snapToZero (long double& x) noexcept            { HIGGS_SNAP_TO_ZERO (x); }
    /** @endcond */
   #else
    inline void snapToZero ([[maybe_unused]] float&       x) noexcept            {}
    /** @cond */
    inline void snapToZero ([[maybe_unused]] double&      x) noexcept            {}
    inline void snapToZero ([[maybe_unused]] long double& x) noexcept            {}
    /** @endcond */
   #endif
}

}

//==============================================================================
#if HIGGS_USE_SIMD
 #include "native/higgs_SIMDNativeOps_fallback.h"

 // include the correct native file for this build target CPU
 #if HIGGS_INTEL
  #ifdef __AVX2__
   #include "native/higgs_SIMDNativeOps_avx.h"
  #else
   #include "native/higgs_SIMDNativeOps_sse.h"
  #endif
 #elif HIGGS_ARM
  #include "native/higgs_SIMDNativeOps_neon.h"
 #else
  #error "SIMD register support not implemented for this platform"
 #endif

 #include "containers/higgs_SIMDRegister.h"
 #include "containers/higgs_SIMDRegister_Impl.h"
#endif

#include "maths/higgs_SpecialFunctions.h"
#include "maths/higgs_Matrix.h"
#include "maths/higgs_Phase.h"
#include "maths/higgs_Polynomial.h"
#include "maths/higgs_FastMathApproximations.h"
#include "maths/higgs_LookupTable.h"
#include "maths/higgs_LogRampedValue.h"
#include "containers/higgs_AudioBlock.h"
#include "containers/higgs_SIMDInterleavingHelpers.h"
#include "processors/higgs_ProcessContext.h"
#include "processors/higgs_ProcessorWrapper.h"
#include "processors/higgs_ProcessorChain.h"
#include "processors/higgs_ProcessorDuplicator.h"
#include "processors/higgs_IIRFilter.h"
#include "processors/higgs_IIRFilter_Impl.h"
#include "processors/higgs_FIRFilter.h"
#include "processors/higgs_StateVariableFilter.h"
#include "processors/higgs_FirstOrderTPTFilter.h"
#include "processors/higgs_Panner.h"
#include "processors/higgs_DelayLine.h"
#include "processors/higgs_Oversampling.h"
#include "processors/higgs_BallisticsFilter.h"
#include "processors/higgs_LinkwitzRileyFilter.h"
#include "processors/higgs_DryWetMixer.h"
#include "processors/higgs_StateVariableTPTFilter.h"
#include "frequency/higgs_FFT.h"
#include "frequency/higgs_Convolution.h"
#include "frequency/higgs_Windowing.h"
#include "filter_design/higgs_FilterDesign.h"
#include "widgets/higgs_Reverb.h"
#include "widgets/higgs_Bias.h"
#include "widgets/higgs_Gain.h"
#include "widgets/higgs_WaveShaper.h"
#include "widgets/higgs_Oscillator.h"
#include "widgets/higgs_LadderFilter.h"
#include "widgets/higgs_Compressor.h"
#include "widgets/higgs_NoiseGate.h"
#include "widgets/higgs_Limiter.h"
#include "widgets/higgs_Phaser.h"
#include "widgets/higgs_Chorus.h"
