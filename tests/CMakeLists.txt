#   SiddiqSoft CMakeLists - Tests
#   Copyright 2024 Abdulkareem Siddiq. All rights reserved.
#   See LICENSE file
#
cmake_minimum_required(VERSION 3.31)

project(timethis_tests
        VERSION         "${CURRENT_PROJECT_VERSION}"
        LANGUAGES       CXX
        DESCRIPTION     "Test suite for timethis")

if(NOT CMAKE_CXX_STANDARD)
    set(CMAKE_CXX_STANDARD 20)
endif()

# Fetch GoogleTest via CPM
if(NOT TARGET GTest::gtest_main)
    CPMAddPackage("gh:google/googletest#v1.17.0")
endif()

file(GLOB_RECURSE TEST_SOURCES CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
)

add_executable(${PROJECT_NAME} ${TEST_SOURCES})
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_${CMAKE_CXX_STANDARD})

target_link_libraries(${PROJECT_NAME}
    PRIVATE
        GTest::gtest_main
        timethis::timethis
)

include(GoogleTest)
gtest_discover_tests(${PROJECT_NAME}
    TEST_PREFIX "${PROJECT_NAME}."
    OUTPUT_DIR "${CMAKE_CURRENT_BINARY_DIR}/results"
    OUTPUT_PREFIX "${CMAKE_BUILD_TYPE}_"
    OUTPUT_SUFFIX ".xml"
)
