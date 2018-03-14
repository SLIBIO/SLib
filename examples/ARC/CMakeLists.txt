cmake_minimum_required(VERSION 2.6)

project(ExampleARC)

set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set (SLIB_PATH $ENV{SLIB_PATH})
include_directories (${SLIB_PATH}/include) 
link_directories(${SLIB_PATH}/lib/Linux/${CMAKE_BUILD_TYPE}-${CMAKE_HOST_SYSTEM_PROCESSOR})

add_executable(ExampleARC main.cpp)

target_link_libraries (
  ExampleARC
  slib
  pthread
)
