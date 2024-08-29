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

#ifdef HIGGS_DSP_H_INCLUDED
 /* When you add this cpp file to your project, you mustn't include it in a file where you've
    already included any other headers - just put it inside a file on its own, possibly with your config
    flags preceding it, but don't include anything else. That also includes avoiding any automatic prefix
    header files that the compiler may be using.
 */
 #error "Incorrect use of HIGGS cpp file"
#endif

#include "higgs_dsp.h"

#include <higgs_audio_formats/higgs_audio_formats.h>

#ifndef HIGGS_USE_VDSP_FRAMEWORK
 #define HIGGS_USE_VDSP_FRAMEWORK 1
#endif

#if (HIGGS_MAC || HIGGS_IOS) && HIGGS_USE_VDSP_FRAMEWORK
 #include <Accelerate/Accelerate.h>
#else
 #undef HIGGS_USE_VDSP_FRAMEWORK
#endif

#if HIGGS_DSP_USE_INTEL_MKL
 #include <mkl_dfti.h>
#endif

#if _IPP_SEQUENTIAL_STATIC || _IPP_SEQUENTIAL_DYNAMIC || _IPP_PARALLEL_STATIC || _IPP_PARALLEL_DYNAMIC
 #include <ippcore.h>
 #include <ipps.h>
 #define HIGGS_IPP_AVAILABLE 1
#endif

#include "processors/higgs_FIRFilter.cpp"
#include "processors/higgs_IIRFilter.cpp"
#include "processors/higgs_FirstOrderTPTFilter.cpp"
#include "processors/higgs_Panner.cpp"
#include "processors/higgs_Oversampling.cpp"
#include "processors/higgs_BallisticsFilter.cpp"
#include "processors/higgs_LinkwitzRileyFilter.cpp"
#include "processors/higgs_DelayLine.cpp"
#include "processors/higgs_DryWetMixer.cpp"
#include "processors/higgs_StateVariableTPTFilter.cpp"
#include "maths/higgs_SpecialFunctions.cpp"
#include "maths/higgs_Matrix.cpp"
#include "maths/higgs_LookupTable.cpp"
#include "frequency/higgs_FFT.cpp"
#include "frequency/higgs_Convolution.cpp"
#include "frequency/higgs_Windowing.cpp"
#include "filter_design/higgs_FilterDesign.cpp"
#include "widgets/higgs_LadderFilter.cpp"
#include "widgets/higgs_Compressor.cpp"
#include "widgets/higgs_NoiseGate.cpp"
#include "widgets/higgs_Limiter.cpp"
#include "widgets/higgs_Phaser.cpp"
#include "widgets/higgs_Chorus.cpp"

#if HIGGS_USE_SIMD
 #if HIGGS_INTEL
  #ifdef __AVX2__
   #include "native/higgs_SIMDNativeOps_avx.cpp"
  #else
   #include "native/higgs_SIMDNativeOps_sse.cpp"
  #endif
 #elif HIGGS_ARM
  #include "native/higgs_SIMDNativeOps_neon.cpp"
 #else
  #error "SIMD register support not implemented for this platform"
 #endif
#endif

#if HIGGS_UNIT_TESTS
 #include "maths/higgs_Matrix_test.cpp"
 #include "maths/higgs_LogRampedValue_test.cpp"

 #if HIGGS_USE_SIMD
  #include "containers/higgs_SIMDRegister_test.cpp"
 #endif

 #include "containers/higgs_AudioBlock_test.cpp"
 #include "containers/higgs_SIMDInterleavingHelpers_test.cpp"
 #include "frequency/higgs_Convolution_test.cpp"
 #include "frequency/higgs_FFT_test.cpp"
 #include "processors/higgs_FIRFilter_test.cpp"
 #include "processors/higgs_ProcessorChain_test.cpp"
#endif
