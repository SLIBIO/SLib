cmake_minimum_required(VERSION 3.0)

project(ExampleUdpHolepunching)

include ($ENV{SLIB_PATH}/tool/slib-app.cmake)

add_executable(ExampleUdpHolepunching main.cpp)

set_target_properties(ExampleUdpHolepunching PROPERTIES LINK_FLAGS "-static-libgcc -static-libstdc++ -Wl,--wrap=memcpy")

target_link_libraries (
  ExampleUdpHolepunching
  slib-core
  zlib
  pthread
)
