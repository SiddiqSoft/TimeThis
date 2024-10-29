cmake_minimum_required(VERSION 3.25)

file(GLOB TimeThis
     "*/*.hpp"
     "*.hpp"
)

add_library(TimeThis INTERFACE)

message(STATUS "CMake install directory: " ${CMAKE_INSTALL_INCLUDEDIR})

target_include_directories(TimeThis
                           INTERFACE
                           $<BUILD_INTERFACE:${PROJECT_INCLUDE_DIR}>
                           $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)