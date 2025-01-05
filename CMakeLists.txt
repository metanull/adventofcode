cmake_minimum_required(VERSION 3.5.0)
project(global LANGUAGES CXX)

include(ExternalProject)
ExternalProject_Add(
    Day12
    PREFIX $(CMAKE_BINARY_DIR)/Day12
    SOURCE_DIR $(CMAKE_CURRENT_SOURCE_DIR)/Day12
    CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=$(CMAKE_INSTALL_PREFIX)
    BUILD_COMMAND make
    INSTALL_COMMAND make install
)

include(ExternalProject)
ExternalProject_Add(
    Day12.2
    PREFIX $(CMAKE_BINARY_DIR)/Day12.2
    SOURCE_DIR $(CMAKE_CURRENT_SOURCE_DIR)/Day12.2
    CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=$(CMAKE_INSTALL_PREFIX)
    BUILD_COMMAND make
    INSTALL_COMMAND make install
)

include(ExternalProject)
ExternalProject_Add(
    Day13.2
    PREFIX $(CMAKE_BINARY_DIR)/Day13
    SOURCE_DIR $(CMAKE_CURRENT_SOURCE_DIR)/Day13
    CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=$(CMAKE_INSTALL_PREFIX)
    BUILD_COMMAND make
    INSTALL_COMMAND make install
)

include(ExternalProject)
ExternalProject_Add(
    Day13.thread
    PREFIX $(CMAKE_BINARY_DIR)/Day13.thread
    SOURCE_DIR $(CMAKE_CURRENT_SOURCE_DIR)/Day13.thread
    CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=$(CMAKE_INSTALL_PREFIX)
    BUILD_COMMAND make
    INSTALL_COMMAND make install
)
