cmake_minimum_required(VERSION 3.5.0)
set(TARGET_NAME AdventOfCode-2024)
project(${TARGET_NAME} VERSION 0.1.0 LANGUAGES CXX)

add_library(AdventOfCode STATIC ./AdventOfCode.cpp)
add_library(Point STATIC ./Point.cpp)
add_library(Vector2D STATIC ./Vector2D.cpp)

# Day 9.2
# add_executable(${TARGET_NAME} Day9/Day9.2.cpp)

# Day 12.1
# add_executable(${TARGET_NAME} Day12/Day12.cpp)

# Day 12.2
# add_executable(${TARGET_NAME} Day12.2/Day12.2.cpp)

# Day 13.1
# add_executable(${TARGET_NAME} Day13/Day13.2.cpp)

# Day 13.2
# add_executable(${TARGET_NAME} Day13.thread/Day13.2.thread.cpp)

# Day 14.1
# add_executable(${TARGET_NAME} Day14/Day14.1.cpp)

# Day 14.2
add_executable(${TARGET_NAME} Day14/Day14.2.cpp)

target_compile_features(${TARGET_NAME} PRIVATE cxx_std_17)
target_link_libraries(${TARGET_NAME} AdventOfCode Point Vector2D)

set_property(TARGET ${TARGET_NAME} PROPERTY VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/.")
