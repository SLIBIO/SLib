cmake_minimum_required(VERSION 2.6)

project(ExampleARC)

include ($ENV{SLIB_PATH}/build/CMake/include.cmake)

add_executable(ExampleARC main.cpp)
target_link_libraries (
  ExampleARC
  slib
  pthread
)
