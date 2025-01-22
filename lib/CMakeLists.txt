cmake_minimum_required(VERSION 3.5.0)

# GLOBAL_LIB_PFX and GLOBAL_TEST_PFX are set at the level above
# message(GLOBAL_LIB_PFX: ${GLOBAL_LIB_PFX})
# message(GLOBAL_TEST_PFX: ${GLOBAL_TEST_PFX})

add_library(${GLOBAL_LIB_PFX}charmap STATIC ./charmap.cpp ./charmap.h)
add_library(${GLOBAL_LIB_PFX}math STATIC ./math.cpp ./math.h)

enable_testing()

set(TARGET_NAME ${GLOBAL_TEST_PFX}charmap)
add_executable(${TARGET_NAME} test/test_charmap.cpp)
target_link_libraries(${TARGET_NAME} ${GLOBAL_LIB_PFX}charmap)
set_cxx_20(${TARGET_NAME})
add_test(NAME ${GLOBAL_TEST_PFX}charmap_xxx COMMAND $<TARGET_FILE:${TARGET_NAME}> xxx)

set(TARGET_NAME ${GLOBAL_TEST_PFX}math)
add_executable(${TARGET_NAME} test/test_math.cpp)
target_link_libraries(${TARGET_NAME} ${GLOBAL_LIB_PFX}math)
set_cxx_20(${TARGET_NAME})
add_test(NAME ${GLOBAL_TEST_PFX}math_xxx COMMAND $<TARGET_FILE:${TARGET_NAME}> xxx)
