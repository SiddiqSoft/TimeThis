cmake_minimum_required(VERSION 3.25)

# Require C++20
message(STATUS "Require C++20..")
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

find_package(Git REQUIRED)
include(FetchContent)
FetchContent_Declare(googletest
					GIT_REPOSITORY https://github.com/google/googletest.git
					GIT_TAG	v1.15.2
					FIND_PACKAGE_ARGS NAMES GTest
)
FetchContent_MakeAvailable(googletest)

# Declare our test source files.
file(GLOB testSRC "./test.cpp")

add_executable(TimeThis_test ${testSRC})

target_link_libraries(TimeThis_test PRIVATE GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(TimeThis_test
                     XML_OUTPUT_DIR "${PROJECT_SOURCE_DIR}/tests/results"
)
