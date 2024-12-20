cmake_minimum_required(VERSION 3.5.0)
project(2024-Day12 VERSION 0.1.0 LANGUAGES C CXX)

add_library(AdventOfCode STATIC ../lib/AdventOfCode.c++)
add_library(Point STATIC ../lib/Point.c++)
add_executable(2024-Day12 Day12.cpp)

target_link_libraries(2024-Day12 AdventOfCode Point)

set_property(TARGET 2024-Day12 PROPERTY VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/..")