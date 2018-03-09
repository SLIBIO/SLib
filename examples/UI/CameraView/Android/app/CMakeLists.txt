cmake_minimum_required(VERSION 3.4.1)

include_directories (${SLIB_PATH}/include)
link_directories (${SLIB_PATH}/lib/Android/${CMAKE_BUILD_TYPE}-${ANDROID_ABI})

file (GLOB SOURCE_FILES
 ${CMAKE_CURRENT_LIST_DIR}/../../src/*.cpp
)

add_library(native-lib SHARED
 src/main/cpp/native-lib.cpp
 ${SOURCE_FILES}
)

target_link_libraries(native-lib slib log GLESv2)
