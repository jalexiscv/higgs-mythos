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

#include "higgs_audio_formats/codecs/opus/higgs_opus_config.h"
#include <higgs_core/system/higgs_CompilerWarnings.h>

#if HIGGS_USE_OPUS && (HIGGS_INCLUDE_OPUS_CODE || ! defined (HIGGS_INCLUDE_OPUS_CODE))

// GCC reports a "compiling without optimization" warning message when building in Debug mode,
// which this define suppresses.
#define OPUS_WILL_BE_SLOW

HIGGS_BEGIN_IGNORE_WARNINGS_GCC_LIKE ("-W#pragma-messages",
                                     "-Wcpp",
                                     "-Wcast-align",
                                     "-Wconditional-uninitialized",
                                     "-Wconversion",
                                     "-Wfloat-equal",
                                     "-Wlanguage-extension-token",
                                     "-Wmissing-prototypes",
                                     "-Wsign-conversion",
                                     "-Wimplicit-fallthrough")
HIGGS_BEGIN_IGNORE_WARNINGS_MSVC (4267 4127 4244 4100 4701 4702 4013 4133 4206 4305 4189 4706 4995 4365 4456 4457 4459 6297 6011 6001 6308 6255 6386 6385 6246 6387 6263 6262 28182 4310 28199 6326)

#include "higgs_audio_formats/codecs/opus/opus/package_version.h"

// opus_custom.h only declares the opus_custom_* functions while these are
// defined, and its include guard makes the first inclusion the one that counts
#define CELT_ENCODER_C
#define CELT_DECODER_C

#include "higgs_audio_formats/codecs/opus/opus/src/opus.c"
#include "higgs_audio_formats/codecs/opus/opus/src/opus_decoder.c"
#include "higgs_audio_formats/codecs/opus/opus/src/opus_encoder.c"
#include "higgs_audio_formats/codecs/opus/opus/src/extensions.c"
#include "higgs_audio_formats/codecs/opus/opus/src/opus_multistream.c"
#include "higgs_audio_formats/codecs/opus/opus/src/opus_multistream_encoder.c"
#include "higgs_audio_formats/codecs/opus/opus/src/opus_multistream_decoder.c"
#include "higgs_audio_formats/codecs/opus/opus/src/repacketizer.c"
#include "higgs_audio_formats/codecs/opus/opus/src/opus_projection_encoder.c"
#include "higgs_audio_formats/codecs/opus/opus/src/opus_projection_decoder.c"
#include "higgs_audio_formats/codecs/opus/opus/src/mapping_matrix.c"
#include "higgs_audio_formats/codecs/opus/opus/src/analysis.c"
#include "higgs_audio_formats/codecs/opus/opus/src/mlp.c"
#include "higgs_audio_formats/codecs/opus/opus/src/mlp_data.c"
#include "higgs_audio_formats/codecs/opus/opus/celt/bands.c"
#include "higgs_audio_formats/codecs/opus/opus/celt/celt.c"
#include "higgs_audio_formats/codecs/opus/opus/celt/celt_encoder.c"
#include "higgs_audio_formats/codecs/opus/opus/celt/celt_decoder.c"
#include "higgs_audio_formats/codecs/opus/opus/celt/cwrs.c"
#include "higgs_audio_formats/codecs/opus/opus/celt/entcode.c"
#include "higgs_audio_formats/codecs/opus/opus/celt/entdec.c"
#include "higgs_audio_formats/codecs/opus/opus/celt/entenc.c"
#include "higgs_audio_formats/codecs/opus/opus/celt/kiss_fft.c"
#include "higgs_audio_formats/codecs/opus/opus/celt/laplace.c"
#include "higgs_audio_formats/codecs/opus/opus/celt/mathops.c"
#include "higgs_audio_formats/codecs/opus/opus/celt/mdct.c"
#include "higgs_audio_formats/codecs/opus/opus/celt/modes.c"
#include "higgs_audio_formats/codecs/opus/opus/celt/pitch.c"
#include "higgs_audio_formats/codecs/opus/opus/celt/celt_lpc.c"
#include "higgs_audio_formats/codecs/opus/opus/celt/quant_bands.c"
#include "higgs_audio_formats/codecs/opus/opus/celt/rate.c"
#include "higgs_audio_formats/codecs/opus/opus/celt/vq.c"
#include "higgs_audio_formats/codecs/opus/opus/silk/CNG.c"
#include "higgs_audio_formats/codecs/opus/opus/silk/code_signs.c"
#include "higgs_audio_formats/codecs/opus/opus/silk/init_decoder.c"
#include "higgs_audio_formats/codecs/opus/opus/silk/decode_core.c"
#include "higgs_audio_formats/codecs/opus/opus/silk/decode_frame.c"
#include "higgs_audio_formats/codecs/opus/opus/silk/decode_parameters.c"
#include "higgs_audio_formats/codecs/opus/opus/silk/decode_indices.c"
#include "higgs_audio_formats/codecs/opus/opus/silk/decode_pulses.c"
#include "higgs_audio_formats/codecs/opus/opus/silk/decoder_set_fs.c"
#include "higgs_audio_formats/codecs/opus/opus/silk/dec_API.c"

