cmake_minimum_required(VERSION 3.4)
project(zad2 C)

set(CMAKE_C_FLAGS "-Wall")

add_executable(wrfifo wrfifo.c)
add_executable(rdfifo rdfifo.c)

add_library(main_lib main_lib.c)
target_link_libraries(wrfifo main_lib)
target_link_libraries(rdfifo main_lib)
