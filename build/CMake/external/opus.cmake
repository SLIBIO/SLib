set (TARGET_NAME opus)
set (ROOT_DIR "${SLIB_PATH}/external/src/opus")

set (
 SRC_LIST

 "${ROOT_DIR}/src/analysis.c"
 "${ROOT_DIR}/src/mlp.c"
 "${ROOT_DIR}/src/mlp_data.c"
 "${ROOT_DIR}/src/opus.c"
 "${ROOT_DIR}/src/opus_decoder.c"
 "${ROOT_DIR}/src/opus_encoder.c"
 "${ROOT_DIR}/src/opus_multistream.c"
 "${ROOT_DIR}/src/opus_multistream_decoder.c"
 "${ROOT_DIR}/src/opus_multistream_encoder.c"
 "${ROOT_DIR}/src/repacketizer.c"

 "${ROOT_DIR}/silk/A2NLSF.c"
 "${ROOT_DIR}/silk/ana_filt_bank_1.c"
 "${ROOT_DIR}/silk/biquad_alt.c"
 "${ROOT_DIR}/silk/bwexpander.c"
 "${ROOT_DIR}/silk/bwexpander_32.c"
 "${ROOT_DIR}/silk/check_control_input.c"
 "${ROOT_DIR}/silk/CNG.c"
 "${ROOT_DIR}/silk/code_signs.c"
 "${ROOT_DIR}/silk/control_audio_bandwidth.c"
 "${ROOT_DIR}/silk/control_codec.c"
 "${ROOT_DIR}/silk/control_SNR.c"
 "${ROOT_DIR}/silk/debug.c"
 "${ROOT_DIR}/silk/dec_API.c"
 "${ROOT_DIR}/silk/decode_core.c"
 "${ROOT_DIR}/silk/decode_frame.c"
 "${ROOT_DIR}/silk/decode_indices.c"
 "${ROOT_DIR}/silk/decode_parameters.c"
 "${ROOT_DIR}/silk/decode_pitch.c"
 "${ROOT_DIR}/silk/decode_pulses.c"
 "${ROOT_DIR}/silk/decoder_set_fs.c"
 "${ROOT_DIR}/silk/enc_API.c"
 "${ROOT_DIR}/silk/encode_indices.c"
 "${ROOT_DIR}/silk/encode_pulses.c"
 "${ROOT_DIR}/silk/gain_quant.c"
 "${ROOT_DIR}/silk/HP_variable_cutoff.c"
 "${ROOT_DIR}/silk/init_decoder.c"
 "${ROOT_DIR}/silk/init_encoder.c"
 "${ROOT_DIR}/silk/inner_prod_aligned.c"
 "${ROOT_DIR}/silk/interpolate.c"
 "${ROOT_DIR}/silk/lin2log.c"
 "${ROOT_DIR}/silk/log2lin.c"
 "${ROOT_DIR}/silk/LP_variable_cutoff.c"
 "${ROOT_DIR}/silk/LPC_analysis_filter.c"
 "${ROOT_DIR}/silk/LPC_fit.c"
 "${ROOT_DIR}/silk/LPC_inv_pred_gain.c"
 "${ROOT_DIR}/silk/NLSF_decode.c"
 "${ROOT_DIR}/silk/NLSF_del_dec_quant.c"
 "${ROOT_DIR}/silk/NLSF_encode.c"
 "${ROOT_DIR}/silk/NLSF_stabilize.c"
 "${ROOT_DIR}/silk/NLSF_unpack.c"
 "${ROOT_DIR}/silk/NLSF_VQ.c"
 "${ROOT_DIR}/silk/NLSF_VQ_weights_laroia.c"
 "${ROOT_DIR}/silk/NLSF2A.c"
 "${ROOT_DIR}/silk/NSQ.c"
 "${ROOT_DIR}/silk/NSQ_del_dec.c"
 "${ROOT_DIR}/silk/pitch_est_tables.c"
 "${ROOT_DIR}/silk/PLC.c"
 "${ROOT_DIR}/silk/process_NLSFs.c"
 "${ROOT_DIR}/silk/quant_LTP_gains.c"
 "${ROOT_DIR}/silk/resampler.c"
 "${ROOT_DIR}/silk/resampler_down2.c"
 "${ROOT_DIR}/silk/resampler_down2_3.c"
 "${ROOT_DIR}/silk/resampler_private_AR2.c"
 "${ROOT_DIR}/silk/resampler_private_down_FIR.c"
 "${ROOT_DIR}/silk/resampler_private_IIR_FIR.c"
 "${ROOT_DIR}/silk/resampler_private_up2_HQ.c"
 "${ROOT_DIR}/silk/resampler_rom.c"
 "${ROOT_DIR}/silk/shell_coder.c"
 "${ROOT_DIR}/silk/sigm_Q15.c"
 "${ROOT_DIR}/silk/sort.c"
 "${ROOT_DIR}/silk/stereo_decode_pred.c"
 "${ROOT_DIR}/silk/stereo_encode_pred.c"
 "${ROOT_DIR}/silk/stereo_find_predictor.c"
 "${ROOT_DIR}/silk/stereo_LR_to_MS.c"
 "${ROOT_DIR}/silk/stereo_MS_to_LR.c"
 "${ROOT_DIR}/silk/stereo_quant_pred.c"
 "${ROOT_DIR}/silk/sum_sqr_shift.c"
 "${ROOT_DIR}/silk/table_LSF_cos.c"
 "${ROOT_DIR}/silk/tables_gain.c"
 "${ROOT_DIR}/silk/tables_LTP.c"
 "${ROOT_DIR}/silk/tables_NLSF_CB_NB_MB.c"
 "${ROOT_DIR}/silk/tables_NLSF_CB_WB.c"
 "${ROOT_DIR}/silk/tables_other.c"
 "${ROOT_DIR}/silk/tables_pitch_lag.c"
 "${ROOT_DIR}/silk/tables_pulses_per_block.c"
 "${ROOT_DIR}/silk/VAD.c"
 "${ROOT_DIR}/silk/VQ_WMat_EC.c"

 "${ROOT_DIR}/silk/fixed/apply_sine_window_FIX.c"
 "${ROOT_DIR}/silk/fixed/autocorr_FIX.c"
 "${ROOT_DIR}/silk/fixed/burg_modified_FIX.c"
 "${ROOT_DIR}/silk/fixed/corrMatrix_FIX.c"
 "${ROOT_DIR}/silk/fixed/encode_frame_FIX.c"
 "${ROOT_DIR}/silk/fixed/find_LPC_FIX.c"
 "${ROOT_DIR}/silk/fixed/find_LTP_FIX.c"
 "${ROOT_DIR}/silk/fixed/find_pitch_lags_FIX.c"
 "${ROOT_DIR}/silk/fixed/find_pred_coefs_FIX.c"
 "${ROOT_DIR}/silk/fixed/k2a_FIX.c"
 "${ROOT_DIR}/silk/fixed/k2a_Q16_FIX.c"
 "${ROOT_DIR}/silk/fixed/LTP_analysis_filter_FIX.c"
 "${ROOT_DIR}/silk/fixed/LTP_scale_ctrl_FIX.c"
 "${ROOT_DIR}/silk/fixed/noise_shape_analysis_FIX.c"
 "${ROOT_DIR}/silk/fixed/pitch_analysis_core_FIX.c"
 "${ROOT_DIR}/silk/fixed/process_gains_FIX.c"
 "${ROOT_DIR}/silk/fixed/regularize_correlations_FIX.c"
 "${ROOT_DIR}/silk/fixed/residual_energy_FIX.c"
 "${ROOT_DIR}/silk/fixed/residual_energy16_FIX.c"
 "${ROOT_DIR}/silk/fixed/schur_FIX.c"
 "${ROOT_DIR}/silk/fixed/schur64_FIX.c"
 "${ROOT_DIR}/silk/fixed/vector_ops_FIX.c"
 "${ROOT_DIR}/silk/fixed/warped_autocorrelation_FIX.c"

 "${ROOT_DIR}/celt/bands.c"
 "${ROOT_DIR}/celt/celt.c"
 "${ROOT_DIR}/celt/celt_decoder.c"
 "${ROOT_DIR}/celt/celt_encoder.c"
 "${ROOT_DIR}/celt/celt_lpc.c"
 "${ROOT_DIR}/celt/cwrs.c"
 "${ROOT_DIR}/celt/entcode.c"
 "${ROOT_DIR}/celt/entdec.c"
 "${ROOT_DIR}/celt/entenc.c"
 "${ROOT_DIR}/celt/kiss_fft.c"
 "${ROOT_DIR}/celt/laplace.c"
 "${ROOT_DIR}/celt/mathops.c"
 "${ROOT_DIR}/celt/mdct.c"
 "${ROOT_DIR}/celt/modes.c"
 "${ROOT_DIR}/celt/pitch.c"
 "${ROOT_DIR}/celt/quant_bands.c"
 "${ROOT_DIR}/celt/rate.c"
 "${ROOT_DIR}/celt/vq.c"
)

