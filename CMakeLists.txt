cmake_minimum_required(VERSION 3.25.0)
project(TimeThis LANGUAGES	CXX
				 VERSION	1.2.0)

# Require C++23
message(STATUS "Require C++23..")
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include(GNUInstallDirs)
add_subdirectory(include)

enable_testing()
add_subdirectory(tests)
