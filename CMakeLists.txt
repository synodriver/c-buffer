cmake_minimum_required(VERSION 3.20)
project(c_buffer C)
include_directories(src)

set(CMAKE_C_STANDARD 11)

add_library(buffer SHARED src/buffer.c)
add_library(ringbuffer SHARED src/ringbuffer.c)
add_executable(test test.c src/buffer.c)
add_executable(testring testring.c src/ringbuffer.c)