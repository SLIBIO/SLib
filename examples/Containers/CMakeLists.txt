cmake_minimum_required(VERSION 3.0)

project(ExampleContainers)

include ($ENV{SLIB_PATH}/tool/slib-app.cmake)

add_executable(ExampleContainers main.cpp)
target_link_libraries (
  ExampleContainers
  slib
  pthread
)
