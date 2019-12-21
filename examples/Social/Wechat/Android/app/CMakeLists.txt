cmake_minimum_required(VERSION 3.4.1)

include (${SLIB_PATH}/tool/slib-app.cmake)

set (SOURCE_FILES
	"${CMAKE_CURRENT_LIST_DIR}/src/main/cpp/native-lib.cpp"

	"${CMAKE_CURRENT_LIST_DIR}/../../src/gen/colors.cpp"
	"${CMAKE_CURRENT_LIST_DIR}/../../src/gen/drawables.cpp"
	"${CMAKE_CURRENT_LIST_DIR}/../../src/gen/layouts.cpp"
	"${CMAKE_CURRENT_LIST_DIR}/../../src/gen/menus.cpp"
	"${CMAKE_CURRENT_LIST_DIR}/../../src/gen/raws.cpp"
	"${CMAKE_CURRENT_LIST_DIR}/../../src/gen/strings.cpp"

	"${CMAKE_CURRENT_LIST_DIR}/../../src/cpp/app.cpp"
	"${CMAKE_CURRENT_LIST_DIR}/../../src/cpp/MainPage.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/../../src/cpp/PayPage.cpp"
)

add_library(native-lib SHARED
	${SOURCE_FILES}
)

target_link_libraries(native-lib slib log GLESv2)
