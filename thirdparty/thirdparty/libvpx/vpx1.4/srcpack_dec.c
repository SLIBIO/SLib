#ifdef _MSC_VER
#pragma warning(disable : 4996)
#endif

#include "vp8/vp8_dx_iface.c"
#include "vp8/decoder/dboolhuff.c"
#include "vp8/decoder/decodeframe.c"
#include "vp8/decoder/decodemv.c"
#include "vp8/decoder/detokenize.c"
#include "vp8/decoder/onyxd_if.c"
#if CONFIG_MULTITHREAD
#	include "vp8/decoder/threading.c"
#endif
#include "vpx/src/vpx_decoder.c"

