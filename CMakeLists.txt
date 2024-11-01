cmake_minimum_required(VERSION 3.25)

#   SiddiqSoft CMakeLists for header only libraries
#   Copyright 2024 Abdulkareem Siddiq. All rights reserved.
#   See LICENSE file

#   ________________________________________________
# - Set the Name of the project.
# - The namespace is set to "siddiqsoft"
# - The options are all prefixed opt_siddiqsoft_XXX
# - The option opt_siddiqsoft_btp is set to OFF and used by our CI to build, test and package

##########################################################################

#   We need to support "C" language in order for the googletest to build.
project(siddiqsoft_timethis
        VERSION 1.5.0
        LANGUAGES CXX C
        HOMEPAGE_URL "https://github.com/siddiqsoft/TimeThis"
        DESCRIPTION "A header-only library to measure time elapsed for a given scope."
)

# ----------------------------------------
# -- Options for our library -------------
# This is valid for our local builds when we're building, testing and packing.
option(opt_siddiqsoft_btp OFF)

# ----------------------------------------
# Include some common stuff
include(FetchContent)
include(GNUInstallDirs)

set(${PROJECT_NAME}_TARGET_NAME                ${PROJECT_NAME})
set(${PROJECT_NAME}_CONFIG_INSTALL_DIR         "${CMAKE_INSTALL_DATADIR}/cmake/${PROJECT_NAME}" CACHE INTERNAL "")
set(${PROJECT_NAME}_INCLUDE_INSTALL_DIR        "${CMAKE_INSTALL_INCLUDEDIR}")
set(${PROJECT_NAME}_TARGETS_EXPORT_NAME        "${PROJECT_NAME}Targets")
set(${PROJECT_NAME}_CMAKE_CONFIG_TEMPLATE      "cmake/config.cmake.in")
set(${PROJECT_NAME}_CMAKE_CONFIG_DIR           "${CMAKE_CURRENT_BINARY_DIR}")
set(${PROJECT_NAME}_CMAKE_VERSION_CONFIG_FILE  "${${PROJECT_NAME}_CMAKE_CONFIG_DIR}/${PROJECT_NAME}ConfigVersion.cmake")
set(${PROJECT_NAME}_CMAKE_PROJECT_CONFIG_FILE  "${${PROJECT_NAME}_CMAKE_CONFIG_DIR}/${PROJECT_NAME}Config.cmake")
set(${PROJECT_NAME}_CMAKE_PROJECT_TARGETS_FILE "${${PROJECT_NAME}_CMAKE_CONFIG_DIR}/${PROJECT_NAME}Targets.cmake")
set(${PROJECT_NAME}_PKGCONFIG_INSTALL_DIR      "${CMAKE_INSTALL_DATADIR}/pkgconfig")


# ----------------------------------------
# -- Declarations for our library --------
# https://github.com/friendlyanon/cmake-init-header-only/tree/master
#

# Name of the library (same as name of project)
set(${PROJECT_NAME}_TARGET_NAME ${PROJECT_NAME})

# Declare the library interface
add_library(${${PROJECT_NAME}_TARGET_NAME} INTERFACE)
add_library(siddiqsoft::${${PROJECT_NAME}_TARGET_NAME} ALIAS ${${PROJECT_NAME}_TARGET_NAME})

set_property(
        TARGET ${${PROJECT_NAME}_TARGET_NAME}
        PROPERTY EXPORT_NAME ${PROJECT_NAME}
        )

# Emit status ..
message(STATUS "....set(${PROJECT_NAME}_TARGET_NAME ${PROJECT_NAME})")
message(STATUS "....add_library(${${PROJECT_NAME}_TARGET_NAME} INTERFACE)")
message(STATUS "....add_library(siddiqsoft::${${PROJECT_NAME}_TARGET_NAME} ALIAS ${${PROJECT_NAME}_TARGET_NAME})")


# Compiler/SDK requirements..
target_compile_features(${${PROJECT_NAME}_TARGET_NAME} INTERFACE cxx_std_20)

target_include_directories(
    ${${PROJECT_NAME}_TARGET_NAME}
    INTERFACE
    "\$<BUILD_INTERFACE:${$PROJECT_SOURCE_DIR}/include>"
    "\$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>"
)


# ---------------------------------------------
# -- Testing rules for our library ------------
if (opt_siddiqsoft_btp)
        message(STATUS "....We're building, testing and packing")
        include(CTest)
        enable_testing()
        add_subdirectory(tests)
endif()


# ---------------------------------------------
# -- Installtion rules for our library --------
# https://github.com/friendlyanon/cmake-init-header-only/blob/master/cmake/install-rules.cmake
