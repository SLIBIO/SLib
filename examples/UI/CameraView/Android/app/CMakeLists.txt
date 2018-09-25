cmake_minimum_required(VERSION 3.4.1)

include (${SLIB_PATH}/tool/slib-app.cmake)

file (GLOB SOURCE_FILES
 ${CMAKE_CURRENT_LIST_DIR}/../../src/*.cpp
)

add_library(native-lib SHARED
 src/main/cpp/native-lib.cpp
 ${SOURCE_FILES}
)

target_link_libraries(native-lib slib log GLESv2)
