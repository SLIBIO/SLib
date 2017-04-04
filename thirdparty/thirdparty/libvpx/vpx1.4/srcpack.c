#ifdef _MSC_VER
#pragma warning(disable : 4996)
#endif

#include "vp8/common/alloccommon.c"
#include "vp8/common/blockd.c"
#include "vp8/common/debugmodes.c"
#include "vp8/common/dequantize.c"
#include "vp8/common/entropy.c"
#include "vp8/common/entropymode.c"
#include "vp8/common/entropymv.c"
#include "vp8/common/extend.c"
#include "vp8/common/filter.c"
#include "vp8/common/findnearmv.c"
#include "vp8/common/idct_blk.c"
#include "vp8/common/idctllm.c"
#include "vp8/common/loopfilter.c"
#include "vp8/common/loopfilter_filters.c"
#include "vp8/common/mbpitch.c"
#if CONFIG_POSTPROC
#include "vp8/common/mfqe.c"
#include "vp8/common/postproc.c"
#endif
#include "vp8/common/modecont.c"
#include "vp8/common/quant_common.c"
#include "vp8/common/reconinter.c"
#include "vp8/common/reconintra.c"
#include "vp8/common/reconintra4x4.c"
#undef VP8_RTCD_H_
#undef RTCD_EXTERN
#include "vp8/common/rtcd.c"
#include "vp8/common/sad_c.c"
#include "vp8/common/setupintrarecon.c"
#include "vp8/common/swapyv12buffer.c"
#include "vp8/common/systemdependent.c"
#include "vp8/common/treecoder.c"
#include "vp8/common/variance_c.c"
#include "vpx/src/vpx_codec.c"
#include "vpx/src/vpx_image.c"
#include "vpx/src/vpx_psnr.c"

#undef VPX_MEM_VPX_MEM_H_
#undef VPX_TRACK_MEM_USAGE
#undef VPX_CHECK_MEM_FUNCTIONS
#undef REPLACE_BUILTIN_FUNCTIONS
#undef vpx_memcpy
#undef vpx_memset
#undef vpx_memmove
#include "vpx_mem/vpx_mem.c"
#undef VPX_SCALE_RTCD_H_
#define setup_rtcd_internal setup_rtcd_internal_vpx_scale_rtcd
#include "vpx_scale/vpx_scale_rtcd.c"
#include "vpx_scale/generic/gen_scalers.c"
#include "vpx_scale/generic/vpx_scale.c"
#include "vpx_scale/generic/yv12config.c"
#include "vpx_scale/generic/yv12extend.c"

#include "vpx_ports/arm_cpudetect.c"
//x86 asm
#if defined(_WIN32) || defined(_WIN64)

#	include "vp8/common/x86/filter_x86.c"
#	include "vp8/common/x86/idct_blk_mmx.c"
#	include "vp8/common/x86/idct_blk_sse2.c"
#	include "vp8/common/x86/loopfilter_x86.c"
#	undef SELECT_EOB
#	include "vp8/common/x86/recon_wrapper_sse2.c"
#	include "vp8/common/x86/variance_mmx.c"
#	include "vp8/common/x86/variance_sse2.c"
#	include "vp8/common/x86/variance_ssse3.c"
#	include "vp8/common/x86/vp8_asm_stubs.c"
#	include "vp8/encoder/x86/denoising_sse2.c"
#	include "vp8/encoder/x86/quantize_sse2.c"
#	undef SELECT_EOB
#	include "vp8/encoder/x86/quantize_sse4.c"
#	include "vp8/encoder/x86/quantize_ssse3.c"
#	include "vp8/encoder/x86/vp8_enc_stubs_mmx.c"
#	include "vp8/encoder/x86/vp8_enc_stubs_sse2.c"

