cmake_minimum_required(VERSION 3.26.0 FATAL_ERROR)

project(parallel VERSION 1.0.0 LANGUAGES CXX)

find_package(Boost REQUIRED system)

set(CMAKE_CXX_COMPILER g++)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Options to compile in kernel
add_compile_options(
        -Wall
        -fno-builtin
        -nostdlib
        -fno-rtti
        -fno-exceptions
        -std=c++0x
)

add_library(parallel STATIC
        thread.cpp
        thread_group.cpp
        thread_pool.cpp
)

target_link_libraries(parallel PRIVATE
        Boost::system
)