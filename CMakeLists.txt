cmake_minimum_required(VERSION 3.26)
project(Simplex)

set(CMAKE_CXX_STANDARD 23)

add_executable(simplex main.cpp
        src/utils.cpp
        src/utils.h
        src/tokenizer.cpp
        src/tokenizer.h
        src/errors.cpp
        src/errors.h
        src/compiler.cpp
        src/compiler.h)
