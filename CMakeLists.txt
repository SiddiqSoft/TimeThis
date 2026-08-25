#   SiddiqSoft CMakeLists 
#   Copyright 2024 Abdulkareem Siddiq. All rights reserved.
#   See LICENSE file
#
#   When building from our CI server, the following options must be set:
#   - timethis_BUILD_TESTS=ON
#   - CI_BUILDID must be set to the gitversion
# 
cmake_minimum_required(VERSION 3.29)

# Version detection must precede project() declaration
# Figures out ${CURRENT_PROJECT_VERSION} and brings in CPM module
include(pack/CMakeCommonHelpers.cmake)

# Project metadata
project(timethis
        VERSION         "${CURRENT_PROJECT_VERSION}"
        LANGUAGES       CXX
        HOMEPAGE_URL    "%%myurl%%"
        DESCRIPTION     "Calculate time taken for given context")

# Set C++ standard globally
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Build options
option(${PROJECT_NAME}_BUILD_TESTS "Build tests for ${PROJECT_NAME}" OFF)
option(CI_BUILDID "Git version for CI builds" "${CURRENT_PROJECT_VERSION}")

# Library definition
add_library(${PROJECT_NAME} INTERFACE)
add_library(${PROJECT_NAME}::${PROJECT_NAME} ALIAS ${PROJECT_NAME})

# Include directories
target_include_directories(${PROJECT_NAME}
    INTERFACE
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>)

# Compile features and options
target_compile_features(${PROJECT_NAME} INTERFACE cxx_std_23)

# Clang-specific options for experimental library features
target_compile_options(${PROJECT_NAME}
    INTERFACE
    $<$<CXX_COMPILER_ID:Clang>:-fexperimental-library>)

# ____________________________
# Debugger visualization (natvis)
#
# - Visual Studio / MSVC generators: .natvis files attached as INTERFACE sources
#   are loaded automatically by the debugger.
# - VS Code consumers (cpptools "cppdbg" or lldb-dap): point "visualizerFile"
#   in launch.json at the file, e.g. when consuming via CPM/FetchContent:
#       "visualizerFile": "${timethis_SOURCE_DIR}/timethis.natvis"
#   or query the target property after find_package():
#       get_target_property(_natvis timethis::timethis NATVIS_FILE)
set(${PROJECT_NAME}_NATVIS
    "${CMAKE_CURRENT_SOURCE_DIR}/timethis.natvis"
    CACHE INTERNAL "Path to the ${PROJECT_NAME} debugger visualization (natvis) file")

# Attach the visualizer to the target (BUILD_INTERFACE keeps installed/exported
# targets free of absolute paths while in-tree builds get the benefit).
target_sources(${PROJECT_NAME}
    INTERFACE
    $<BUILD_INTERFACE:${${PROJECT_NAME}_NATVIS}>)

# Expose the path as a target property so find_package() consumers can query it:
#   get_target_property(_natvis timethis::timethis NATVIS_FILE)
set_target_properties(${PROJECT_NAME} PROPERTIES
    NATVIS_FILE "${${PROJECT_NAME}_NATVIS}")

message(STATUS "${PROJECT_NAME}: natvis visualizer --> ${${PROJECT_NAME}_NATVIS}")

# Testing configuration
if(${PROJECT_NAME}_BUILD_TESTS AND EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/tests" AND IS_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/tests")
    message(STATUS "${PROJECT_NAME}: Building tests (${PROJECT_NAME}_BUILD_TESTS=ON)")
    enable_testing()
    add_subdirectory(tests)
endif()
