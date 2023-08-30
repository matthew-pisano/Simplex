cmake_minimum_required(VERSION 3.26)
set(CMAKE_CXX_STANDARD 23)
project(Simplex)

add_executable(simplex main.cpp
        src/utils.cpp
        src/utils.h
        src/tokenizer.cpp
        src/tokenizer.h
        src/errors.cpp
        src/errors.h
        # src/compiler.cpp
        # src/compiler.h
        src/common.cpp
        src/common.h
        src/parser.cpp
        src/parser.h)
