set (TARGET_NAME vpx)
set (ROOT_DIR "${SLIB_PATH}/external/src/libvpx")

set (
 C_SRC_LIST

 "${ROOT_DIR}/vpx_config.c"

 "${ROOT_DIR}/vpx/src/svc_encodeframe.c"
 "${ROOT_DIR}/vpx/src/vpx_codec.c"
 "${ROOT_DIR}/vpx/src/vpx_decoder.c"
 "${ROOT_DIR}/vpx/src/vpx_encoder.c"
 "${ROOT_DIR}/vpx/src/vpx_image.c"

 "${ROOT_DIR}/vp8/vp8_cx_iface.c"
 "${ROOT_DIR}/vp8/vp8_dx_iface.c"

 "${ROOT_DIR}/vp8/common/alloccommon.c"
 "${ROOT_DIR}/vp8/common/blockd.c"
 "${ROOT_DIR}/vp8/common/context.c"
 "${ROOT_DIR}/vp8/common/copy_c.c"
 "${ROOT_DIR}/vp8/common/debugmodes.c"
 "${ROOT_DIR}/vp8/common/dequantize.c"
 "${ROOT_DIR}/vp8/common/entropy.c"
 "${ROOT_DIR}/vp8/common/entropymode.c"
 "${ROOT_DIR}/vp8/common/entropymv.c"
 "${ROOT_DIR}/vp8/common/extend.c"
 "${ROOT_DIR}/vp8/common/filter.c"
 "${ROOT_DIR}/vp8/common/findnearmv.c"
 "${ROOT_DIR}/vp8/common/idct_blk.c"
 "${ROOT_DIR}/vp8/common/idctllm.c"
 "${ROOT_DIR}/vp8/common/loopfilter_filters.c"
 "${ROOT_DIR}/vp8/common/mbpitch.c"
 "${ROOT_DIR}/vp8/common/mfqe.c"
 "${ROOT_DIR}/vp8/common/modecont.c"
 "${ROOT_DIR}/vp8/common/postproc.c"
 "${ROOT_DIR}/vp8/common/quant_common.c"
 "${ROOT_DIR}/vp8/common/reconinter.c"
 "${ROOT_DIR}/vp8/common/reconintra.c"
 "${ROOT_DIR}/vp8/common/reconintra4x4.c"
 "${ROOT_DIR}/vp8/common/rtcd.c"
 "${ROOT_DIR}/vp8/common/setupintrarecon.c"
 "${ROOT_DIR}/vp8/common/swapyv12buffer.c"
 "${ROOT_DIR}/vp8/common/treecoder.c"
 "${ROOT_DIR}/vp8/common/vp8_loopfilter.c"
 "${ROOT_DIR}/vp8/common/vp8_skin_detection.c"
 "${ROOT_DIR}/vp8/common/generic/systemdependent.c"

 "${ROOT_DIR}/vp8/decoder/dboolhuff.c"
 "${ROOT_DIR}/vp8/decoder/decodeframe.c"
 "${ROOT_DIR}/vp8/decoder/decodemv.c"
 "${ROOT_DIR}/vp8/decoder/detokenize.c"
 "${ROOT_DIR}/vp8/decoder/onyxd_if.c"
 "${ROOT_DIR}/vp8/decoder/threading.c"

 "${ROOT_DIR}/vp8/encoder/bitstream.c"
 "${ROOT_DIR}/vp8/encoder/boolhuff.c"
 "${ROOT_DIR}/vp8/encoder/dct.c"
 "${ROOT_DIR}/vp8/encoder/denoising.c"
 "${ROOT_DIR}/vp8/encoder/encodeframe.c"
 "${ROOT_DIR}/vp8/encoder/encodeintra.c"
 "${ROOT_DIR}/vp8/encoder/encodemb.c"
 "${ROOT_DIR}/vp8/encoder/encodemv.c"
 "${ROOT_DIR}/vp8/encoder/ethreading.c"
 "${ROOT_DIR}/vp8/encoder/firstpass.c"
 "${ROOT_DIR}/vp8/encoder/lookahead.c"
 "${ROOT_DIR}/vp8/encoder/mcomp.c"
 "${ROOT_DIR}/vp8/encoder/modecosts.c"
 "${ROOT_DIR}/vp8/encoder/onyx_if.c"
 "${ROOT_DIR}/vp8/encoder/pickinter.c"
 "${ROOT_DIR}/vp8/encoder/picklpf.c"
 "${ROOT_DIR}/vp8/encoder/ratectrl.c"
 "${ROOT_DIR}/vp8/encoder/rdopt.c"
 "${ROOT_DIR}/vp8/encoder/segmentation.c"
 "${ROOT_DIR}/vp8/encoder/temporal_filter.c"
 "${ROOT_DIR}/vp8/encoder/tokenize.c"
 "${ROOT_DIR}/vp8/encoder/treewriter.c"
 "${ROOT_DIR}/vp8/encoder/vp8_quantize.c"

 "${ROOT_DIR}/vp9/vp9_cx_iface.c"
 "${ROOT_DIR}/vp9/vp9_dx_iface.c"

 "${ROOT_DIR}/vp9/common/vp9_alloccommon.c"
 "${ROOT_DIR}/vp9/common/vp9_blockd.c"
 "${ROOT_DIR}/vp9/common/vp9_common_data.c"
 "${ROOT_DIR}/vp9/common/vp9_debugmodes.c"
 "${ROOT_DIR}/vp9/common/vp9_entropy.c"
 "${ROOT_DIR}/vp9/common/vp9_entropymode.c"
 "${ROOT_DIR}/vp9/common/vp9_entropymv.c"
 "${ROOT_DIR}/vp9/common/vp9_filter.c"
 "${ROOT_DIR}/vp9/common/vp9_frame_buffers.c"
 "${ROOT_DIR}/vp9/common/vp9_idct.c"
 "${ROOT_DIR}/vp9/common/vp9_loopfilter.c"
 "${ROOT_DIR}/vp9/common/vp9_mfqe.c"
 "${ROOT_DIR}/vp9/common/vp9_mvref_common.c"
 "${ROOT_DIR}/vp9/common/vp9_postproc.c"
 "${ROOT_DIR}/vp9/common/vp9_pred_common.c"
 "${ROOT_DIR}/vp9/common/vp9_quant_common.c"
 "${ROOT_DIR}/vp9/common/vp9_reconinter.c"
 "${ROOT_DIR}/vp9/common/vp9_reconintra.c"
 "${ROOT_DIR}/vp9/common/vp9_rtcd.c"
 "${ROOT_DIR}/vp9/common/vp9_scale.c"
 "${ROOT_DIR}/vp9/common/vp9_scan.c"
 "${ROOT_DIR}/vp9/common/vp9_seg_common.c"
 "${ROOT_DIR}/vp9/common/vp9_thread_common.c"
 "${ROOT_DIR}/vp9/common/vp9_tile_common.c"

 "${ROOT_DIR}/vp9/decoder/vp9_decodeframe.c"
 "${ROOT_DIR}/vp9/decoder/vp9_decodemv.c"
 "${ROOT_DIR}/vp9/decoder/vp9_decoder.c"
 "${ROOT_DIR}/vp9/decoder/vp9_detokenize.c"
 "${ROOT_DIR}/vp9/decoder/vp9_dsubexp.c"

 "${ROOT_DIR}/vp9/encoder/vp9_alt_ref_aq.c"
 "${ROOT_DIR}/vp9/encoder/vp9_aq_360.c"
 "${ROOT_DIR}/vp9/encoder/vp9_aq_complexity.c"
 "${ROOT_DIR}/vp9/encoder/vp9_aq_cyclicrefresh.c"
 "${ROOT_DIR}/vp9/encoder/vp9_aq_variance.c"
 "${ROOT_DIR}/vp9/encoder/vp9_bitstream.c"
 "${ROOT_DIR}/vp9/encoder/vp9_blockiness.c"
 "${ROOT_DIR}/vp9/encoder/vp9_context_tree.c"
 "${ROOT_DIR}/vp9/encoder/vp9_cost.c"
 "${ROOT_DIR}/vp9/encoder/vp9_dct.c"
 "${ROOT_DIR}/vp9/encoder/vp9_encodeframe.c"
 "${ROOT_DIR}/vp9/encoder/vp9_encodemb.c"
 "${ROOT_DIR}/vp9/encoder/vp9_encodemv.c"
 "${ROOT_DIR}/vp9/encoder/vp9_encoder.c"
 "${ROOT_DIR}/vp9/encoder/vp9_ethread.c"
 "${ROOT_DIR}/vp9/encoder/vp9_extend.c"
 "${ROOT_DIR}/vp9/encoder/vp9_firstpass.c"
 "${ROOT_DIR}/vp9/encoder/vp9_frame_scale.c"
 "${ROOT_DIR}/vp9/encoder/vp9_lookahead.c"
 "${ROOT_DIR}/vp9/encoder/vp9_mbgraph.c"
 "${ROOT_DIR}/vp9/encoder/vp9_mcomp.c"
 "${ROOT_DIR}/vp9/encoder/vp9_multi_thread.c"
 "${ROOT_DIR}/vp9/encoder/vp9_noise_estimate.c"
 "${ROOT_DIR}/vp9/encoder/vp9_picklpf.c"
 "${ROOT_DIR}/vp9/encoder/vp9_pickmode.c"
 "${ROOT_DIR}/vp9/encoder/vp9_quantize.c"
 "${ROOT_DIR}/vp9/encoder/vp9_ratectrl.c"
 "${ROOT_DIR}/vp9/encoder/vp9_rd.c"
 "${ROOT_DIR}/vp9/encoder/vp9_rdopt.c"
 "${ROOT_DIR}/vp9/encoder/vp9_resize.c"
 "${ROOT_DIR}/vp9/encoder/vp9_segmentation.c"
 "${ROOT_DIR}/vp9/encoder/vp9_skin_detection.c"
 "${ROOT_DIR}/vp9/encoder/vp9_speed_features.c"
 "${ROOT_DIR}/vp9/encoder/vp9_subexp.c"
 "${ROOT_DIR}/vp9/encoder/vp9_svc_layercontext.c"
 "${ROOT_DIR}/vp9/encoder/vp9_temporal_filter.c"
 "${ROOT_DIR}/vp9/encoder/vp9_tokenize.c"
 "${ROOT_DIR}/vp9/encoder/vp9_treewriter.c"

 "${ROOT_DIR}/vpx_dsp/add_noise.c"
 "${ROOT_DIR}/vpx_dsp/avg.c"
 "${ROOT_DIR}/vpx_dsp/bitreader.c"
 "${ROOT_DIR}/vpx_dsp/bitreader_buffer.c"
 "${ROOT_DIR}/vpx_dsp/bitwriter.c"
 "${ROOT_DIR}/vpx_dsp/bitwriter_buffer.c"
 "${ROOT_DIR}/vpx_dsp/deblock.c"
 "${ROOT_DIR}/vpx_dsp/fastssim.c"
 "${ROOT_DIR}/vpx_dsp/fwd_txfm.c"
 "${ROOT_DIR}/vpx_dsp/intrapred.c"
 "${ROOT_DIR}/vpx_dsp/inv_txfm.c"
 "${ROOT_DIR}/vpx_dsp/loopfilter.c"
 "${ROOT_DIR}/vpx_dsp/prob.c"
 "${ROOT_DIR}/vpx_dsp/psnr.c"
 "${ROOT_DIR}/vpx_dsp/psnrhvs.c"
 "${ROOT_DIR}/vpx_dsp/quantize.c"
 "${ROOT_DIR}/vpx_dsp/sad.c"
 "${ROOT_DIR}/vpx_dsp/skin_detection.c"
 "${ROOT_DIR}/vpx_dsp/subtract.c"
 "${ROOT_DIR}/vpx_dsp/sum_squares.c"
 "${ROOT_DIR}/vpx_dsp/variance.c"
 "${ROOT_DIR}/vpx_dsp/vpx_convolve.c"
 "${ROOT_DIR}/vpx_dsp/vpx_dsp_rtcd.c"

 "${ROOT_DIR}/vpx_mem/vpx_mem.c"

 "${ROOT_DIR}/vpx_scale/vpx_scale_rtcd.c"
 "${ROOT_DIR}/vpx_scale/generic/gen_scalers.c"
 "${ROOT_DIR}/vpx_scale/generic/vpx_scale.c"
 "${ROOT_DIR}/vpx_scale/generic/yv12config.c"
 "${ROOT_DIR}/vpx_scale/generic/yv12extend.c"

 "${ROOT_DIR}/vpx_util/vpx_thread.c"
 "${ROOT_DIR}/vpx_util/vpx_write_yuv_frame.c"
)