if (SLIB_X86)
 list (
  APPEND SRC_LIST

  "${ROOT_DIR}/silk/x86/NSQ_del_dec_sse.c"
  "${ROOT_DIR}/silk/x86/NSQ_sse.c"
  "${ROOT_DIR}/silk/x86/VAD_sse.c"
  "${ROOT_DIR}/silk/x86/VQ_WMat_EC_sse.c"
  "${ROOT_DIR}/silk/x86/x86_silk_map.c"

  "${ROOT_DIR}/silk/fixed/x86/burg_modified_FIX_sse.c"
  "${ROOT_DIR}/silk/fixed/x86/vector_ops_FIX_sse.c"

  "${ROOT_DIR}/celt/x86/celt_lpc_sse.c"
  "${ROOT_DIR}/celt/x86/pitch_sse.c"
  "${ROOT_DIR}/celt/x86/pitch_sse2.c"
  "${ROOT_DIR}/celt/x86/pitch_sse4_1.c"
  "${ROOT_DIR}/celt/x86/vq_sse2.c"
  "${ROOT_DIR}/celt/x86/x86_celt_map.c"
  "${ROOT_DIR}/celt/x86/x86cpu.c"
 )
endif ()

if (SLIB_ARM)
 list (
  APPEND SRC_LIST

  "${ROOT_DIR}/celt/arm/arm_celt_map.c"
  "${ROOT_DIR}/celt/arm/armcpu.c"
  "${ROOT_DIR}/celt/arm/celt_neon_intr.c"
  "${ROOT_DIR}/celt/arm/pitch_neon_intr.c"

  "${ROOT_DIR}/silk/arm/arm_silk_map.c"
  "${ROOT_DIR}/silk/arm/biquad_alt_neon_intr.c"
  "${ROOT_DIR}/silk/arm/LPC_inv_pred_gain_neon_intr.c"
  "${ROOT_DIR}/silk/arm/NSQ_del_dec_neon_intr.c"
  "${ROOT_DIR}/silk/arm/NSQ_neon.c"

  "${ROOT_DIR}/silk/fixed/arm/warped_autocorrelation_FIX_neon_intr.c"
 )
 if (NOT SLIB_ARM64)
  list (
   APPEND SRC_LIST
   "${ROOT_DIR}/celt/arm/celt_pitch_xcorr_arm-gnu.S"
  )
 endif ()
endif ()

include ("${CMAKE_CURRENT_LIST_DIR}/common.inc.cmake")

target_include_directories (
 ${TARGET_NAME}
 PRIVATE "${ROOT_DIR}"
 PRIVATE "${ROOT_DIR}/include"
 PRIVATE "${ROOT_DIR}/celt"
 PRIVATE "${ROOT_DIR}/silk"
 PRIVATE "${ROOT_DIR}/silk/fixed"
)
target_compile_definitions (
 ${TARGET_NAME}
 PRIVATE HAVE_CONFIG_H
)
if (SLIB_X86)
 target_compile_options (
  ${TARGET_NAME}
  PRIVATE -mavx2
 )
endif()
if (ANDROID AND SLIB_ARM64)
 target_compile_options (
  ${TARGET_NAME}
  PRIVATE -Wno-incompatible-pointer-types
 )
endif ()