// PI is a double in celt/mathops.h and a float in silk/float/SigProc_FLP.h
#undef PI
#include "higgs_audio_formats/codecs/opus/opus/silk/enc_API.c"
#include "higgs_audio_formats/codecs/opus/opus/silk/encode_indices.c"
#include "higgs_audio_formats/codecs/opus/opus/silk/encode_pulses.c"
#include "higgs_audio_formats/codecs/opus/opus/silk/gain_quant.c"
#include "higgs_audio_formats/codecs/opus/opus/silk/interpolate.c"
#include "higgs_audio_formats/codecs/opus/opus/silk/LP_variable_cutoff.c"
#include "higgs_audio_formats/codecs/opus/opus/silk/NLSF_decode.c"
#include "higgs_audio_formats/codecs/opus/opus/silk/NSQ.c"
#include "higgs_audio_formats/codecs/opus/opus/silk/NSQ_del_dec.c"
#include "higgs_audio_formats/codecs/opus/opus/silk/PLC.c"
#include "higgs_audio_formats/codecs/opus/opus/silk/shell_coder.c"
#include "higgs_audio_formats/codecs/opus/opus/silk/tables_gain.c"
#include "higgs_audio_formats/codecs/opus/opus/silk/tables_LTP.c"
#include "higgs_audio_formats/codecs/opus/opus/silk/tables_NLSF_CB_NB_MB.c"
#include "higgs_audio_formats/codecs/opus/opus/silk/tables_NLSF_CB_WB.c"
#include "higgs_audio_formats/codecs/opus/opus/silk/tables_other.c"
#include "higgs_audio_formats/codecs/opus/opus/silk/tables_pitch_lag.c"
#include "higgs_audio_formats/codecs/opus/opus/silk/tables_pulses_per_block.c"
#include "higgs_audio_formats/codecs/opus/opus/silk/VAD.c"
#include "higgs_audio_formats/codecs/opus/opus/silk/control_audio_bandwidth.c"
#include "higgs_audio_formats/codecs/opus/opus/silk/quant_LTP_gains.c"
#include "higgs_audio_formats/codecs/opus/opus/silk/VQ_WMat_EC.c"
#include "higgs_audio_formats/codecs/opus/opus/silk/HP_variable_cutoff.c"
#include "higgs_audio_formats/codecs/opus/opus/silk/NLSF_encode.c"
#include "higgs_audio_formats/codecs/opus/opus/silk/NLSF_VQ.c"
#include "higgs_audio_formats/codecs/opus/opus/silk/NLSF_unpack.c"
#include "higgs_audio_formats/codecs/opus/opus/silk/NLSF_del_dec_quant.c"
#include "higgs_audio_formats/codecs/opus/opus/silk/process_NLSFs.c"
#include "higgs_audio_formats/codecs/opus/opus/silk/stereo_LR_to_MS.c"
#include "higgs_audio_formats/codecs/opus/opus/silk/stereo_MS_to_LR.c"
#include "higgs_audio_formats/codecs/opus/opus/silk/check_control_input.c"
#include "higgs_audio_formats/codecs/opus/opus/silk/control_SNR.c"
#include "higgs_audio_formats/codecs/opus/opus/silk/init_encoder.c"
#include "higgs_audio_formats/codecs/opus/opus/silk/control_codec.c"
#include "higgs_audio_formats/codecs/opus/opus/silk/A2NLSF.c"
#include "higgs_audio_formats/codecs/opus/opus/silk/ana_filt_bank_1.c"
#include "higgs_audio_formats/codecs/opus/opus/silk/biquad_alt.c"
#include "higgs_audio_formats/codecs/opus/opus/silk/bwexpander_32.c"
#include "higgs_audio_formats/codecs/opus/opus/silk/bwexpander.c"
#include "higgs_audio_formats/codecs/opus/opus/silk/debug.c"
#include "higgs_audio_formats/codecs/opus/opus/silk/decode_pitch.c"
#include "higgs_audio_formats/codecs/opus/opus/silk/inner_prod_aligned.c"
#include "higgs_audio_formats/codecs/opus/opus/silk/lin2log.c"
#include "higgs_audio_formats/codecs/opus/opus/silk/log2lin.c"
#include "higgs_audio_formats/codecs/opus/opus/silk/LPC_analysis_filter.c"
#include "higgs_audio_formats/codecs/opus/opus/silk/LPC_inv_pred_gain.c"
#include "higgs_audio_formats/codecs/opus/opus/silk/table_LSF_cos.c"

