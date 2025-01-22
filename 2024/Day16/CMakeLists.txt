cmake_minimum_required(VERSION 3.5.0)

set(SUBDIR_PFX 2024.16.)

set(EXE_PFX AdventOfCode-${SUBDIR_PFX})
set(LIB_PFX LIB.${SUBDIR_PFX})
set(TEST_PFX TEST.${SUBDIR_PFX})

add_library(${LIB_PFX}Reindeer STATIC ./Reindeer.cpp)
add_library(${LIB_PFX}Compass STATIC ./Compass.cpp)
add_library(${LIB_PFX}Maze STATIC ./Maze.cpp)
add_library(${LIB_PFX}MazeRunner STATIC ./MazeRunner.cpp ./MazeSegment.cpp ./MazeRunnerPosition.cpp)

enable_testing()

set(TARGET_NAME ${TEST_PFX}compass)
add_executable(${TARGET_NAME} Test/test_compass.cpp)
target_link_libraries(${TARGET_NAME} ${LIB_PFX}Compass)
set_cxx_20(${TARGET_NAME})
add_test(NAME ${TEST_PFX}compass_clockwise COMMAND $<TARGET_FILE:${TARGET_NAME}> cw)
add_test(NAME ${TEST_PFX}compass_counterclockwise COMMAND $<TARGET_FILE:${TARGET_NAME}> ccw)
add_test(NAME ${TEST_PFX}compass_reverse COMMAND $<TARGET_FILE:${TARGET_NAME}> rev)
add_test(NAME ${TEST_PFX}compass_equality COMMAND $<TARGET_FILE:${TARGET_NAME}> eq)
add_test(NAME ${TEST_PFX}compass_assignment COMMAND $<TARGET_FILE:${TARGET_NAME}> assign)
add_test(NAME ${TEST_PFX}compass_references COMMAND $<TARGET_FILE:${TARGET_NAME}> ref)

set(TARGET_NAME ${TEST_PFX}maze)
add_executable(${TARGET_NAME} Test/test_maze.cpp)
target_link_libraries(${TARGET_NAME} ${LIB_PFX}Compass ${LIB_PFX}Maze)
set_cxx_20(${TARGET_NAME})
add_test(NAME ${TEST_PFX}maze_assign COMMAND $<TARGET_FILE:${TARGET_NAME}> assign)
add_test(NAME ${TEST_PFX}maze_convert COMMAND $<TARGET_FILE:${TARGET_NAME}> convert)
add_test(NAME ${TEST_PFX}maze_accessors COMMAND $<TARGET_FILE:${TARGET_NAME}> accessors)
add_test(NAME ${TEST_PFX}maze_getters COMMAND $<TARGET_FILE:${TARGET_NAME}> getters)
add_test(NAME ${TEST_PFX}maze_setters COMMAND $<TARGET_FILE:${TARGET_NAME}> setters)
add_test(NAME ${TEST_PFX}maze_remarkable COMMAND $<TARGET_FILE:${TARGET_NAME}> remarkable)

set(TARGET_NAME ${EXE_PFX}1)
add_executable(${TARGET_NAME} Day16.1.cpp )
target_link_libraries(${TARGET_NAME} ${YEAR_LIB_PFX}AdventOfCode ${LIB_PFX}Reindeer ${LIB_PFX}Compass ${LIB_PFX}Maze ${LIB_PFX}MazeRunner)
set_cxx_20(${TARGET_NAME})
set_property(TARGET ${TARGET_NAME} PROPERTY VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/.")

