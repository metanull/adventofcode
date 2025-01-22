cmake_minimum_required(VERSION 3.5.0)

set(SUBDIR_PFX "Global.")

set(EXE_PFX AdventOfCode-${SUBDIR_PFX})
set(LIB_PFX LIB.${SUBDIR_PFX})
set(TEST_PFX TEST.${SUBDIR_PFX})

add_library(${LIB_PFX}charmap STATIC ./charmap.cpp)
add_library(${LIB_PFX}math STATIC ./math.cpp)

enable_testing()

set(TARGET_NAME ${TEST_PFX}charmap)
add_executable(${TARGET_NAME} test/test_charmap.cpp)
target_link_libraries(${TARGET_NAME} ${LIB_PFX}charmap)
set_cxx_20(${TARGET_NAME})
add_test(NAME ${TEST_PFX}charmap_xxx COMMAND $<TARGET_FILE:${TARGET_NAME}> xxx)

set(TARGET_NAME ${TEST_PFX}math)
add_executable(${TARGET_NAME} test/test_math.cpp)
target_link_libraries(${TARGET_NAME} ${LIB_PFX}math)
set_cxx_20(${TARGET_NAME})
add_test(NAME ${TEST_PFX}math_xxx COMMAND $<TARGET_FILE:${TARGET_NAME}> xxx)