// QA is 24 in silk/LPC_inv_pred_gain.c and 16 in silk/NLSF2A.c
#undef QA
#include "higgs_audio_formats/codecs/opus/opus/silk/NLSF2A.c"
#include "higgs_audio_formats/codecs/opus/opus/silk/NLSF_stabilize.c"
#include "higgs_audio_formats/codecs/opus/opus/silk/NLSF_VQ_weights_laroia.c"
#include "higgs_audio_formats/codecs/opus/opus/silk/pitch_est_tables.c"
#include "higgs_audio_formats/codecs/opus/opus/silk/resampler.c"
#include "higgs_audio_formats/codecs/opus/opus/silk/resampler_down2_3.c"
#include "higgs_audio_formats/codecs/opus/opus/silk/resampler_down2.c"
#include "higgs_audio_formats/codecs/opus/opus/silk/resampler_private_AR2.c"
#include "higgs_audio_formats/codecs/opus/opus/silk/resampler_private_down_FIR.c"
#include "higgs_audio_formats/codecs/opus/opus/silk/resampler_private_IIR_FIR.c"
#include "higgs_audio_formats/codecs/opus/opus/silk/resampler_private_up2_HQ.c"
#include "higgs_audio_formats/codecs/opus/opus/silk/resampler_rom.c"
#include "higgs_audio_formats/codecs/opus/opus/silk/sigm_Q15.c"
#include "higgs_audio_formats/codecs/opus/opus/silk/sort.c"
#include "higgs_audio_formats/codecs/opus/opus/silk/sum_sqr_shift.c"
#include "higgs_audio_formats/codecs/opus/opus/silk/stereo_decode_pred.c"
#include "higgs_audio_formats/codecs/opus/opus/silk/stereo_encode_pred.c"
#include "higgs_audio_formats/codecs/opus/opus/silk/stereo_find_predictor.c"
#include "higgs_audio_formats/codecs/opus/opus/silk/stereo_quant_pred.c"
#include "higgs_audio_formats/codecs/opus/opus/silk/LPC_fit.c"
#include "higgs_audio_formats/codecs/opus/opus/silk/float/apply_sine_window_FLP.c"
#include "higgs_audio_formats/codecs/opus/opus/silk/float/corrMatrix_FLP.c"
#include "higgs_audio_formats/codecs/opus/opus/silk/float/encode_frame_FLP.c"
#include "higgs_audio_formats/codecs/opus/opus/silk/float/find_LPC_FLP.c"
#include "higgs_audio_formats/codecs/opus/opus/silk/float/find_LTP_FLP.c"
#include "higgs_audio_formats/codecs/opus/opus/silk/float/find_pitch_lags_FLP.c"
#include "higgs_audio_formats/codecs/opus/opus/silk/float/find_pred_coefs_FLP.c"
#include "higgs_audio_formats/codecs/opus/opus/silk/float/LPC_analysis_filter_FLP.c"
#include "higgs_audio_formats/codecs/opus/opus/silk/float/LTP_analysis_filter_FLP.c"
#include "higgs_audio_formats/codecs/opus/opus/silk/float/LTP_scale_ctrl_FLP.c"
#include "higgs_audio_formats/codecs/opus/opus/silk/float/noise_shape_analysis_FLP.c"
#include "higgs_audio_formats/codecs/opus/opus/silk/float/process_gains_FLP.c"
#include "higgs_audio_formats/codecs/opus/opus/silk/float/regularize_correlations_FLP.c"
#include "higgs_audio_formats/codecs/opus/opus/silk/float/residual_energy_FLP.c"
#include "higgs_audio_formats/codecs/opus/opus/silk/float/warped_autocorrelation_FLP.c"
#include "higgs_audio_formats/codecs/opus/opus/silk/float/wrappers_FLP.c"
#include "higgs_audio_formats/codecs/opus/opus/silk/float/autocorrelation_FLP.c"
#include "higgs_audio_formats/codecs/opus/opus/silk/float/burg_modified_FLP.c"
#include "higgs_audio_formats/codecs/opus/opus/silk/float/bwexpander_FLP.c"
#include "higgs_audio_formats/codecs/opus/opus/silk/float/energy_FLP.c"
#include "higgs_audio_formats/codecs/opus/opus/silk/float/inner_product_FLP.c"
#include "higgs_audio_formats/codecs/opus/opus/silk/float/k2a_FLP.c"
#include "higgs_audio_formats/codecs/opus/opus/silk/float/LPC_inv_pred_gain_FLP.c"
#include "higgs_audio_formats/codecs/opus/opus/silk/float/pitch_analysis_core_FLP.c"
#include "higgs_audio_formats/codecs/opus/opus/silk/float/scale_copy_vector_FLP.c"
#include "higgs_audio_formats/codecs/opus/opus/silk/float/scale_vector_FLP.c"
#include "higgs_audio_formats/codecs/opus/opus/silk/float/schur_FLP.c"
#include "higgs_audio_formats/codecs/opus/opus/silk/float/sort_FLP.c"

