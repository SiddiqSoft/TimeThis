cmake_minimum_required(VERSION 3.28.0)
project(TimeThis DESCRIPTION "Simple stopwatch implementation with optional callback on destructor"
				 HOMEPAGE_URL "https://github.com/SiddiqSoft/TimeThis"
				 LANGUAGES	CXX
				 VERSION		1.2.1)

include(GNUInstallDirs)
find_package(Git REQUIRED)
include(FetchContent)


# Require C++20
message(STATUS "Require C++23 (host)..")
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED On)
set(CMAKE_CXX_EXTENSIONS Off)

add_subdirectory(include)

enable_testing()
add_subdirectory(tests)
