# Copyright (C) 2016 Nikolaj Schepsen

set(CMAKE_DISABLE_IN_SOURCE_BUILD ON)

project("Bencode!PARSER" CXX)

# REQUIREMENTS
cmake_minimum_required(VERSION 3.0.0 FATAL_ERROR)

set(CMAKE_CXX_FLAGS "-std=c++11 -O3 -g -Wall")

# SUBFOLDERS
add_subdirectory(tests)
# BUILD
add_executable(unittest tests/unit.cpp)

# Copyright (C) 2016 Nikolaj Schepsen