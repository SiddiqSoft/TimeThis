cmake_minimum_required(VERSION 3.25)

project(${PROJECT_NAME}tests
		LANGUAGES CXX C
		)
# Require C++20
message(STATUS "Require C++20..${${PROJECT_NAME}tests}")
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

#find_package(${PROJECT_NAME}_TARGET_NAME)
enable_testing()

# Declare our test source files.
file(GLOB siddiqsoft_ho_tests "*.cpp")

add_executable(${${PROJECT_NAME}tests} siddiqsoft_ho_tests)

target_link_libraries(${${PROJECT_NAME}tests} PRIVATE GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(${${PROJECT_NAME}tests}
                     XML_OUTPUT_DIR "${PROJECT_SOURCE_DIR}/tests/results"
)
