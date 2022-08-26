cmake_minimum_required(VERSION 3.0.0)
project(c_buffer VERSION 1.0.0)
SET(CMAKE_BUILD_TYPE "Release")

include_directories(inc)

add_library(buffer SHARED src/buffer.c)

add_executable(test test.c)

target_link_libraries(test buffer)
