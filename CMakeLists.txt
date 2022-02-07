cmake_minimum_required(VERSION 3.13)

project(
    options
    VERSION 1.0.0
    LANGUAGES CXX)

option(USE_TESTS "Build tests" OFF)
option(USE_EXAMPLE "Build example" OFF)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON) # ensure -std=c++...
set(CMAKE_CXX_EXTENSIONS OFF) # avoid -std=g++...

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_EXPORT_COMPILE_COMMANDS On)

# use ccache if found
find_program(CCACHE_PROGRAM ccache)
if(CCACHE_PROGRAM)
    set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE ${CCACHE_PROGRAM})
endif()

add_subdirectory(src)
