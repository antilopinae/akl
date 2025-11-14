cmake_minimum_required(VERSION 3.26.0 FATAL_ERROR)

add_library(akl STATIC
        atomic.cpp
)

target_compile_options(akl PRIVATE
        -Wall
        -fno-builtin
        -nostdlib
        -fno-rtti
        -fno-exceptions
        -std=c++0x
)

set(AKL_KERNEL_MODULE_SRC
        ${CMAKE_CURRENT_SOURCE_DIR}/atomic.cpp
        CACHE INTERNAL "Ant kernel lib resources for kernel module" FORCE
)
