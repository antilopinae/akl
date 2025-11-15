cmake_minimum_required(VERSION 3.26.0 FATAL_ERROR)

add_library(akl STATIC
        # kernel methods implementation
        __sync.c
        logger.c

        # cpp kernel library
        # atomic.cpp
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
        # kernel methods implementation
        ${CMAKE_CURRENT_SOURCE_DIR}/kern_lib.c
        ${CMAKE_CURRENT_SOURCE_DIR}/__sync.c
        ${CMAKE_CURRENT_SOURCE_DIR}/logger.c

        # support cpp methods implementation
        ${CMAKE_CURRENT_SOURCE_DIR}/operators_support.cpp

        # cpp kernel library
        # ${CMAKE_CURRENT_SOURCE_DIR}/atomic.cpp

        CACHE INTERNAL "Ant kernel lib resources for kernel module" FORCE
)

set(AKL_INCLUDE_DIRS
        akl/
        CACHE INTERNAL "Ant kernel lib include dirs for kernel module" FORCE
)