if (SLIB_X86)
 list (
  APPEND C_SRC_LIST
  
  "${ROOT_DIR}/vp8/common/x86/filter_x86.c"
  "${ROOT_DIR}/vp8/common/x86/idct_blk_mmx.c"
  "${ROOT_DIR}/vp8/common/x86/idct_blk_sse2.c"
  "${ROOT_DIR}/vp8/common/x86/loopfilter_x86.c"
  "${ROOT_DIR}/vp8/common/x86/vp8_asm_stubs.c"

  "${ROOT_DIR}/vp8/encoder/x86/denoising_sse2.c"
  "${ROOT_DIR}/vp8/encoder/x86/quantize_sse4.c"
  "${ROOT_DIR}/vp8/encoder/x86/vp8_enc_stubs_sse2.c"
  "${ROOT_DIR}/vp8/encoder/x86/vp8_quantize_sse2.c"
  "${ROOT_DIR}/vp8/encoder/x86/vp8_quantize_ssse3.c"

  "${ROOT_DIR}/vp9/common/x86/vp9_idct_intrin_sse2.c"

  "${ROOT_DIR}/vp9/encoder/x86/temporal_filter_sse4.c"
  "${ROOT_DIR}/vp9/encoder/x86/vp9_dct_intrin_sse2.c"
  "${ROOT_DIR}/vp9/encoder/x86/vp9_dct_ssse3.c"
  "${ROOT_DIR}/vp9/encoder/x86/vp9_diamond_search_sad_avx.c"
  "${ROOT_DIR}/vp9/encoder/x86/vp9_error_avx2.c"
  "${ROOT_DIR}/vp9/encoder/x86/vp9_frame_scale_ssse3.c"
  "${ROOT_DIR}/vp9/encoder/x86/vp9_quantize_sse2.c"

  "${ROOT_DIR}/vpx_dsp/x86/avg_intrin_avx2.c"
  "${ROOT_DIR}/vpx_dsp/x86/avg_intrin_sse2.c"
  "${ROOT_DIR}/vpx_dsp/x86/avg_pred_sse2.c"
  "${ROOT_DIR}/vpx_dsp/x86/fwd_txfm_avx2.c"
  "${ROOT_DIR}/vpx_dsp/x86/fwd_txfm_sse2.c"
  "${ROOT_DIR}/vpx_dsp/x86/inv_txfm_sse2.c"
  "${ROOT_DIR}/vpx_dsp/x86/inv_txfm_ssse3.c"
  "${ROOT_DIR}/vpx_dsp/x86/loopfilter_avx2.c"
  "${ROOT_DIR}/vpx_dsp/x86/loopfilter_sse2.c"
  "${ROOT_DIR}/vpx_dsp/x86/quantize_avx.c"
  "${ROOT_DIR}/vpx_dsp/x86/quantize_sse2.c"
  "${ROOT_DIR}/vpx_dsp/x86/quantize_ssse3.c"
  "${ROOT_DIR}/vpx_dsp/x86/sad_avx2.c"
  "${ROOT_DIR}/vpx_dsp/x86/sad4d_avx2.c"
  "${ROOT_DIR}/vpx_dsp/x86/sum_squares_sse2.c"
  "${ROOT_DIR}/vpx_dsp/x86/variance_avx2.c"
  "${ROOT_DIR}/vpx_dsp/x86/variance_sse2.c"
  "${ROOT_DIR}/vpx_dsp/x86/vpx_asm_stubs.c"
  "${ROOT_DIR}/vpx_dsp/x86/vpx_subpixel_8t_intrin_avx2.c"
  "${ROOT_DIR}/vpx_dsp/x86/vpx_subpixel_8t_intrin_ssse3.c"
 )
 set (
  ASM_SRC_LIST

  "${ROOT_DIR}/vp8/common/x86/copy_sse2.asm"
  "${ROOT_DIR}/vp8/common/x86/copy_sse3.asm"
  "${ROOT_DIR}/vp8/common/x86/dequantize_mmx.asm"
  "${ROOT_DIR}/vp8/common/x86/idctllm_mmx.asm"
  "${ROOT_DIR}/vp8/common/x86/idctllm_sse2.asm"
  "${ROOT_DIR}/vp8/common/x86/iwalsh_sse2.asm"
  "${ROOT_DIR}/vp8/common/x86/loopfilter_sse2.asm"
  "${ROOT_DIR}/vp8/common/x86/mfqe_sse2.asm"
  "${ROOT_DIR}/vp8/common/x86/recon_mmx.asm"
  "${ROOT_DIR}/vp8/common/x86/recon_sse2.asm"
  "${ROOT_DIR}/vp8/common/x86/subpixel_mmx.asm"
  "${ROOT_DIR}/vp8/common/x86/subpixel_sse2.asm"
  "${ROOT_DIR}/vp8/common/x86/subpixel_ssse3.asm"

  "${ROOT_DIR}/vp8/encoder/x86/dct_sse2.asm"
  "${ROOT_DIR}/vp8/encoder/x86/encodeopt.asm"
  "${ROOT_DIR}/vp8/encoder/x86/fwalsh_sse2.asm"
  "${ROOT_DIR}/vp8/encoder/x86/temporal_filter_apply_sse2.asm"

  "${ROOT_DIR}/vp9/common/x86/vp9_mfqe_sse2.asm"

  "${ROOT_DIR}/vp9/encoder/x86/vp9_dct_sse2.asm"
  "${ROOT_DIR}/vp9/encoder/x86/vp9_error_sse2.asm"

  "${ROOT_DIR}/vpx_dsp/x86/add_noise_sse2.asm"
  "${ROOT_DIR}/vpx_dsp/x86/bitdepth_conversion_sse2.asm"
  "${ROOT_DIR}/vpx_dsp/x86/deblock_sse2.asm"
  "${ROOT_DIR}/vpx_dsp/x86/intrapred_sse2.asm"
  "${ROOT_DIR}/vpx_dsp/x86/intrapred_ssse3.asm"
  "${ROOT_DIR}/vpx_dsp/x86/inv_wht_sse2.asm"
  "${ROOT_DIR}/vpx_dsp/x86/sad4d_sse2.asm"
  "${ROOT_DIR}/vpx_dsp/x86/sad_sse2.asm"
  "${ROOT_DIR}/vpx_dsp/x86/sad_sse3.asm"
  "${ROOT_DIR}/vpx_dsp/x86/sad_sse4.asm"
  "${ROOT_DIR}/vpx_dsp/x86/sad_ssse3.asm"
  "${ROOT_DIR}/vpx_dsp/x86/subpel_variance_sse2.asm"
  "${ROOT_DIR}/vpx_dsp/x86/subtract_sse2.asm"
  "${ROOT_DIR}/vpx_dsp/x86/vpx_convolve_copy_sse2.asm"
  "${ROOT_DIR}/vpx_dsp/x86/vpx_high_subpixel_8t_sse2.asm"
  "${ROOT_DIR}/vpx_dsp/x86/vpx_high_subpixel_bilinear_sse2.asm"
  "${ROOT_DIR}/vpx_dsp/x86/vpx_subpixel_8t_sse2.asm"
  "${ROOT_DIR}/vpx_dsp/x86/vpx_subpixel_8t_ssse3.asm"
  "${ROOT_DIR}/vpx_dsp/x86/vpx_subpixel_bilinear_sse2.asm"
  "${ROOT_DIR}/vpx_dsp/x86/vpx_subpixel_bilinear_ssse3.asm"

  "${ROOT_DIR}/vpx_ports/emms.asm"
 )
 if (SLIB_X86_64)
  list (
   APPEND ASM_SRC_LIST
   "${ROOT_DIR}/vp8/common/x86/loopfilter_block_sse2_x86_64.asm"
   "${ROOT_DIR}/vp9/encoder/x86/vp9_quantize_ssse3_x86_64.asm"
   "${ROOT_DIR}/vpx_dsp/x86/avg_ssse3_x86_64.asm"
   "${ROOT_DIR}/vpx_dsp/x86/fwd_txfm_ssse3_x86_64.asm"
   "${ROOT_DIR}/vpx_dsp/x86/ssim_opt_x86_64.asm"
  )
 endif ()
