cmake_minimum_required(VERSION 3.5.0)
#set(TARGET_NAME AdventOfCode-2024)
#project(${TARGET_NAME} VERSION 0.1.0 LANGUAGES CXX)

add_library(2024.AdventOfCode STATIC ./AdventOfCode.cpp)
add_library(2024.Point STATIC ./Point.cpp)
add_library(2024.Vector2D STATIC ./Vector2D.cpp)
add_library(2024.Math STATIC ./Math.cpp)

# Day 9.2
add_subdirectory(Day9)

# Day 12
add_subdirectory(Day12)

# Day 13
add_subdirectory(Day13)

# Day 14
add_subdirectory(Day14)

# Day 15
add_subdirectory(Day15)

# Day 16
add_subdirectory(Day16)

# Day 18
add_subdirectory(Day18)