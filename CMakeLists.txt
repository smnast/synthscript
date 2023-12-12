cmake_minimum_required(VERSION 3.26)
project(SynthScript)

set(CMAKE_CXX_STANDARD 17)
include_directories(include)

add_executable(SynthScript src/main.cpp
        src/Reader.cpp
        include/Reader.h
        src/Lexer.cpp
        include/Lexer.h
        include/Tokens.h
)
