cmake_minimum_required(VERSION 3.28)

# Require C++23
message(STATUS "Require C++23 (tests)..")
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED On)
set(CMAKE_CXX_EXTENSIONS Off)
#set(CMAKE_CXX_FLAGS "-fconcepts-diagnostics-depth=2" )

include(FetchContent)
FetchContent_Declare(googletest
					GIT_REPOSITORY https://github.com/google/googletest.git
					GIT_TAG	v1.13.0
)
include(GoogleTest)
FetchContent_MakeAvailable(googletest)
add_library(GTest::GTest INTERFACE IMPORTED)
target_link_libraries(GTest::GTest INTERFACE gtest_main)

add_executable(tests test.cpp)

target_link_libraries(tests PRIVATE GTest::GTest TimeThis)

add_test(TimeThis_gtests tests)
gtest_discover_tests(tests)
