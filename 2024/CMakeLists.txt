cmake_minimum_required(VERSION 3.5.0)
set(TARGET_NAME AdventOfCode-2024)
project(${TARGET_NAME} VERSION 0.1.0 LANGUAGES CXX)

add_library(AdventOfCode STATIC ./AdventOfCode.cpp)
add_library(Point STATIC ./Point.cpp)
add_library(Vector2D STATIC ./Vector2D.cpp)
add_library(Math STATIC ./Math.cpp)

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
# add_executable(${TARGET_NAME} Day14/Day14.2.cpp)

# Day 15.1
# add_executable(${TARGET_NAME} Day15/Day15.1.cpp)

# Day 15.2
# add_executable(${TARGET_NAME} Day15/Day15.2.cpp)

# Day 16.1
# set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /HEAP:524288000") # Set heap size to 500MB
#add_library(Reindeer STATIC ./Day16/Reindeer.cpp)
#add_library(Compass STATIC ./Day16/Compass.cpp)
#add_library(Maze STATIC ./Day16/Maze.cpp)
#add_library(MazeRunner STATIC ./Day16/MazeRunner.cpp ./Day16/MazeSegment.cpp ./Day16/MazeRunnerPosition.cpp)
#add_executable(${TARGET_NAME} Day16/Day16.1.cpp )
#target_link_libraries(${TARGET_NAME} AdventOfCode Reindeer Compass Maze MazeRunner)
#add_executable(test_compass Day16/Test/test_compass.cpp)
#target_link_libraries(test_compass Compass)
#add_executable(test_maze Day16/Test/test_maze.cpp)
#target_link_libraries(test_maze Maze Compass)
#enable_testing()
#add_test(NAME test_compass_clockwise COMMAND $<TARGET_FILE:test_compass> cw)
#add_test(NAME test_compass_counterclockwise COMMAND $<TARGET_FILE:test_compass> ccw)
#add_test(NAME test_compass_reverse COMMAND $<TARGET_FILE:test_compass> rev)
#add_test(NAME test_compass_equality COMMAND $<TARGET_FILE:test_compass> eq)
#add_test(NAME test_compass_assignment COMMAND $<TARGET_FILE:test_compass> assign)
#add_test(NAME test_compass_references COMMAND $<TARGET_FILE:test_compass> ref)
#add_test(NAME test_maze_assign COMMAND $<TARGET_FILE:test_maze> assign)
#add_test(NAME test_maze_convert COMMAND $<TARGET_FILE:test_maze> convert)
#add_test(NAME test_maze_accessors COMMAND $<TARGET_FILE:test_maze> accessors)
#add_test(NAME test_maze_getters COMMAND $<TARGET_FILE:test_maze> getters)
#add_test(NAME test_maze_setters COMMAND $<TARGET_FILE:test_maze> setters)
#add_test(NAME test_maze_remarkable COMMAND $<TARGET_FILE:test_maze> remarkable)

# Day 18.1
add_library(Map STATIC ./Day18/Map.cpp)
add_library(Compass STATIC ./Day18/Compass.cpp)
add_library(Maze STATIC ./Day18/Maze.cpp)
add_library(MazeRunner STATIC ./Day18/MazeRunner.cpp ./Day18/MazeSegment.cpp ./Day18/MazeRunnerPosition.cpp)
add_executable(${TARGET_NAME} Day18/Day18.1.cpp )
target_link_libraries(${TARGET_NAME} Compass Maze MazeRunner Map)



target_compile_features(${TARGET_NAME} PRIVATE cxx_std_17)
# target_link_libraries(${TARGET_NAME} AdventOfCode Math Point Vector2D)

set_property(TARGET ${TARGET_NAME} PROPERTY VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/.")