endif ()

if (SLIB_ARM)
 list (
  APPEND C_SRC_LIST

  "${ROOT_DIR}/vpx_ports/arm_cpudetect.c"

  "${ROOT_DIR}/vp8/common/arm/loopfilter_arm.c"
  "${ROOT_DIR}/vp8/common/arm/neon/bilinearpredict_neon.c"
  "${ROOT_DIR}/vp8/common/arm/neon/copymem_neon.c"
  "${ROOT_DIR}/vp8/common/arm/neon/dc_only_idct_add_neon.c"
  "${ROOT_DIR}/vp8/common/arm/neon/dequant_idct_neon.c"
  "${ROOT_DIR}/vp8/common/arm/neon/dequantizeb_neon.c"
  "${ROOT_DIR}/vp8/common/arm/neon/idct_blk_neon.c"
  "${ROOT_DIR}/vp8/common/arm/neon/idct_dequant_0_2x_neon.c"
  "${ROOT_DIR}/vp8/common/arm/neon/idct_dequant_full_2x_neon.c"
  "${ROOT_DIR}/vp8/common/arm/neon/iwalsh_neon.c"
  "${ROOT_DIR}/vp8/common/arm/neon/loopfiltersimplehorizontaledge_neon.c"
  "${ROOT_DIR}/vp8/common/arm/neon/loopfiltersimpleverticaledge_neon.c"
  "${ROOT_DIR}/vp8/common/arm/neon/mbloopfilter_neon.c"
  "${ROOT_DIR}/vp8/common/arm/neon/shortidct4x4llm_neon.c"
  "${ROOT_DIR}/vp8/common/arm/neon/sixtappredict_neon.c"
  "${ROOT_DIR}/vp8/common/arm/neon/vp8_loopfilter_neon.c"

  "${ROOT_DIR}/vp8/encoder/arm/neon/denoising_neon.c"
  "${ROOT_DIR}/vp8/encoder/arm/neon/fastquantizeb_neon.c"
  "${ROOT_DIR}/vp8/encoder/arm/neon/shortfdct_neon.c"
  "${ROOT_DIR}/vp8/encoder/arm/neon/vp8_shortwalsh4x4_neon.c"

  "${ROOT_DIR}/vp9/common/arm/neon/vp9_iht4x4_add_neon.c"
  "${ROOT_DIR}/vp9/common/arm/neon/vp9_iht8x8_add_neon.c"

  "${ROOT_DIR}/vp9/encoder/arm/neon/vp9_dct_neon.c"
  "${ROOT_DIR}/vp9/encoder/arm/neon/vp9_error_neon.c"
  "${ROOT_DIR}/vp9/encoder/arm/neon/vp9_frame_scale_neon.c"
  "${ROOT_DIR}/vp9/encoder/arm/neon/vp9_quantize_neon.c"

  "${ROOT_DIR}/vpx_dsp/arm/avg_neon.c"
  "${ROOT_DIR}/vpx_dsp/arm/avg_pred_neon.c"
  "${ROOT_DIR}/vpx_dsp/arm/deblock_neon.c"
  "${ROOT_DIR}/vpx_dsp/arm/fdct_neon.c"
  "${ROOT_DIR}/vpx_dsp/arm/fdct_partial_neon.c"
  "${ROOT_DIR}/vpx_dsp/arm/fdct16x16_neon.c"
  "${ROOT_DIR}/vpx_dsp/arm/fdct32x32_neon.c"
  "${ROOT_DIR}/vpx_dsp/arm/fwd_txfm_neon.c"
  "${ROOT_DIR}/vpx_dsp/arm/hadamard_neon.c"
  "${ROOT_DIR}/vpx_dsp/arm/idct4x4_1_add_neon.c"
  "${ROOT_DIR}/vpx_dsp/arm/idct4x4_add_neon.c"
  "${ROOT_DIR}/vpx_dsp/arm/idct8x8_1_add_neon.c"
  "${ROOT_DIR}/vpx_dsp/arm/idct8x8_add_neon.c"
  "${ROOT_DIR}/vpx_dsp/arm/idct16x16_1_add_neon.c"
  "${ROOT_DIR}/vpx_dsp/arm/idct16x16_add_neon.c"
  "${ROOT_DIR}/vpx_dsp/arm/idct32x32_1_add_neon.c"
  "${ROOT_DIR}/vpx_dsp/arm/idct32x32_34_add_neon.c"
  "${ROOT_DIR}/vpx_dsp/arm/idct32x32_135_add_neon.c"
  "${ROOT_DIR}/vpx_dsp/arm/idct32x32_add_neon.c"
  "${ROOT_DIR}/vpx_dsp/arm/intrapred_neon.c"
  "${ROOT_DIR}/vpx_dsp/arm/loopfilter_neon.c"
  "${ROOT_DIR}/vpx_dsp/arm/quantize_neon.c"
  "${ROOT_DIR}/vpx_dsp/arm/sad_neon.c"
  "${ROOT_DIR}/vpx_dsp/arm/sad4d_neon.c"
  "${ROOT_DIR}/vpx_dsp/arm/subpel_variance_neon.c"
  "${ROOT_DIR}/vpx_dsp/arm/subtract_neon.c"
  "${ROOT_DIR}/vpx_dsp/arm/variance_neon.c"
  "${ROOT_DIR}/vpx_dsp/arm/vpx_convolve_avg_neon.c"
  "${ROOT_DIR}/vpx_dsp/arm/vpx_convolve_copy_neon.c"
  "${ROOT_DIR}/vpx_dsp/arm/vpx_convolve_neon.c"
  "${ROOT_DIR}/vpx_dsp/arm/vpx_convolve8_neon.c"
  "${ROOT_DIR}/vpx_dsp/arm/vpx_scaled_convolve8_neon.c"
 )
 if (NOT SLIB_ARM64)
  set (
   ASM_SRC_LIST
   "${ROOT_DIR}/gen/asm/armv7/vpx_dsp/arm/idct_neon.asm.S"
   "${ROOT_DIR}/gen/asm/armv7/vpx_dsp/arm/idct4x4_1_add_neon.asm.S"
   "${ROOT_DIR}/gen/asm/armv7/vpx_dsp/arm/idct4x4_add_neon.asm.S"
   "${ROOT_DIR}/gen/asm/armv7/vpx_dsp/arm/intrapred_neon_asm.asm.S"
   "${ROOT_DIR}/gen/asm/armv7/vpx_dsp/arm/loopfilter_4_neon.asm.S"
   "${ROOT_DIR}/gen/asm/armv7/vpx_dsp/arm/loopfilter_8_neon.asm.S"
   "${ROOT_DIR}/gen/asm/armv7/vpx_dsp/arm/loopfilter_16_neon.asm.S"
   "${ROOT_DIR}/gen/asm/armv7/vpx_dsp/arm/save_reg_neon.asm.S"
   "${ROOT_DIR}/gen/asm/armv7/vpx_dsp/arm/vpx_convolve_avg_neon_asm.asm.S"
   "${ROOT_DIR}/gen/asm/armv7/vpx_dsp/arm/vpx_convolve_copy_neon_asm.asm.S"
   "${ROOT_DIR}/gen/asm/armv7/vpx_dsp/arm/vpx_convolve8_avg_neon_asm.asm.S"
   "${ROOT_DIR}/gen/asm/armv7/vpx_dsp/arm/vpx_convolve8_neon_asm.asm.S"
  )
 endif ()
