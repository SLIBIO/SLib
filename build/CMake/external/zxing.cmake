set (TARGET_NAME zxing)
set (ROOT_DIR "${SLIB_PATH}/external/src/zxing/zxing")

file (GLOB SRC_LIST
 "${ROOT_DIR}/*.cpp"
 "${ROOT_DIR}/aztec/*.cpp"
 "${ROOT_DIR}/datamatrix/*.cpp"
 "${ROOT_DIR}/maxicode/*.cpp"
 "${ROOT_DIR}/oned/*.cpp"
 "${ROOT_DIR}/oned/rss/*.cpp"
 "${ROOT_DIR}/pdf417/*.cpp"
 "${ROOT_DIR}/qrcode/*.cpp"
 "${ROOT_DIR}/textcodec/*.cpp"
)

include ("${CMAKE_CURRENT_LIST_DIR}/common.inc.cmake")

target_include_directories (
 ${TARGET_NAME}
 PRIVATE "${ROOT_DIR}"
)
target_compile_options (
 ${TARGET_NAME}
 PRIVATE -Wno-tautological-constant-compare
)
