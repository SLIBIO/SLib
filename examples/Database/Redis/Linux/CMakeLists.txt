cmake_minimum_required(VERSION 3.0)

project(RedisClientExample)

include ($ENV{SLIB_PATH}/tool/slib-app.cmake)

add_executable(RedisClientExample
  ../main.cpp
)

set_target_properties(RedisClientExample PROPERTIES LINK_FLAGS "-static-libgcc -static-libstdc++ -Wl,--wrap=memcpy")

target_link_libraries (
  RedisClientExample
  slib
  hiredis
  pthread
)
