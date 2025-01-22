cmake_minimum_required(VERSION 3.5.0)

set(SUBDIR_PFX Year.2024.18.)

set(EXE_PFX AdventOfCode-${SUBDIR_PFX})
set(LIB_PFX LIB.${SUBDIR_PFX})
set(TEST_PFX TEST.${SUBDIR_PFX})

add_library(${LIB_PFX}Compass STATIC ./Compass.cpp)
add_library(${LIB_PFX}Maze STATIC ./Maze.cpp)
add_library(${LIB_PFX}MazeRunner STATIC ./MazeRunner.cpp ./MazeSegment.cpp ./MazeRunnerPosition.cpp)

message($<TARGET_FILE:${GLOBAL_LIB_PFX}charmap>)
set(TARGET_NAME ${EXE_PFX}1)
add_executable(${TARGET_NAME} Day18.1.cpp )
target_link_libraries(${TARGET_NAME} ${LIB_PFX}Compass ${LIB_PFX}Maze ${LIB_PFX}MazeRunner ${GLOBAL_LIB_PFX}charmap)
set_cxx_20(${TARGET_NAME})
set_property(TARGET ${TARGET_NAME} PROPERTY VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/.")
