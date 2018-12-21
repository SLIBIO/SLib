set (TARGET_NAME hiredis)
set (ROOT_DIR "${SLIB_PATH}/external/src/hiredis")

set (
 SRC_LIST
 "${ROOT_DIR}/async.c"
 "${ROOT_DIR}/dict.c"
 "${ROOT_DIR}/hiredis.c"
 "${ROOT_DIR}/net.c"
 "${ROOT_DIR}/read.c"
 "${ROOT_DIR}/sds.c"
)

include ("${CMAKE_CURRENT_LIST_DIR}/common.inc.cmake")
