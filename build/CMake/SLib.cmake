set (SLIB_PATH "${CMAKE_CURRENT_LIST_DIR}/../..")

if (CMAKE_SYSTEM_PROCESSOR MATCHES "^arm" OR CMAKE_SYSTEM_PROCESSOR MATCHES "^aarch64")
 set (SLIB_ARM YES)
 if (CMAKE_SIZEOF_VOID_P EQUAL 8)
  set (SLIB_ARM64 YES)
 endif ()
endif ()
if (CMAKE_SYSTEM_PROCESSOR MATCHES "^x86" OR CMAKE_SYSTEM_PROCESSOR MATCHES "i[3456]86")
 set (SLIB_X86 YES)
 if (CMAKE_SIZEOF_VOID_P EQUAL 8)
  set (SLIB_X86_64 YES)
 endif ()
endif ()

set (SLIB_LIB_PATH "${SLIB_PATH}/lib/CMake/${CMAKE_BUILD_TYPE}-${CMAKE_SYSTEM_PROCESSOR}")
set (SLIB_BIN_PATH "${SLIB_PATH}/bin/CMake/${CMAKE_SYSTEM_PROCESSOR}")
if (ANDROID)
 set (SLIB_LIB_PATH "${SLIB_PATH}/lib/Android/${CMAKE_BUILD_TYPE}-${ANDROID_ABI}")
 set (SLIB_BIN_PATH "${SLIB_PATH}/bin/Android/${ANDROID_ABI}")
endif ()
if (CMAKE_SYSTEM_NAME STREQUAL Linux)
 set (SLIB_LIB_PATH "${SLIB_PATH}/lib/Linux/${CMAKE_BUILD_TYPE}-${CMAKE_SYSTEM_PROCESSOR}")
 set (SLIB_BIN_PATH "${SLIB_PATH}/bin/Linux/${CMAKE_SYSTEM_PROCESSOR}")
endif ()

set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -frtti")
# generates no debug information
if (CMAKE_BUILD_TYPE MATCHES Release)
 set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g0")
 set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g0")
endif ()
if (SLIB_ARM AND NOT SLIB_ARM64)
 set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -mfpu=neon")
 set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -mfpu=neon") 
endif ()

if (ANDROID)
 include_directories (
  "${SLIB_PATH}/include"
  "${SLIB_PATH}/external/include"
 )
else ()
 include_directories (
  "${SLIB_PATH}/include"
  "${SLIB_PATH}/external/include"
  "${SLIB_PATH}/external/include/glib"
  "${SLIB_PATH}/external/include/gtk"
 )
endif ()

link_directories(
 "${SLIB_LIB_PATH}"
)

# enable asm
enable_language(ASM)
if (ANDROID AND SLIB_X86)
 enable_language (ASM_NASM)
 set (
  CMAKE_ASM_NASM_COMPILE_OBJECT
  "<CMAKE_ASM_NASM_COMPILER> <FLAGS> -o <OBJECT> <SOURCE>"
 )
endif ()

if(ANDROID)
 include_directories("${ANDROID_NDK}/sources/android/cpufeatures")
 add_library(cpufeatures "${ANDROID_NDK}/sources/android/cpufeatures/cpu-features.c")
 target_link_libraries(cpufeatures dl)
 set_target_properties (
  cpufeatures
  PROPERTIES
  ARCHIVE_OUTPUT_DIRECTORY "${SLIB_LIB_PATH}"
 )
endif()

file (
 GLOB SLIB_CORE_FILES
 "${SLIB_PATH}/src/slib/core/*.cpp"
 "${SLIB_PATH}/src/slib/math/*.cpp"
 "${SLIB_PATH}/src/slib/crypto/*.cpp"
 "${SLIB_PATH}/src/slib/network/*.cpp"
)
add_library (
 slib-core STATIC
 ${SLIB_CORE_FILES}
)
set_target_properties (
 slib-core
 PROPERTIES
 ARCHIVE_OUTPUT_DIRECTORY "${SLIB_LIB_PATH}"
)

if (ANDROID)
 file (
  GLOB SLIB_EXTRA_FILES
  "${SLIB_PATH}/src/slib/graphics/*.cpp"
  "${SLIB_PATH}/src/slib/render/*.cpp"
  "${SLIB_PATH}/src/slib/ui/*.cpp"
  "${SLIB_PATH}/src/slib/media/*.cpp"
  "${SLIB_PATH}/src/slib/device/*.cpp"
  "${SLIB_PATH}/src/slib/db/*.cpp"
  "${SLIB_PATH}/src/slib/geo/*.cpp"
 )
else ()
 file (
  GLOB SLIB_EXTRA_FILES
  "${SLIB_PATH}/src/slib/graphics/*.cpp"
  "${SLIB_PATH}/src/slib/render/*.cpp"
  "${SLIB_PATH}/src/slib/ui/*.cpp"
  "${SLIB_PATH}/src/slib/media/*.cpp"
  "${SLIB_PATH}/src/slib/db/*.cpp"
  "${SLIB_PATH}/src/slib/geo/*.cpp"
  "${SLIB_PATH}/src/slib/web/*.cpp"
 )
endif ()
add_library (
 slib STATIC
 ${SLIB_CORE_FILES}
 ${SLIB_EXTRA_FILES}
)
set_target_properties (
 slib
 PROPERTIES
 ARCHIVE_OUTPUT_DIRECTORY "${SLIB_LIB_PATH}"
)

include ("${CMAKE_CURRENT_LIST_DIR}/external/zlib.cmake")
include ("${CMAKE_CURRENT_LIST_DIR}/external/libpng.cmake")
include ("${CMAKE_CURRENT_LIST_DIR}/external/libjpeg.cmake")
include ("${CMAKE_CURRENT_LIST_DIR}/external/freetype.cmake")
include ("${CMAKE_CURRENT_LIST_DIR}/external/sqlite.cmake")
include ("${CMAKE_CURRENT_LIST_DIR}/external/yasm.cmake")
include ("${CMAKE_CURRENT_LIST_DIR}/external/opus.cmake")
include ("${CMAKE_CURRENT_LIST_DIR}/external/vpx.cmake")
include ("${CMAKE_CURRENT_LIST_DIR}/external/zxing.cmake")
