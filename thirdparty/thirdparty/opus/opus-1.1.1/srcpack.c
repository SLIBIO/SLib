#ifdef _MSC_VER
#pragma warning(disable : 4996)
#endif

#define HAVE_CONFIG_H
#include "src/config.h"

#include "src/src/analysis.c"
#include "src/src/mlp.c"
#include "src/src/mlp_data.c"
#include "src/src/opus.c"
#include "src/src/opus_decoder.c"
#include "src/src/opus_encoder.c"
#include "src/src/opus_multistream.c"
#include "src/src/opus_multistream_decoder.c"
#undef MAX_PULSES
#include "src/src/opus_multistream_encoder.c"
#include "src/src/repacketizer.c"

#include "src/silk/A2NLSF.c"
#include "src/silk/ana_filt_bank_1.c"
#include "src/silk/biquad_alt.c"
#include "src/silk/bwexpander.c"
#include "src/silk/bwexpander_32.c"
#include "src/silk/check_control_input.c"
#include "src/silk/CNG.c"
#include "src/silk/code_signs.c"
#include "src/silk/control_audio_bandwidth.c"
#undef  PI 
#include "src/silk/control_codec.c"
#include "src/silk/control_SNR.c"
#include "src/silk/debug.c"
#include "src/silk/dec_API.c"
#include "src/silk/decode_core.c"
#include "src/silk/decode_frame.c"
#include "src/silk/decode_indices.c"
#include "src/silk/decode_parameters.c"
#include "src/silk/decode_pitch.c"
#include "src/silk/decode_pulses.c"
#include "src/silk/decoder_set_fs.c"
#include "src/silk/enc_API.c"
#include "src/silk/encode_indices.c"
#include "src/silk/encode_pulses.c"
#include "src/silk/gain_quant.c"
#include "src/silk/HP_variable_cutoff.c"
#include "src/silk/init_decoder.c"
#include "src/silk/init_encoder.c"
#include "src/silk/inner_prod_aligned.c"
#include "src/silk/interpolate.c"
#include "src/silk/lin2log.c"
#include "src/silk/log2lin.c"
#include "src/silk/LP_variable_cutoff.c"
#include "src/silk/LPC_analysis_filter.c"
#include "src/silk/LPC_inv_pred_gain.c"
#include "src/silk/NLSF_decode.c"
#include "src/silk/NLSF_del_dec_quant.c"
#include "src/silk/NLSF_encode.c"
#include "src/silk/NLSF_stabilize.c"
#include "src/silk/NLSF_unpack.c"
#include "src/silk/NLSF_VQ.c"
#include "src/silk/NLSF_VQ_weights_laroia.c"
#undef QA
#include "src/silk/NLSF2A.c"
#include "src/silk/NSQ.c"
#include "src/silk/NSQ_del_dec.c"
#include "src/silk/pitch_est_tables.c"
#include "src/silk/PLC.c"
#include "src/silk/process_NLSFs.c"
#include "src/silk/quant_LTP_gains.c"
#include "src/silk/resampler.c"
#include "src/silk/resampler_down2.c"
#include "src/silk/resampler_down2_3.c"
#include "src/silk/resampler_private_AR2.c"
#include "src/silk/resampler_private_down_FIR.c"
#include "src/silk/resampler_private_IIR_FIR.c"
#include "src/silk/resampler_private_up2_HQ.c"
#include "src/silk/resampler_rom.c"
#include "src/silk/shell_coder.c"
#include "src/silk/sigm_Q15.c"
#include "src/silk/sort.c"
#include "src/silk/stereo_decode_pred.c"
#include "src/silk/stereo_encode_pred.c"
#include "src/silk/stereo_find_predictor.c"
#include "src/silk/stereo_LR_to_MS.c"
#include "src/silk/stereo_MS_to_LR.c"
#include "src/silk/stereo_quant_pred.c"
#include "src/silk/sum_sqr_shift.c"
#include "src/silk/table_LSF_cos.c"
#include "src/silk/tables_gain.c"
#include "src/silk/tables_LTP.c"
#include "src/silk/tables_NLSF_CB_NB_MB.c"
#include "src/silk/tables_NLSF_CB_WB.c"
#include "src/silk/tables_other.c"
#include "src/silk/tables_pitch_lag.c"
#include "src/silk/tables_pulses_per_block.c"
#include "src/silk/VAD.c"
#include "src/silk/VQ_WMat_EC.c"

