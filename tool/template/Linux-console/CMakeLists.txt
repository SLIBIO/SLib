cmake_minimum_required(VERSION 3.0)

project(SLIB_TEMPLATE_APP_NAME)

include ($ENV{SLIB_PATH}/tool/slib-app.cmake)

add_executable(SLIB_TEMPLATE_APP_NAME
  main.cpp
)

set_target_properties(SLIB_TEMPLATE_APP_NAME PROPERTIES LINK_FLAGS "-static-libgcc -static-libstdc++ -Wl,--wrap=memcpy")

target_link_libraries (
  SLIB_TEMPLATE_APP_NAME
  slib
  pthread
)
