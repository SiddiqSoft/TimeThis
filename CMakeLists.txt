cmake_minimum_required(VERSION 3.25)

# The project version is updated below with the tag from git
#   We need to support "C" language in order for the googletest to build.
project(TimeThis
        VERSION 0.0.0
        LANGUAGES CXX C
        HOMEPAGE_URL "%%myurl%%"
        DESCRIPTION "A header-only library to measure time elapsed for a given scope."
)

if (WIN32)
    message(STATUS "Select the generators for Windows..")
    # Set the C and C++ compilers to Clang
    # set(CMAKE_C_COMPILER clang)
    # set(CMAKE_CXX_COMPILER clang++)
    set(CMAKE_GENERATOR "Visual Studio 17 2022")
    set(CMAKE_GENERATOR_PLATFORM x64)
elseif (UNIX)
    message(STATUS "Select the generators for UNIX/Linux..")
    # Set the C and C++ compilers to Clang
    set(CMAKE_C_COMPILER clang)
    set(CMAKE_CXX_COMPILER clang++)
endif()

message(STATUS "Require C++20")
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)


message(STATUS "-- Importing git, gnuinstalldirs..")
find_package(Git REQUIRED)
include(FetchContent)
include(GNUInstallDirs)

message(STATUS "-- Going for the src..")
add_subdirectory("src")

message(STATUS "-- The project source directory............: ${PROJECT_SOURCE_DIR}")
message(STATUS "-- The PROJECT_BINARY_DIR directory........: ${PROJECT_BINARY_DIR}")
message(STATUS "-- The CMAKE_CURRENT_BINARY_DIR directory..: ${CMAKE_CURRENT_BINARY_DIR}")
message(STATUS "-- The CMAKE_BINARY_DIR directory..........: ${CMAKE_BINARY_DIR}")
message(STATUS "-- The CMAKE_RUNTIME_OUTPUT_DIRECTORY......: ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")
message(STATUS "-- The tests..")
include(CTest)
enable_testing()
add_subdirectory("tests")

