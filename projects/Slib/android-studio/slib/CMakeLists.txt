cmake_minimum_required(VERSION 3.4.1)

file (GLOB SLIB_FILES
    ${CMAKE_CURRENT_LIST_DIR}/../../../../src/slib/core/*.cpp
    ${CMAKE_CURRENT_LIST_DIR}/../../../../src/slib/math/*.cpp
    ${CMAKE_CURRENT_LIST_DIR}/../../../../src/slib/crypto/*.cpp
    ${CMAKE_CURRENT_LIST_DIR}/../../../../src/slib/network/*.cpp
    ${CMAKE_CURRENT_LIST_DIR}/../../../../src/slib/graphics/*.cpp
    ${CMAKE_CURRENT_LIST_DIR}/../../../../src/slib/render/*.cpp
    ${CMAKE_CURRENT_LIST_DIR}/../../../../src/slib/ui/*.cpp
    ${CMAKE_CURRENT_LIST_DIR}/../../../../src/slib/media/*.cpp
    ${CMAKE_CURRENT_LIST_DIR}/../../../../src/slib/device/*.cpp
    ${CMAKE_CURRENT_LIST_DIR}/../../../../src/slib/db/*.cpp
    ${CMAKE_CURRENT_LIST_DIR}/../../../../src/slib/map/*.cpp
    ${CMAKE_CURRENT_LIST_DIR}/../../../../src/slib/web/*.cpp
)

set (THIRDPARTY_FILES
 ${CMAKE_CURRENT_LIST_DIR}/../../../../src/thirdparty/thirdparty_libjpeg.c
 ${CMAKE_CURRENT_LIST_DIR}/../../../../src/thirdparty/thirdparty_libpng.c
 ${CMAKE_CURRENT_LIST_DIR}/../../../../src/thirdparty/thirdparty_zlib.c
 ${CMAKE_CURRENT_LIST_DIR}/../../../../src/thirdparty/thirdparty_freetype.c
 ${CMAKE_CURRENT_LIST_DIR}/../../../../src/thirdparty/thirdparty_sqlite3.c
)

if (ANDROID_ABI STREQUAL "armeabi-v7a")
 set (THIRDPARTY_NEON_FILES
  ${CMAKE_CURRENT_LIST_DIR}/../../../../src/thirdparty/thirdparty_libopus.c
  ${CMAKE_CURRENT_LIST_DIR}/../../../../src/thirdparty/thirdparty_libvpx.c
  ${CMAKE_CURRENT_LIST_DIR}/../../../../src/thirdparty/thirdparty_libvpx_enc.c
  ${CMAKE_CURRENT_LIST_DIR}/../../../../src/thirdparty/thirdparty_libvpx_dec.c
 )
 if (CMAKE_BUILD_TYPE MATCHES Debug)
  set_source_files_properties(${THIRDPARTY_NEON_FILES} PROPERTIES COMPILE_FLAGS -O3)
 endif ()
 set_property(SOURCE ${THIRDPARTY_NEON_FILES} APPEND_STRING PROPERTY COMPILE_FLAGS " -mfpu=neon")
 add_definitions("-DHAVE_NEON=1")
else ()
 set (THIRDPARTY_NEON_FILES)
 set (THIRDPARTY_FILES ${THIRDPARTY_FILES}
  ${CMAKE_CURRENT_LIST_DIR}/../../../../src/thirdparty/thirdparty_libopus.c
  ${CMAKE_CURRENT_LIST_DIR}/../../../../src/thirdparty/thirdparty_libvpx.c
  ${CMAKE_CURRENT_LIST_DIR}/../../../../src/thirdparty/thirdparty_libvpx_enc.c
  ${CMAKE_CURRENT_LIST_DIR}/../../../../src/thirdparty/thirdparty_libvpx_dec.c
 )
endif ()
if (CMAKE_BUILD_TYPE MATCHES Debug)
 set_source_files_properties(${THIRDPARTY_FILES} PROPERTIES COMPILE_FLAGS -O3)
endif ()

add_library(
 slib STATIC
 ${SLIB_FILES}
 ${THIRDPARTY_FILES}
 ${THIRDPARTY_NEON_FILES}
)
