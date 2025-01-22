cmake_minimum_required(VERSION 3.5.0)

set(SUBDIR_PFX 2024.09.)

set(EXE_PFX AdventOfCode-${SUBDIR_PFX})
set(LIB_PFX LIB.${SUBDIR_PFX})
set(TEST_PFX TEST.${SUBDIR_PFX})

set(TARGET_NAME ${EXE_PFX}2)
add_executable(${TARGET_NAME} Day9.2.cpp )
target_link_libraries(${TARGET_NAME} ${YEAR_LIB_PFX}AdventOfCode )
set_cxx_20(${TARGET_NAME})
set_property(TARGET ${TARGET_NAME} PROPERTY VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/.")
