cmake_minimum_required(VERSION 3.5.0)

set(SUBDIR_PFX Year.2024.14.)

set(EXE_PFX AdventOfCode-${SUBDIR_PFX})
set(LIB_PFX LIB.${SUBDIR_PFX})
set(TEST_PFX TEST.${SUBDIR_PFX})

set(TARGET_NAME ${EXE_PFX}1)
add_executable(${TARGET_NAME} Day14.1.cpp )
target_link_libraries(${TARGET_NAME} ${YEAR_LIB_PFX}AdventOfCode ${YEAR_LIB_PFX}Point ${YEAR_LIB_PFX}Vector2D ${GLOBAL_LIB_PFX}charmap)
set_cxx_20(${TARGET_NAME})
set_property(TARGET ${TARGET_NAME} PROPERTY VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/.")

set(TARGET_NAME ${EXE_PFX}2)
add_executable(${TARGET_NAME} Day14.2.cpp )
target_link_libraries(${TARGET_NAME} ${YEAR_LIB_PFX}AdventOfCode ${YEAR_LIB_PFX}Point ${YEAR_LIB_PFX}Vector2D ${GLOBAL_LIB_PFX}charmap)
set_cxx_20(${TARGET_NAME})
set_property(TARGET ${TARGET_NAME} PROPERTY VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/.")

