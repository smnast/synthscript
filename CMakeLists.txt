cmake_minimum_required(VERSION 3.26)
project(SynthScript)

set(CMAKE_CXX_STANDARD 17)

# Include Doctest
include(FetchContent)

FetchContent_Declare(
    doctest
    GIT_REPOSITORY https://github.com/doctest/doctest.git
    GIT_TAG        master
)

FetchContent_MakeAvailable(doctest)

add_subdirectory(src)

enable_testing()

add_subdirectory(tests)
