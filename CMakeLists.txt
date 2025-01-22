cmake_minimum_required(VERSION 3.5.0)
set(PROJECT_NAME AdventOfCode-2024)
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

enable_testing()
add_subdirectory(lib)
add_subdirectory(2023)
add_subdirectory(2024)

#target_compile_features(${TARGET_NAME} PRIVATE cxx_std_20)
#set_property(TARGET ${TARGET_NAME} PROPERTY CXX_STANDARD 20)
#set_property(TARGET ${TARGET_NAME} PROPERTY CXX_STANDARD_REQUIRED ON)
#set_property(TARGET ${TARGET_NAME} PROPERTY VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/.")
