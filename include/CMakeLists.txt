# Require C++20
message(STATUS "Require C++20 (include)..")
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED On)
set(CMAKE_CXX_EXTENSIONS Off)

add_library(TimeThis INTERFACE)
target_sources(TimeThis INTERFACE TimeThis.hpp)
target_include_directories(TimeThis INTERFACE "${PROJECT_SOURCE_DIR}/include")
target_compile_definitions(TimeThis INTERFACE cxx_std_20)
