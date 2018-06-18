set (SLIB_BASE_PATH "${CMAKE_CURRENT_LIST_DIR}/../..")

include_directories ("${SLIB_BASE_PATH}/include")

set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -frtti")
if (ANDROID)
 link_directories ("${SLIB_BASE_PATH}/lib/Android/${CMAKE_BUILD_TYPE}-${ANDROID_ABI}")
else ()
 if (CMAKE_SYSTEM_NAME STREQUAL Linux)
  link_directories("${SLIB_BASE_PATH}/lib/Linux/${CMAKE_BUILD_TYPE}-${CMAKE_SYSTEM_PROCESSOR}")
 else()
  link_directories("${SLIB_BASE_PATH}/lib/CMake/${CMAKE_BUILD_TYPE}-${CMAKE_SYSTEM_PROCESSOR}")
 endif()
endif()
