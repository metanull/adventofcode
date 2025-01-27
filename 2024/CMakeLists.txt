cmake_minimum_required(VERSION 3.5.0)

add_library(${YEAR_LIB_PFX}AdventOfCode STATIC ./AdventOfCode.cpp)
add_library(${YEAR_LIB_PFX}Point STATIC ./Point.cpp)
add_library(${YEAR_LIB_PFX}Vector2D STATIC ./Vector2D.cpp)

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
#add_subdirectory(Day16)
add_subdirectory(Day16-v2)

# Day 18
add_subdirectory(Day18)