#include "src/silk/fixed/apply_sine_window_FIX.c"
#include "src/silk/fixed/autocorr_FIX.c"
#undef QA
#include "src/silk/fixed/burg_modified_FIX.c"
#include "src/silk/fixed/corrMatrix_FIX.c"
#include "src/silk/fixed/encode_frame_FIX.c"
#include "src/silk/fixed/find_LPC_FIX.c"
#include "src/silk/fixed/find_LTP_FIX.c"
#include "src/silk/fixed/find_pitch_lags_FIX.c"
#include "src/silk/fixed/find_pred_coefs_FIX.c"
#include "src/silk/fixed/k2a_FIX.c"
#include "src/silk/fixed/k2a_Q16_FIX.c"
#include "src/silk/fixed/LTP_analysis_filter_FIX.c"
#include "src/silk/fixed/LTP_scale_ctrl_FIX.c"
#include "src/silk/fixed/noise_shape_analysis_FIX.c"
#include "src/silk/fixed/pitch_analysis_core_FIX.c"
#include "src/silk/fixed/prefilter_FIX.c"
#include "src/silk/fixed/process_gains_FIX.c"
#include "src/silk/fixed/regularize_correlations_FIX.c"
#include "src/silk/fixed/residual_energy_FIX.c"
#include "src/silk/fixed/residual_energy16_FIX.c"
#include "src/silk/fixed/schur_FIX.c"
#include "src/silk/fixed/schur64_FIX.c"
#include "src/silk/fixed/solve_LS_FIX.c"
#include "src/silk/fixed/vector_ops_FIX.c"
#include "src/silk/fixed/warped_autocorrelation_FIX.c"

#include "src/celt/bands.c"
#include "src/celt/celt.c"
#undef OPUS_CUSTOM_H
#undef CELT_H
#define AnalysisInfo AnalysisInfo__celt_decode_c
#define trim_icdf trim_icdf__celt_decode_c
#define spread_icdf spread_icdf__celt_decode_c
#define tapset_icdf tapset_icdf__celt_decode_c
#include "src/celt/celt_decoder.c"

#undef OPUS_CUSTOM_H
#undef CELT_H
#undef AnalysisInfo
#undef trim_icdf
#undef spread_icdf
#undef tapset_icdf
#define AnalysisInfo AnalysisInfo__celt_encode_c
#define trim_icdf trim_icdf__celt_encode_c
#define spread_icdf spread_icdf__celt_encode_c
#define tapset_icdf tapset_icdf__celt_encode_c
#include "src/celt/celt_encoder.c"
#include "src/celt/celt_lpc.c"
#include "src/celt/cwrs.c"
#include "src/celt/entcode.c"
#include "src/celt/entdec.c"
#include "src/celt/entenc.c"
#include "src/celt/kiss_fft.c"
#include "src/celt/laplace.c"
#include "src/celt/mathops.c"
#include "src/celt/mdct.c"
#include "src/celt/modes.c"
#include "src/celt/pitch.c"
#include "src/celt/quant_bands.c"
#include "src/celt/rate.c"
#include "src/celt/vq.c"

#if defined(OPUS_X86_MAY_HAVE_SSE)
// silk/x86
#define NSQ_del_dec_struct NSQ_del_dec_struct_sse
#define NSQ_sample_struct NSQ_sample_struct_sse
#define NSQ_sample_pair NSQ_sample_pair_sse
#include "src/silk/x86/NSQ_del_dec_sse.c"
#include "src/silk/x86/NSQ_sse.c"
#define tiltWeights tiltWeights_sse
#include "src/silk/x86/VAD_sse.c"
#include "src/silk/x86/VQ_WMat_EC_sse.c"
#include "src/silk/x86/x86_silk_map.c"

// silk/fixed/x86
#include "src/silk/fixed/x86/burg_modified_FIX_sse.c"
#include "src/silk/fixed/x86/prefilter_FIX_sse.c"
#include "src/silk/fixed/x86/vector_ops_FIX_sse.c"

// celt/x86
#include "src/celt/x86/celt_lpc_sse.c"
#include "src/celt/x86/pitch_sse.c"
#include "src/celt/x86/pitch_sse2.c"
#include "src/celt/x86/pitch_sse4_1.c"
#include "src/celt/x86/x86_celt_map.c"
#include "src/celt/x86/x86cpu.c"

#endif

#if defined(OPUS_ARM_ASM)
// celt/arm
#include "src/celt/arm/arm_celt_map.c"
#include "src/celt/arm/armcpu.c"
#if defined(OPUS_ARM_MAY_HAVE_NEON)
#include "src/celt/arm/celt_neon_intr.c"
#endif
#endif
