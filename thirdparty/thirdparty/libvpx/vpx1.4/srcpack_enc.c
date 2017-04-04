#ifdef _MSC_VER
#pragma warning(disable : 4996)
#endif

#include "vp8/vp8_cx_iface.c"
#include "vp8/encoder/bitstream.c"
#include "vp8/encoder/boolhuff.c"
#include "vp8/encoder/dct.c"
#include "vp8/encoder/denoising.c"
#include "vp8/encoder/encodeframe.c"
#include "vp8/encoder/encodeintra.c"
#include "vp8/encoder/encodemb.c"
#include "vp8/encoder/encodemv.c"
#undef VP8_ENCODER_ONYX_INT_H_
#undef CHECK_MEM_ERROR
#if CONFIG_MULTITHREAD
#	include "vp8/encoder/ethreading.c"
#endif
#include "vp8/encoder/firstpass.c"
#include "vp8/encoder/lookahead.c"
#include "vp8/encoder/mcomp.c"
#include "vp8/encoder/modecosts.c"
#include "vp8/encoder/onyx_if.c"
#include "vp8/encoder/pickinter.c"
#include "vp8/encoder/picklpf.c"
#include "vp8/encoder/quantize.c"
#include "vp8/encoder/ratectrl.c"
#include "vp8/encoder/rdopt.c"
#include "vp8/encoder/segmentation.c"
#include "vp8/encoder/temporal_filter.c"
#include "vp8/encoder/tokenize.c"
#include "vp8/encoder/treewriter.c"
#include "vpx/src/vpx_encoder.c"
