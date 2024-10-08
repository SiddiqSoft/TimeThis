cmake_minimum_required(VERSION 3.28)
# Require C++20
message(STATUS "Require C++23 (include)..")
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED On)
set(CMAKE_CXX_EXTENSIONS Off)

add_library(TimeThis INTERFACE)
target_sources(TimeThis INTERFACE TimeThis.hpp)
target_include_directories(TimeThis INTERFACE "${PROJECT_SOURCE_DIR}/include")
target_compile_definitions(TimeThis INTERFACE cxx_std_23)