#elif defined(__APPLE__) && defined(__MACH__)
#   include <TargetConditionals.h>
#	if TARGET_OS_IPHONE
#		if TARGET_OS_SIMULATOR
#           include "vp8/common/x86/filter_x86.c"
#           include "vp8/common/x86/idct_blk_mmx.c"
#           include "vp8/common/x86/idct_blk_sse2.c"
#           include "vp8/common/x86/loopfilter_x86.c"
#           undef SELECT_EOB
#           include "vp8/common/x86/recon_wrapper_sse2.c"
#           include "vp8/common/x86/variance_mmx.c"
#           include "vp8/common/x86/variance_sse2.c"
#           include "vp8/common/x86/vp8_asm_stubs.c"
#           include "vp8/encoder/x86/denoising_sse2.c"
#           include "vp8/encoder/x86/quantize_sse2.c"
#           undef SELECT_EOB
#           if defined(__x86_64__) || defined(__amd64__)
#               define __SSSE3__
#               define __SSE4_1__
#               include "vp8/encoder/x86/quantize_sse4.c"
#               include "vp8/common/x86/variance_ssse3.c"
#               include "vp8/encoder/x86/quantize_ssse3.c"
#           endif
#           include "vp8/encoder/x86/vp8_enc_stubs_mmx.c"
#           include "vp8/encoder/x86/vp8_enc_stubs_sse2.c"
#		else
#			include "vp8/common/arm/neon/bilinearpredict_neon.c"
#			include "vp8/common/arm/neon/copymem_neon.c"
#			include "vp8/common/arm/neon/dc_only_idct_add_neon.c"
#			define cospi8sqrt2minus1 cospi8sqrt2minus1_dequant_idct_neon_c
#			define sinpi8sqrt2 sinpi8sqrt2_dequant_idct_neon_c
#			include "vp8/common/arm/neon/dequant_idct_neon.c"
#			include "vp8/common/arm/neon/dequantizeb_neon.c"
#			include "vp8/common/arm/neon/idct_blk_neon.c"
#			include "vp8/common/arm/neon/idct_dequant_0_2x_neon.c"

#			undef cospi8sqrt2minus1
#			undef sinpi8sqrt2
#			define cospi8sqrt2minus1 cospi8sqrt2minus1_idct_dequant_full_2x_neon_c
#			define sinpi8sqrt2 sinpi8sqrt2_idct_dequant_full_2x_neon_c
#			include "vp8/common/arm/neon/idct_dequant_full_2x_neon.c"
#			include "vp8/common/arm/neon/iwalsh_neon.c"
#			include "vp8/common/arm/neon/loopfilter_neon.c"
#			include "vp8/common/arm/neon/loopfiltersimplehorizontaledge_neon.c"
#			include "vp8/common/arm/neon/loopfiltersimpleverticaledge_neon.c"
#			include "vp8/common/arm/neon/mbloopfilter_neon.c"
#			include "vp8/common/arm/neon/reconintra_neon.c"

#			include "vp8/common/arm/neon/sad_neon.c"

#			undef cospi8sqrt2minus1
#			undef sinpi8sqrt2
#			define cospi8sqrt2minus1 cospi8sqrt2minus1_shortidct4x4llm_neon_c
#			define sinpi8sqrt2 sinpi8sqrt2_shortidct4x4llm_neon_c
#			include "vp8/common/arm/neon/shortidct4x4llm_neon.c"
#			define vp8_sub_pel_filters vp8_sub_pel_filters_sixtappredict_neon_c
#			include "vp8/common/arm/neon/sixtappredict_neon.c"
#			include "vp8/common/arm/neon/variance_neon.c"
#			include "vp8/common/arm/neon/vp8_subpixelvariance_neon.c"
#			include "vp8/encoder/arm/neon/denoising_neon.c"
#			include "vp8/encoder/arm/neon/fastquantizeb_neon.c"
#			include "vp8/encoder/arm/neon/shortfdct_neon.c"
#			include "vp8/encoder/arm/neon/subtract_neon.c"
#			include "vp8/encoder/arm/neon/vp8_mse16x16_neon.c"
#			include "vp8/encoder/arm/neon/vp8_shortwalsh4x4_neon.c"
#			include "vp8/common/arm/loopfilter_arm.c"
#			include "vp8/common/arm/variance_arm.c"
#			include "vp8/common/arm/filter_arm.c"
#		endif
#	elif defined(TARGET_OS_MAC)
#       include "vp8/common/x86/filter_x86.c"
#       include "vp8/common/x86/idct_blk_mmx.c"
#       include "vp8/common/x86/idct_blk_sse2.c"
#       include "vp8/common/x86/loopfilter_x86.c"
#       undef SELECT_EOB
#       include "vp8/common/x86/recon_wrapper_sse2.c"
#       include "vp8/common/x86/variance_mmx.c"
#       include "vp8/common/x86/variance_sse2.c"
#       include "vp8/common/x86/variance_ssse3.c"
#       include "vp8/common/x86/vp8_asm_stubs.c"
#       include "vp8/encoder/x86/denoising_sse2.c"
#       include "vp8/encoder/x86/quantize_sse2.c"
#       undef SELECT_EOB
#       define __SSE4_1__
#       include "vp8/encoder/x86/quantize_sse4.c"
#       include "vp8/encoder/x86/quantize_ssse3.c"
#       include "vp8/encoder/x86/vp8_enc_stubs_mmx.c"
#       include "vp8/encoder/x86/vp8_enc_stubs_sse2.c"
#   endif
#elif defined(__ANDROID__)
#	if defined(__aarch64__)
#		include "vp8/common/arm/neon/bilinearpredict_neon.c"
#		include "vp8/common/arm/neon/copymem_neon.c"
#		include "vp8/common/arm/neon/dc_only_idct_add_neon.c"
#		define cospi8sqrt2minus1 cospi8sqrt2minus1_dequant_idct_neon_c
#		define sinpi8sqrt2 sinpi8sqrt2_dequant_idct_neon_c
#		include "vp8/common/arm/neon/dequant_idct_neon.c"
#		include "vp8/common/arm/neon/dequantizeb_neon.c"
#		include "vp8/common/arm/neon/idct_blk_neon.c"
#		include "vp8/common/arm/neon/idct_dequant_0_2x_neon.c"

