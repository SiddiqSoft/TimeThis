cmake_minimum_required(VERSION 3.25.0)

# Require C++23
message(STATUS "Require C++20 (tests)..")
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED On)
set(CMAKE_CXX_EXTENSIONS Off)

include(FetchContent)
FetchContent_Declare(googletest
					GIT_REPOSITORY https://github.com/google/googletest.git
					GIT_TAG	v1.13.0
)

FetchContent_MakeAvailable(googletest)
add_library(GTest::GTest INTERFACE IMPORTED)
target_link_libraries(GTest::GTest INTERFACE gtest_main)

add_executable(tests test.cpp)

target_link_libraries(tests PRIVATE GTest::GTest TimeThis)

add_test(TimeThis_gtests tests)