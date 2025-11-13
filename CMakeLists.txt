cmake_minimum_required(VERSION 3.26.0 FATAL_ERROR)

project(parallel VERSION 1.0.0 LANGUAGES CXX)

find_package(Boost REQUIRED system)

add_library(parallel STATIC
        thread-pool.cpp
)

target_link_libraries(parallel PRIVATE
        Boost::system
)