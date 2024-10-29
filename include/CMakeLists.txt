cmake_minimum_required(VERSION 3.25)

file(GLOB ISO3166UtilLib
     "*/*.hpp"
     "*.hpp"
)

add_library(ISO3166UtilLib INTERFACE)

message(STATUS "CMake install directory: " ${CMAKE_INSTALL_INCLUDEDIR})

target_include_directories(ISO3166UtilLib
                           INTERFACE
                           $<BUILD_INTERFACE:${PROJECT_INCLUDE_DIR}>
                           $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)