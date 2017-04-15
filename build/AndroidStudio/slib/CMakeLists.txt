cmake_minimum_required(VERSION 3.4.1)

set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -frtti")
if (CMAKE_BUILD_TYPE MATCHES Release)
 set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g0")
 set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g0")
endif ()

set (SLIB_PATH ${CMAKE_CURRENT_LIST_DIR}/../../..)
set (SLIB_OUTPUT_PATH ${SLIB_PATH}/lib/Android/${CMAKE_BUILD_TYPE}-${ANDROID_ABI}/)

include_directories (
 ${SLIB_PATH}/include
 ${SLIB_PATH}/thirdparty
)

file (
 GLOB SLIB_CORE_FILES
 ${SLIB_PATH}/src/slib/core/*.cpp
 ${SLIB_PATH}/src/slib/math/*.cpp
 ${SLIB_PATH}/src/slib/crypto/*.cpp
 ${SLIB_PATH}/src/slib/network/*.cpp
)
add_library (
 slib-core STATIC
 ${SLIB_CORE_FILES}
)
set_target_properties (
 slib-core
 PROPERTIES
 ARCHIVE_OUTPUT_DIRECTORY "${SLIB_OUTPUT_PATH}"
)

file (
 GLOB SLIB_EXTRA_FILES
 ${SLIB_PATH}/src/slib/graphics/*.cpp
 ${SLIB_PATH}/src/slib/render/*.cpp
 ${SLIB_PATH}/src/slib/ui/*.cpp
 ${SLIB_PATH}/src/slib/media/*.cpp
 ${SLIB_PATH}/src/slib/device/*.cpp
 ${SLIB_PATH}/src/slib/db/*.cpp
 ${SLIB_PATH}/src/slib/geo/*.cpp
)

add_library (
 slib STATIC
 ${SLIB_CORE_FILES}
 ${SLIB_EXTRA_FILES}
)
set_target_properties (
 slib
 PROPERTIES
 ARCHIVE_OUTPUT_DIRECTORY "${SLIB_OUTPUT_PATH}"
)


if (CMAKE_BUILD_TYPE MATCHES Debug)
 set_source_files_properties (
  ${SLIB_PATH}/src/thirdparty/thirdparty_zlib.c
  ${SLIB_PATH}/src/thirdparty/thirdparty_libpng.c
  ${SLIB_PATH}/src/thirdparty/thirdparty_libjpeg.c
  ${SLIB_PATH}/src/thirdparty/thirdparty_freetype.c
  ${SLIB_PATH}/src/thirdparty/thirdparty_sqlite3.c
  ${SLIB_PATH}/src/thirdparty/thirdparty_libopus.c
  ${SLIB_PATH}/src/thirdparty/thirdparty_libvpx.c
  ${SLIB_PATH}/src/thirdparty/thirdparty_libvpx_enc.c
  ${SLIB_PATH}/src/thirdparty/thirdparty_libvpx_dec.c
  PROPERTIES
  COMPILE_FLAGS -O3
 )
endif ()

if (ANDROID_ABI STREQUAL "armeabi-v7a")
 set_property (
  SOURCE
  ${SLIB_PATH}/src/thirdparty/thirdparty_libopus.c
  ${SLIB_PATH}/src/thirdparty/thirdparty_libvpx.c
  ${SLIB_PATH}/src/thirdparty/thirdparty_libvpx_enc.c
  ${SLIB_PATH}/src/thirdparty/thirdparty_libvpx_dec.c
  APPEND_STRING PROPERTY
  COMPILE_FLAGS " -mfpu=neon"
 )
endif ()

add_library (
 zlib STATIC
 ${SLIB_PATH}/src/thirdparty/thirdparty_zlib.c
)
set_target_properties (
 zlib
 PROPERTIES
 ARCHIVE_OUTPUT_DIRECTORY "${SLIB_OUTPUT_PATH}"
)

add_library (
 png STATIC
 ${SLIB_PATH}/src/thirdparty/thirdparty_libpng.c
)
set_target_properties (
 png
 PROPERTIES
 ARCHIVE_OUTPUT_DIRECTORY "${SLIB_OUTPUT_PATH}"
)

add_library (
 jpeg STATIC
 ${SLIB_PATH}/src/thirdparty/thirdparty_libjpeg.c
)
set_target_properties (
 jpeg
 PROPERTIES
 ARCHIVE_OUTPUT_DIRECTORY "${SLIB_OUTPUT_PATH}"
)

add_library (
 freetype STATIC
 ${SLIB_PATH}/src/thirdparty/thirdparty_freetype.c
)
set_target_properties (
 freetype
 PROPERTIES
 ARCHIVE_OUTPUT_DIRECTORY "${SLIB_OUTPUT_PATH}"
)

add_library (
 sqlite3 STATIC
 ${SLIB_PATH}/src/thirdparty/thirdparty_sqlite3.c
)
set_target_properties (
 sqlite3
 PROPERTIES
 ARCHIVE_OUTPUT_DIRECTORY "${SLIB_OUTPUT_PATH}"
)

add_library (
 opus STATIC
 ${SLIB_PATH}/src/thirdparty/thirdparty_libopus.c
)
set_target_properties (
 opus
 PROPERTIES
 ARCHIVE_OUTPUT_DIRECTORY "${SLIB_OUTPUT_PATH}"
)

add_library (
 vpx STATIC
 ${SLIB_PATH}/src/thirdparty/thirdparty_libvpx.c
 ${SLIB_PATH}/src/thirdparty/thirdparty_libvpx_enc.c
 ${SLIB_PATH}/src/thirdparty/thirdparty_libvpx_dec.c
)
set_target_properties (
 vpx
 PROPERTIES
 ARCHIVE_OUTPUT_DIRECTORY "${SLIB_OUTPUT_PATH}"
)

