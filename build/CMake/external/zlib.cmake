set (TARGET_NAME zlib)
set (ROOT_DIR "${SLIB_PATH}/external/src/zlib")

set (
 SRC_LIST
 "${ROOT_DIR}/adler32.c"
 "${ROOT_DIR}/compress.c"
 "${ROOT_DIR}/crc32.c"
 "${ROOT_DIR}/deflate.c"
 "${ROOT_DIR}/gzclose.c"
 "${ROOT_DIR}/gzlib.c"
 "${ROOT_DIR}/gzread.c"
 "${ROOT_DIR}/gzwrite.c"
 "${ROOT_DIR}/infback.c"
 "${ROOT_DIR}/inffast.c"
 "${ROOT_DIR}/inflate.c"
 "${ROOT_DIR}/inftrees.c"
 "${ROOT_DIR}/trees.c"
 "${ROOT_DIR}/uncompr.c"
 "${ROOT_DIR}/zutil.c"
)

include ("${CMAKE_CURRENT_LIST_DIR}/common.inc.cmake")
