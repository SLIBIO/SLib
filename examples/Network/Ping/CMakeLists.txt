cmake_minimum_required(VERSION 3.0)

project(ExamplePing)

include ($ENV{SLIB_PATH}/tool/slib-app.cmake)

add_executable(ExamplePing main.cpp)

set_target_properties(ExamplePing PROPERTIES LINK_FLAGS "-static-libgcc -static-libstdc++ -Wl,--wrap=memcpy")

target_link_libraries (
  ExamplePing
  slib
  pthread
)
