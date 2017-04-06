cmake_minimum_required(VERSION 2.8)

project(slib)

set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -frtti")

set (SLIB_PATH ${CMAKE_CURRENT_LIST_DIR}/../..)
set (SLIB_OUTPUT_PATH ${SLIB_PATH}/lib/Linux/${CMAKE_BUILD_TYPE}-${CMAKE_HOST_SYSTEM_PROCESSOR})

include_directories (
 ${SLIB_PATH}/include
 ${SLIB_PATH}/thirdparty
)

file (
 GLOB SLIB_CORE_FILES
 ${SLIB_PATH}/include/slib/core/*.h 
 ${SLIB_PATH}/src/slib/core/*.cpp
 ${SLIB_PATH}/include/slib/math/*.h
 ${SLIB_PATH}/src/slib/math/*.cpp
 ${SLIB_PATH}/include/slib/crypto/*.h
 ${SLIB_PATH}/src/slib/crypto/*.cpp
)
add_library (slib-core ${SLIB_CORE_FILES})
set_target_properties (slib-core
 PROPERTIES
 ARCHIVE_OUTPUT_DIRECTORY "${SLIB_OUTPUT_PATH}"
)

file (
 GLOB SLIB_NETWORK_FILES
 ${SLIB_PATH}/include/slib/network/*.h
 ${SLIB_PATH}/src/slib/network/*.cpp
)
add_library (slib-network ${SLIB_NETWORK_FILES})
set_target_properties (slib-network
 PROPERTIES
 ARCHIVE_OUTPUT_DIRECTORY "${SLIB_OUTPUT_PATH}"
)

file (
 GLOB SLIB_DB_FILES
 ${SLIB_PATH}/include/slib/db/*.h
 ${SLIB_PATH}/src/slib/db/*.cpp
)
add_library (slib-db ${SLIB_DB_FILES})
set_target_properties (slib-db
 PROPERTIES
 ARCHIVE_OUTPUT_DIRECTORY "${SLIB_OUTPUT_PATH}"
)

file (
 GLOB SLIB_WEB_FILES
 ${SLIB_PATH}/include/slib/web/*.h
 ${SLIB_PATH}/src/slib/web/*.cpp
)
add_library (slib-web ${SLIB_WEB_FILES})
set_target_properties (slib-web
 PROPERTIES
 ARCHIVE_OUTPUT_DIRECTORY "${SLIB_OUTPUT_PATH}"
)

add_library (
 zlib
 ${SLIB_PATH}/src/thirdparty/thirdparty_zlib.c
)
set_target_properties (zlib
 PROPERTIES
 ARCHIVE_OUTPUT_DIRECTORY "${SLIB_OUTPUT_PATH}"
)


add_library (slib
 ${SLIB_CORE_FILES}
 ${SLIB_NETWORK_FILES}
 ${SLIB_DB_FILES}
 ${SLIB_WEB_FILES}
)
set_target_properties (slib
 PROPERTIES
 ARCHIVE_OUTPUT_DIRECTORY "${SLIB_OUTPUT_PATH}"
)
