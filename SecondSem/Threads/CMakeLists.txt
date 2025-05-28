cmake_minimum_required(VERSION 3.10)
project(laba7)

set(CMAKE_C_STANDARD 11)

set(CMAKE_C_COMPILER "gcc")
set(CMAKE_CXX_COMPILER "g++")

add_executable(laba7
    src/main.c
    src/basic_threads.c
    src/matrix_multiplication.c
    src/message_queue.c
    src/philosophers.c
)

target_include_directories(laba7 PRIVATE include)
include_directories(/mingw64/include)
link_directories(/mingw64/lib)