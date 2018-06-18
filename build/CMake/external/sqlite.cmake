set (TARGET_NAME sqlite3)
set (ROOT_DIR "${SLIB_PATH}/external/src/sqlite")

set (
 SRC_LIST
 "${ROOT_DIR}/sqlite3.c"
)

include ("${CMAKE_CURRENT_LIST_DIR}/common.inc.cmake")
