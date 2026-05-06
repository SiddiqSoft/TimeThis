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

# Platform-specific dependencies
if(CMAKE_CXX_COMPILER_ID MATCHES "Clang|GNU")
    # Linux/Unix specific
    find_package(CURL 8.7 REQUIRED)
    target_link_libraries(${PROJECT_NAME} INTERFACE CURL::libcurl)
elseif(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
    # Windows specific
    CPMAddPackage("gh:SiddiqSoft/acw32h#2.7.4")
    target_link_libraries(${PROJECT_NAME} INTERFACE acw32h::acw32h)
endif()

# Common dependencies
CPMAddPackage("gh:nlohmann/json#v3.12.0")
target_link_libraries(${PROJECT_NAME} INTERFACE nlohmann_json::nlohmann_json)

CPMAddPackage("gh:SiddiqSoft/SplitUri#3.0.3")
target_link_libraries(${PROJECT_NAME} INTERFACE SplitUri::SplitUri)

CPMAddPackage("gh:SiddiqSoft/AzureCppUtils#3.2.5")
target_link_libraries(${PROJECT_NAME} INTERFACE AzureCppUtils::AzureCppUtils)

CPMAddPackage("gh:SiddiqSoft/string2map#2.4.3")
target_link_libraries(${PROJECT_NAME} INTERFACE string2map::string2map)

CPMAddPackage("gh:SiddiqSoft/RunOnEnd#1.4.2")
target_link_libraries(${PROJECT_NAME} INTERFACE RunOnEnd::RunOnEnd)

CPMAddPackage("gh:SiddiqSoft/asynchrony#2.1.1")
target_link_libraries(${PROJECT_NAME} INTERFACE asynchrony::asynchrony)

# Testing configuration
if(${PROJECT_NAME}_BUILD_TESTS AND EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/tests" AND IS_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/tests")
    message(STATUS "${PROJECT_NAME}: Building tests (${PROJECT_NAME}_BUILD_TESTS=ON)")
    enable_testing()
    add_subdirectory(tests)
endif()
