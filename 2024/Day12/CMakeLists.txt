cmake_minimum_required(VERSION 3.5.0)
set(TARGET_NAME AdventOfCode-2024-Day12)
project(${TARGET_NAME} VERSION 0.1.0 LANGUAGES C CXX)

add_library(AdventOfCode STATIC ../lib/AdventOfCode.c++)
add_library(Point STATIC ../lib/Point.c++)
add_executable(${TARGET_NAME} Day12.c++)

target_compile_features(${TARGET_NAME} PRIVATE cxx_std_17)
target_link_libraries(${TARGET_NAME} AdventOfCode Point)

set_property(TARGET ${TARGET_NAME} PROPERTY VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/..")