cmake_minimum_required(VERSION 3.5.0)
set(TARGET_NAME AdventOfCode)
project(${TARGET_NAME} VERSION 0.1.0 LANGUAGES CXX)

add_subdirectory(2024)target_compile_features(${TARGET_NAME} PRIVATE cxx_std_17)
set_property(TARGET ${TARGET_NAME} PROPERTY VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/.")
