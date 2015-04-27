cmake_minimum_required(VERSION 2.8)
project(llvm2nts)

set( CMAKE_EXPORT_COMPILE_COMMANDS 1 )

list( APPEND CMAKE_CXX_FLAGS "-std=c++14 -g -Wall -Wextra -fno-rtti")

find_package(LLVM REQUIRED CONFIG)

add_definitions(${LLVM_DEFINITIONS})
add_definitions(-D__STDC_CONSTANT_MACROS -D__STDC_LIMIT_MACROS)

add_subdirectory(llvm2nts)
add_subdirectory(test_llvm2nts)
add_subdirectory(libNTS)
add_subdirectory(run)
