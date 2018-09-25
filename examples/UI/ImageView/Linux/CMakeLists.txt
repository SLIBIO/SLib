cmake_minimum_required(VERSION 3.0)

project(ExampleImageView)

include ($ENV{SLIB_PATH}/tool/slib-app.cmake)

add_executable(ExampleImageView
  ../src/app.cpp
  main.cpp
)

set_target_properties(ExampleImageView PROPERTIES LINK_FLAGS "-static-libgcc -static-libstdc++ -Wl,--wrap=memcpy")

target_link_libraries (
  ExampleImageView
  slib
  pthread
  gtk-x11-2.0
  gdk-x11-2.0
  gobject-2.0
  glib-2.0
  cairo
  X11
  GL
)

file(COPY ../images DESTINATION .)
