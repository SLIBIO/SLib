cmake_minimum_required(VERSION 3.4.1)

set (SLIB_PATH ${CMAKE_CURRENT_LIST_DIR}/../../..)
set (SLIB_OUTPUT_PATH ${SLIB_PATH}/lib/Android/${CMAKE_BUILD_TYPE}-${ANDROID_ABI}/)

include_directories (
 ${SLIB_PATH}/include
 ${SLIB_PATH}/thirdparty
)

file (GLOB SLIB_CORE_FILES
 ${SLIB_PATH}/src/slib/core/*.cpp
 ${SLIB_PATH}/src/slib/math/*.cpp
 ${SLIB_PATH}/src/slib/crypto/*.cpp
)
add_library (
 slib-core STATIC
 ${SLIB_CORE_FILES}
)
set_target_properties (slib-core
 PROPERTIES
 ARCHIVE_OUTPUT_DIRECTORY "${SLIB_OUTPUT_PATH}"
)

file (GLOB SLIB_NETWORK_FILES
 ${SLIB_PATH}/src/slib/network/*.cpp
)
add_library (
 slib-network STATIC
 ${SLIB_NETWORK_FILES}
)
set_target_properties (slib-network
 PROPERTIES
 ARCHIVE_OUTPUT_DIRECTORY "${SLIB_OUTPUT_PATH}"
)

file (GLOB SLIB_GRAPHICS_FILES
 ${SLIB_PATH}/src/slib/graphics/*.cpp
)
add_library (
 slib-graphics STATIC
 ${SLIB_GRAPHICS_FILES}
)
set_target_properties (slib-graphics
 PROPERTIES
 ARCHIVE_OUTPUT_DIRECTORY "${SLIB_OUTPUT_PATH}"
)

file (GLOB SLIB_RENDER_FILES
 ${SLIB_PATH}/src/slib/render/*.cpp
)
add_library (
 slib-render STATIC
 ${SLIB_RENDER_FILES}
)
set_target_properties (slib-render
 PROPERTIES
 ARCHIVE_OUTPUT_DIRECTORY "${SLIB_OUTPUT_PATH}"
)

file (GLOB SLIB_UI_FILES
 ${SLIB_PATH}/src/slib/ui/*.cpp
)
add_library (
 slib-ui STATIC
 ${SLIB_UI_FILES}
)
set_target_properties (slib-ui
 PROPERTIES
 ARCHIVE_OUTPUT_DIRECTORY "${SLIB_OUTPUT_PATH}"
)

file (GLOB SLIB_MEDIA_FILES
 ${SLIB_PATH}/src/slib/media/*.cpp
)
add_library (
 slib-media STATIC
 ${SLIB_MEDIA_FILES}
)
set_target_properties (slib-media
 PROPERTIES
 ARCHIVE_OUTPUT_DIRECTORY "${SLIB_OUTPUT_PATH}"
)

file (GLOB SLIB_DEVICE_FILES
 ${SLIB_PATH}/src/slib/device/*.cpp
)
add_library (
 slib-device STATIC
 ${SLIB_DEVICE_FILES}
)
set_target_properties (slib-device
 PROPERTIES
 ARCHIVE_OUTPUT_DIRECTORY "${SLIB_OUTPUT_PATH}"
)

file (GLOB SLIB_DB_FILES
 ${SLIB_PATH}/src/slib/db/*.cpp
)
add_library (
 slib-db STATIC
 ${SLIB_DB_FILES}
)
set_target_properties (slib-db
 PROPERTIES
 ARCHIVE_OUTPUT_DIRECTORY "${SLIB_OUTPUT_PATH}"
)

file (GLOB SLIB_WEB_FILES
 ${SLIB_PATH}/src/slib/web/*.cpp
)
add_library (
 slib-web STATIC
 ${SLIB_WEB_FILES}
)
set_target_properties (slib-web
 PROPERTIES
 ARCHIVE_OUTPUT_DIRECTORY "${SLIB_OUTPUT_PATH}"
)

file (GLOB SLIB_GEO_FILES
 ${SLIB_PATH}/src/slib/geo/*.cpp
)
add_library (
 slib-geo STATIC
 ${SLIB_GEO_FILES}
)
set_target_properties (slib-geo
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
set_target_properties (zlib
 PROPERTIES
 ARCHIVE_OUTPUT_DIRECTORY "${SLIB_OUTPUT_PATH}"
)

add_library (
 png STATIC
 ${SLIB_PATH}/src/thirdparty/thirdparty_libpng.c
)
set_target_properties (png
 PROPERTIES
 ARCHIVE_OUTPUT_DIRECTORY "${SLIB_OUTPUT_PATH}"
)

add_library (
 jpeg STATIC
 ${SLIB_PATH}/src/thirdparty/thirdparty_libjpeg.c
)
set_target_properties (jpeg
 PROPERTIES
 ARCHIVE_OUTPUT_DIRECTORY "${SLIB_OUTPUT_PATH}"
)

add_library (
 freetype STATIC
 ${SLIB_PATH}/src/thirdparty/thirdparty_freetype.c
)
set_target_properties (freetype
 PROPERTIES
 ARCHIVE_OUTPUT_DIRECTORY "${SLIB_OUTPUT_PATH}"
)

add_library (
 sqlite3 STATIC
 ${SLIB_PATH}/src/thirdparty/thirdparty_sqlite3.c
)
set_target_properties (sqlite3
 PROPERTIES
 ARCHIVE_OUTPUT_DIRECTORY "${SLIB_OUTPUT_PATH}"
)

add_library (
 opus STATIC
 ${SLIB_PATH}/src/thirdparty/thirdparty_libopus.c
)
set_target_properties (opus
 PROPERTIES
 ARCHIVE_OUTPUT_DIRECTORY "${SLIB_OUTPUT_PATH}"
)

add_library (
 vpx STATIC
 ${SLIB_PATH}/src/thirdparty/thirdparty_libvpx.c
 ${SLIB_PATH}/src/thirdparty/thirdparty_libvpx_enc.c
 ${SLIB_PATH}/src/thirdparty/thirdparty_libvpx_dec.c
)
set_target_properties (vpx
 PROPERTIES
 ARCHIVE_OUTPUT_DIRECTORY "${SLIB_OUTPUT_PATH}"
)

add_library (
 slib_dummy SHARED
 ${CMAKE_CURRENT_LIST_DIR}/dummy.cpp
)

target_link_libraries (
 slib_dummy
 slib-core
 slib-network
 slib-graphics
 slib-render
 slib-ui
 slib-media
 slib-device
 slib-db
 slib-web
 slib-geo
 zlib
 png
 jpeg
 freetype
 sqlite3
 opus
 vpx
)