endif ()

set (SRC_LIST ${C_SRC_LIST} ${ASM_SRC_LIST})

include ("${CMAKE_CURRENT_LIST_DIR}/common.inc.cmake")

if (SLIB_ARM)
 if (NOT SLIB_ARM64)
  target_include_directories (
   ${TARGET_NAME}
   PRIVATE "${ROOT_DIR}/gen/asm/armv7"
  )
 endif ()
 target_include_directories (
  ${TARGET_NAME}
  PRIVATE "${ROOT_DIR}"
 )
endif ()
if (SLIB_X86)
 set_source_files_properties (
  ${C_SRC_LIST} PROPERTIES
  COMPILE_FLAGS " -mavx2 -I\"${ROOT_DIR}\""
 )
 if (SLIB_X86_64)
  set(YASM_INC_MACHINE x86_64)
  set(YASM_CUSTOM_FLAGS "-f elf64")
 else ()
  set(YASM_INC_MACHINE i386)
  set(YASM_CUSTOM_FLAGS "-f elf32")
 endif ()
 if (ANDROID)
  set(YASM_CUSTOM_FLAGS "${YASM_CUSTOM_FLAGS} -D__ANDROID__")
 endif ()
 set_source_files_properties (
  ${ASM_SRC_LIST} PROPERTIES
  COMPILE_FLAGS " ${YASM_CUSTOM_FLAGS} -Xvc -I\"${ROOT_DIR}/gen/asm/${YASM_INC_MACHINE}\" -I\"${ROOT_DIR}\""
 )
endif()

if (NOT ANDROID AND SLIB_X86)
 add_dependencies (${TARGET_NAME} yasm)
endif ()
