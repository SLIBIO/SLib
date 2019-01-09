cmake_minimum_required(VERSION 3.4.1)

include (${SLIB_PATH}/tool/slib-app.cmake)

file (GLOB SOURCE_FILES
        "${CMAKE_CURRENT_LIST_DIR}/src/main/cpp/native-lib.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/../../src/gen/*.cpp"

        "${CMAKE_CURRENT_LIST_DIR}/../../src/cpp/app.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/../../src/cpp/MainMenu.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/../../src/cpp/LaunchScreen.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/../../src/cpp/ControlScreenPage.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/../../src/cpp/QRCodeScannerPage.cpp"
)

add_library(native-lib SHARED
 ${SOURCE_FILES}
)

target_link_libraries(native-lib slib log GLESv2 zxing)