#		undef cospi8sqrt2minus1
#		undef sinpi8sqrt2
#		define cospi8sqrt2minus1 cospi8sqrt2minus1_idct_dequant_full_2x_neon_c
#		define sinpi8sqrt2 sinpi8sqrt2_idct_dequant_full_2x_neon_c
#		include "vp8/common/arm/neon/idct_dequant_full_2x_neon.c"
#		include "vp8/common/arm/neon/iwalsh_neon.c"
#		include "vp8/common/arm/neon/loopfilter_neon.c"
#		include "vp8/common/arm/neon/loopfiltersimplehorizontaledge_neon.c"
#		include "vp8/common/arm/neon/loopfiltersimpleverticaledge_neon.c"
#		include "vp8/common/arm/neon/mbloopfilter_neon.c"
#		include "vp8/common/arm/neon/reconintra_neon.c"

#		include "vp8/common/arm/neon/sad_neon.c"

#		undef cospi8sqrt2minus1
#		undef sinpi8sqrt2
#		define cospi8sqrt2minus1 cospi8sqrt2minus1_shortidct4x4llm_neon_c
#		define sinpi8sqrt2 sinpi8sqrt2_shortidct4x4llm_neon_c
#		include "vp8/common/arm/neon/shortidct4x4llm_neon.c"
#		define vp8_sub_pel_filters vp8_sub_pel_filters_sixtappredict_neon_c
#		include "vp8/common/arm/neon/sixtappredict_neon.c"
#		include "vp8/common/arm/neon/variance_neon.c"
#		include "vp8/common/arm/neon/vp8_subpixelvariance_neon.c"
#		include "vp8/encoder/arm/neon/denoising_neon.c"
#		include "vp8/encoder/arm/neon/fastquantizeb_neon.c"
#		include "vp8/encoder/arm/neon/shortfdct_neon.c"
#		include "vp8/encoder/arm/neon/subtract_neon.c"
#		include "vp8/encoder/arm/neon/vp8_mse16x16_neon.c"
#		include "vp8/encoder/arm/neon/vp8_shortwalsh4x4_neon.c"
#		include "vp8/common/arm/loopfilter_arm.c"
#		include "vp8/common/arm/variance_arm.c"
#		include "vp8/common/arm/filter_arm.c"
#	elif defined(__arm__)
#		include "vp8/common/arm/bilinearfilter_arm.c"
#		include "vp8/common/arm/dequantize_arm.c"
#		include "vp8/common/arm/filter_arm.c"
#		include "vp8/common/arm/loopfilter_arm.c"
#		include "vp8/common/arm/variance_arm.c"
#		include "vp8/common/arm/idct_blk_v6.c"
#		include "vp8/encoder/arm/dct_arm.c"
#	endif
#endif
