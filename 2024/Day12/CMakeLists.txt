cmake_minimum_required(VERSION 3.5.0)
set(TARGET_NAME AdventOfCode-2024-Day12)
project(${TARGET_NAME} VERSION 0.1.0 LANGUAGES C CXX)

add_library(AdventOfCode STATIC ../src/AdventOfCode.cpp)
add_library(Point STATIC ../src/Point.cpp)
add_library(Vector2D STATIC ../src/Vector2D.cpp)
add_executable(${TARGET_NAME} Day12.2.cpp)

target_compile_features(${TARGET_NAME} PRIVATE cxx_std_17)
target_link_libraries(${TARGET_NAME} AdventOfCode Point Vector2D)

set_property(TARGET ${TARGET_NAME} PROPERTY VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/..")