cmake_minimum_required(VERSION 3.27)

project(MTG_pathfind_debugExport)

add_executable(MTG_pathfind_debugExport ../../src/PathFinding.cpp ./main.cpp)
target_compile_definitions(MTG_pathfind_debugExport PRIVATE DEBUG_EXPORT)

set(CMAKE_CPP_FLAGS "${CMAKE_CPP_FLAGS} -std=c++23 -Wall -Wextra -Wpedantic -g3 -O0")
