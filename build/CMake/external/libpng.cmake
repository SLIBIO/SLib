set (TARGET_NAME png)
set (ROOT_DIR "${SLIB_PATH}/external/src/libpng")

set (
 SRC_LIST
 "${ROOT_DIR}/png.c"
 "${ROOT_DIR}/pngerror.c"
 "${ROOT_DIR}/pngget.c"
 "${ROOT_DIR}/pngmem.c"
 "${ROOT_DIR}/pngpread.c"
 "${ROOT_DIR}/pngread.c"
 "${ROOT_DIR}/pngrio.c"
 "${ROOT_DIR}/pngrtran.c"
 "${ROOT_DIR}/pngrutil.c"
 "${ROOT_DIR}/pngset.c"
 "${ROOT_DIR}/pngtrans.c"
 "${ROOT_DIR}/pngwio.c"
 "${ROOT_DIR}/pngwrite.c"
 "${ROOT_DIR}/pngwtran.c"
 "${ROOT_DIR}/pngwutil.c"
)

if (SLIB_ARM)
 list (
  APPEND SRC_LIST
  "${ROOT_DIR}/arm/arm_init.c"
  "${ROOT_DIR}/arm/filter_neon_intrinsics.c"
 )
endif ()

if (SLIB_X86)
 list (
  APPEND SRC_LIST
  "${ROOT_DIR}/intel/intel_init.c"
  "${ROOT_DIR}/intel/filter_sse2_intrinsics.c"
 )
endif ()

include ("${CMAKE_CURRENT_LIST_DIR}/common.inc.cmake")

target_include_directories (
 ${TARGET_NAME}
 PRIVATE "${ROOT_DIR}"
)
