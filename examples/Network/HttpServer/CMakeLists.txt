cmake_minimum_required(VERSION 3.0)

project(ExampleHttpServer)

include ($ENV{SLIB_PATH}/include/slib-app.cmake)

add_executable(ExampleHttpServer main.cpp)
target_link_libraries (
  ExampleHttpServer
  slib
  zlib
  pthread
)
