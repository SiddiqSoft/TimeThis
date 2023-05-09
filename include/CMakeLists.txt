# Require C++23
message(STATUS "Require C++23..")
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_library(TimeThis INTERFACE TimeThis.hpp)
#target_include_directories(TimeThis INTERFACE .)