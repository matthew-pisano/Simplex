cmake_minimum_required(VERSION 3.26)
set(CMAKE_CXX_STANDARD 23)
project(Simplex)

include_directories(include lib)

add_executable(simplex src/main.cpp
        src/utils.cpp
        src/tokenizer.cpp
        src/errors.cpp
        # src/compiler.cpp
        # src/compiler.h
        src/common.cpp
        src/parser.cpp)