#if OPUS_ARM_PRESUME_NEON_INTR
 #include "higgs_audio_formats/codecs/opus/opus/celt/arm/celt_neon_intr.c"
 #include "higgs_audio_formats/codecs/opus/opus/celt/arm/pitch_neon_intr.c"
 #include "higgs_audio_formats/codecs/opus/opus/silk/arm/biquad_alt_neon_intr.c"
 #undef QA
 #include "higgs_audio_formats/codecs/opus/opus/silk/arm/LPC_inv_pred_gain_neon_intr.c"
 #include "higgs_audio_formats/codecs/opus/opus/silk/arm/NSQ_del_dec_neon_intr.c"
 #include "higgs_audio_formats/codecs/opus/opus/silk/arm/NSQ_neon.c"
#endif

#if OPUS_X86_PRESUME_SSE2
 #include "higgs_audio_formats/codecs/opus/opus/celt/x86/pitch_sse.c"
 #include "higgs_audio_formats/codecs/opus/opus/celt/x86/pitch_sse2.c"
 #include "higgs_audio_formats/codecs/opus/opus/celt/x86/vq_sse2.c"
#endif

HIGGS_END_IGNORE_WARNINGS_MSVC
HIGGS_END_IGNORE_WARNINGS_GCC_LIKE

#else
// To get around 'empty translation unit' errors and 'has no symbols' warnings emitted by libtool
void higgs_audioFormatsOpusPlaceholder (void);
void higgs_audioFormatsOpusPlaceholder (void) {}
#endif
