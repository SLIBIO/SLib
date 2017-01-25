#ifdef _MSC_VER
#pragma warning(disable : 4996)
#endif

#define FALSE 0
#define TRUE 1

#include "src/jaricom.c"
#include "src/jcapimin.c"
#include "src/jcapistd.c"
#include "src/jcarith.c"
#include "src/jccoefct.c"
#include "src/jccolor.c"
#undef FIX
#include "src/jcdctmgr.c"
#define encode_mcu_DC_first jchuff_c__encode_mcu_DC_first
#define encode_mcu_AC_first jchuff_c__encode_mcu_AC_first
#define encode_mcu_DC_refine jchuff_c__encode_mcu_DC_refine
#define encode_mcu_AC_refine jchuff_c__encode_mcu_AC_refine
#include "src/jchuff.c"
#include "src/jcinit.c"
#include "src/jcmainct.c"
#define emit_byte jcmarker_c__emit_byte
#include "src/jcmarker.c"
#include "src/jcmaster.c"
#include "src/jcomapi.c"
#include "src/jcparam.c"
#include "src/jcprepct.c"
#include "src/jcsample.c"
#define my_coef_controller jctrans_c__my_coef_controller
#define my_coef_ptr jctrans_c__my_coef_ptr
#define start_iMCU_row jctrans_c__start_iMCU_row
#define start_pass_coef jctrans_c__start_pass_coef
#define compress_output jctrans_c__compress_output
#include "src/jctrans.c"
#include "src/jdapimin.c"
#include "src/jdapistd.c"
#define arith_entropy_ptr jdarith_c__arith_entropy_ptr
#include "src/jdarith.c"
#include "src/jdatadst.c"
#include "src/jdatasrc.c"
#undef my_coef_controller
#define my_coef_controller jdcoefct_c__my_coef_controller
#undef my_coef_ptr
#define my_coef_ptr jdcoefct_c__my_coef_ptr
#undef start_iMCU_row
#define start_iMCU_row jdcoefct_c__start_iMCU_row
#undef start_pass_coef
#include "src/jdcoefct.c"
#define my_cconvert_ptr jdcolor_c__my_cconvert_ptr
#undef FIX
#undef TABLE_SIZE
#define rgb_gray_convert jdcolor_c__rgb_gray_convert
#define rgb_convert jdcolor_c__rgb_convert
#define null_convert jdcolor_c__null_convert
#define grayscale_convert jdcolor_c__grayscale_convert
#include "src/jdcolor.c"
#undef FIX
#include "src/jddctmgr.c"
#define savable_state jdhuff_c__savable_state
#define huff_entropy_ptr jdhuff_c__huff_entropy_ptr
#define finish_pass_huff jdhuff_c__finish_pass_huff
#define process_restart jdhuff_c__process_restart
#define decode_mcu_DC_first jdhuff_c__decode_mcu_DC_first
#define decode_mcu_AC_first jdhuff_c__decode_mcu_AC_first
#define decode_mcu_DC_refine jdhuff_c__decode_mcu_DC_refine
#define decode_mcu_AC_refine jdhuff_c__decode_mcu_AC_refine
#include "src/jdhuff.c"
#define initial_setup jdinput_c__initial_setup
#define per_scan_setup jdinput_c__per_scan_setup
#include "src/jdinput.c"
#define my_main_controller jdmainct_c__my_main_controller
#define my_main_ptr jdmainct_c__my_main_ptr
#define process_data_simple_main jdmainct_c__process_data_simple_main
#define start_pass_main jdmainct_c__start_pass_main
#include "src/jdmainct.c"
#define M_SOF0 M_0xc0
#define M_SOF1 M_0xc1
#define M_SOF2 M_0xc2
#define M_SOF3 M_0xc3
#define M_SOF5 M_0xc5
#define M_SOF6 M_0xc6
#define M_SOF7 M_0xc7
#define M_JPG M_0xc8
#define M_SOF9 M_0xc9
#define M_SOF10 M_0xca
#define M_SOF11 M_0xcb
#define M_SOF13 M_0xcd
#define M_SOF14 M_0xce
#define M_SOF15 M_0xcf
#define M_DHT M_0xc4
#define M_DAC M_0xcc
#define M_RST0 M_0xd0
#define M_RST1 M_0xd1
#define M_RST2 M_0xd2
#define M_RST3 M_0xd3
#define M_RST4 M_0xd4
#define M_RST5 M_0xd5
#define M_RST6 M_0xd6
#define M_RST7 M_0xd7
#define M_SOI M_0xd8
#define M_EOI M_0xd9
#define M_SOS M_0xda
#define M_DQT M_0xdb
#define M_DNL M_0xdc
#define M_DRI M_0xdd
#define M_DHP M_0xde
#define M_EXP M_0xdf
#define M_APP0 M_0xe0
#define M_APP1 M_0xe1
#define M_APP2 M_0xe2
#define M_APP3 M_0xe3
#define M_APP4 M_0xe4
#define M_APP5 M_0xe5
#define M_APP6 M_0xe6
#define M_APP7 M_0xe7
#define M_APP8 M_0xe8
#define M_APP9 M_0xe9
#define M_APP10 M_0xea
#define M_APP11 M_0xeb
#define M_APP12 M_0xec
#define M_APP13 M_0xed
#define M_APP14 M_0xee
#define M_APP15 M_0xef
#define M_JPG0 M_0xf0
#define M_JPG8 M_0xf8
#define M_JPG13 M_0xfd
#define M_COM M_0xfe
#define M_TEM M_0x01
#define M_ERROR M_0x100
#define JPEG_MARKER jdmarker_c__JPEG_MARKER
#define my_marker_ptr jdmarker_c__my_marker_ptr
#include "src/jdmarker.c"
#define my_master_ptr jdmarker_c__my_master_ptr
#include "src/jdmaster.c"
#undef FIX
#define build_ycc_rgb_table jdmergy_c__build_ycc_rgb_table
#define build_bg_ycc_rgb_table jdmergy_c__build_bg_ycc_rgb_table
#include "src/jdmerge.c"
#include "src/jdpostct.c"
#define my_upsampler jdsample_c__my_upsampler
#define my_upsample_ptr jdsample_c__my_upsample_ptr
#include "src/jdsample.c"
#include "src/jdtrans.c"
#include "src/jerror.c"
#undef FIX
#include "src/jfdctflt.c"
#undef CONST_BITS
#include "src/jfdctfst.c"
#undef DESCALE
#undef CONST_BITS
#undef FIX_0_541196100
#undef MULTIPLY
#include "src/jfdctint.c"
#include "src/jidctflt.c"
#undef CONST_BITS
#undef FIX_1_847759065
#undef MULTIPLY
#undef DEQUANTIZE
#include "src/jidctfst.c"
#undef DESCALE
#undef CONST_BITS
#undef FIX_1_847759065
#undef MULTIPLY
#undef DEQUANTIZE
#include "src/jidctint.c"
#include "src/jmemmgr.c"
//#include "src/jmemansi.c"
#include "src/jmemnobs.c"
#include "src/jquant1.c"
#define my_cquantizer jquant2_c__my_cquantizer
#define my_cquantize_ptr jquant2_c__my_cquantize_ptr
#define box jquant2_box
#include "src/jquant2.c"
#include "src/jutils.c"
