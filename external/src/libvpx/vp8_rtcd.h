#include "vpx_config.h"

#if ARCH_X86_64
#   include "gen/rtcd/x86_64/vp8_rtcd.h"
#elif ARCH_X86
#   include "gen/rtcd/x86/vp8_rtcd.h"
#elif ARCH_ARM
#   if defined(__arm64__) || defined(__arm64) || defined(__aarch64__)
#       include "gen/rtcd/arm64/vp8_rtcd.h"
#   else
#       include "gen/rtcd/armv7/vp8_rtcd.h"
#   endif
#endif
