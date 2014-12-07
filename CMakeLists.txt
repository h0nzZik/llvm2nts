cmake_minimum_required(VERSION 2.8)

set( CMAKE_EXPORT_COMPILE_COMMANDS 1 )

list( APPEND CMAKE_CXX_FLAGS "-std=c++0x -g -Wall -Wextra")

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "/usr/share/llvm/cmake")

find_package(LLVM REQUIRED CONFIG)

add_definitions(${LLVM_DEFINITIONS})
add_definitions(-D__STDC_CONSTANT_MACROS -D__STDC_LIMIT_MACROS)
include_directories(${LLVM_INCLUDE_DIRS})

add_subdirectory(src)
