cmake_minimum_required(VERSION 3.25.0)


find_package(Git REQUIRED)

execute_process(COMMAND ${GIT_EXECUTABLE} describe --tags --always OUTPUT_VARIABLE GIT_COMMIT_VERSION OUTPUT_STRIP_TRAILING_WHITESPACE)
message(STATUS "Found: ${GIT_COMMIT_VERSION}..")
string(REGEX MATCH "^[0-9]\.[0-9]\.[0-9]" GIT_VERSION ${GIT_COMMIT_VERSION})
message(STATUS "Found: ${GIT_COMMIT_VERSION} -> ${GIT_VERSION}")

project(TimeThis DESCRIPTION "Simple stopwatch implementation with optional callback on destructor"
				 HOMEPAGE_URL "https://github.com/SiddiqSoft/TimeThis"
				 LANGUAGES	CXX
				 VERSION		${GIT_VERSION})

# Require C++20
message(STATUS "Require C++20 (host)..")
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED On)
set(CMAKE_CXX_EXTENSIONS Off)

include(GNUInstallDirs)
add_subdirectory(include)

enable_testing()
add_subdirectory(tests)
