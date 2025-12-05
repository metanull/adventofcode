cmake_minimum_required(VERSION 3.5.0)
set(PROJECT_NAME AdventOfCode-Havelange-Pascal)
project(${PROJECT_NAME} VERSION 0.1.0 LANGUAGES CXX)

function(set_cxx_17 TARGET_NAME)
    target_compile_features(${TARGET_NAME} PRIVATE cxx_std_17)
    set_property(TARGET ${TARGET_NAME} PROPERTY CXX_STANDARD 17)
    set_property(TARGET ${TARGET_NAME} PROPERTY CXX_STANDARD_REQUIRED ON)
endfunction()

function(set_cxx_20 TARGET_NAME)
    target_compile_features(${TARGET_NAME} PRIVATE cxx_std_20)
    set_property(TARGET ${TARGET_NAME} PROPERTY CXX_STANDARD 20)
    set_property(TARGET ${TARGET_NAME} PROPERTY CXX_STANDARD_REQUIRED ON)
endfunction()

enable_testing()

# Setting variables to faciliate referenciation of the GLOBAL_LIB by other projects
set(GLOBAL_LIB_PFX LIB.Global.)
set(GLOBAL_TEST_PFX TEST.Global.)
set(GLOBAL_LIB_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/lib)
# message("GLOBAL_LIB_PFX: ${GLOBAL_LIB_PFX}")
# message("GLOBAL_LIB_INCLUDE_DIR: ${GLOBAL_LIB_INCLUDE_DIR}")

# Adding the GLOBAL_LIB dir to the includes to faciliate usage by other projects
# MSVC + VSCode, couldn't make it work just 
# include_directories(AFTER ${GLOBAL_LIB_INCLUDE_DIR}). 
# It seems that as soon as include_directories or target_include_directories is called, the default includes are lost ")
# Because of this I had to use the environment variable INCLUDE
# This statement permits #include "name_of_global_lib.h" to work, without worrying about the path to the Global Library
include_directories($ENV{INCLUDE} ${GLOBAL_LIB_INCLUDE_DIR})

# Build the Global Library
add_subdirectory(lib)

# Build the individual projects
add_subdirectory(2023)
add_subdirectory(2024)
add_subdirectory(2025)
