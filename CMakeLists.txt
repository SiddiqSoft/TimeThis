cmake_minimum_required(VERSION 3.25)

#   SiddiqSoft CMakeLists for header only libraries
#   Copyright 2024 Abdulkareem Siddiq. All rights reserved.
#   See LICENSE file

#   ________________________________________________
# - Set the Name of the project and the details
# - The namespace is set to "siddiqsoft"
# - The options are all prefixed opt_siddiqsoft_XXX
# - The option opt_siddiqsoft_btp is set to OFF and used by our CI to build, test and package

# -[]-------------------------------------
# We need to support "C" language in order for the googletest to build.
project(timethis
        VERSION 1.5.0
        LANGUAGES CXX C
        HOMEPAGE_URL "https://github.com/siddiqsoft/TimeThis"
        DESCRIPTION "A header-only library to measure time elapsed for a given scope."
)

# Based on the guide https://ryandeangraham.medium.com/building-a-header-only-library-hello-world-f9dd05ac1066

include(GNUInstallDirs)

add_library(${PROJECT_NAME} INTERFACE)

target_compile_features(${PROJECT_NAME} INTERFACE cxx_std_20)

target_include_directories( ${PROJECT_NAME}
                            INTERFACE
                            $<BUILD_INTERFACE:${${PROJECT_NAME}_SOURCE_DIR}>
                            $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

if(opt_siddiqsoft_btp)
    include(tests)
endif()
