cmake_minimum_required(VERSION 3.25.0)
project(TimeThis DESCRIPTION "Simple stopwatch implementation with optional callback on destructor"
				 HOMEPAGE_URL "https://github.com/SiddiqSoft/TimeThis"
				 LANGUAGES	CXX
				 VERSION		1.2.1)

include(GNUInstallDirs)
find_package(Git REQUIRED)
include(FetchContent)


# Require C++20
message(STATUS "Require C++20 (host)..")
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED On)
set(CMAKE_CXX_EXTENSIONS Off)
#set(CMAKE_CXX_FLAGS "-fconcepts-diagnostics-depth=2" )

add_subdirectory(include)

enable_testing()
add_subdirectory(tests)
