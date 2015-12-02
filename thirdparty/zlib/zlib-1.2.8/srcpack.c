#ifdef _MSC_VER
#pragma warning(disable : 4996)
#endif

#if defined(__APPLE__)
#include <unistd.h>
#endif

#include "src/adler32.c"
#include "src/compress.c"
#undef DO1
#undef DO8
#include "src/crc32.c"
#undef GZIP
#include "src/gzclose.c"
#include "src/gzlib.c"
#include "src/gzread.c"
#include "src/gzwrite.c"
#undef COPY
//#include "src/infback.c"
#include "src/inffast.c"
#include "src/inflate.c"
#include "src/inftrees.c"
#include "src/uncompr.c"
#undef GZIP
#define code trees_c_code
#include "src/trees.c"
#define static_tree_desc_s deflate_c_static_tree_desc_s
#include "src/deflate.c"
#define internal_state zutil_c_internal_state
#include "src/zutil.c"
