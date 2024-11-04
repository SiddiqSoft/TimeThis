#   SiddiqSoft CMakeLists for header only libraries
#   Copyright 2024 Abdulkareem Siddiq. All rights reserved.
#   See LICENSE file

cmake_minimum_required(VERSION 3.25)

# -[]-------------------------------------
# Gather the git version
if( DEFINED CI_BUILDID)
    message(STATUS "Using the CI supplied buildid: CURRENT_PROJECT_VERSION --> ${CI_BUILDID}")
    set(CURRENT_PROJECT_VERSION ${CI_BUILDID})
else()
    execute_process(
        COMMAND git describe --tags --always 
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        OUTPUT_VARIABLE GIT_DESCRIBE_OUTPUT
        OUTPUT_STRIP_TRAILING_WHITESPACE
    )
    message(STATUS "Git version of this code: ${GIT_DESCRIBE_OUTPUT}")
    if(GIT_DESCRIBE_OUTPUT)
        string(REGEX MATCH "([0-9]+)\\.([0-9]+)\\.([0-9]+)" PV_MMP "${GIT_DESCRIBE_OUTPUT}")
        set(PV_MAJOR ${CMAKE_MATCH_1})
        set(PV_MINOR ${CMAKE_MATCH_2})
        set(PV_PATCH ${CMAKE_MATCH_3})
        set(CURRENT_PROJECT_VERSION "${PV_MAJOR}.${PV_MINOR}.${PV_PATCH}")
        message(STATUS "Git version of this code derived: ${CURRENT_PROJECT_VERSION}")
    else()
        set(CURRENT_PROJECT_VERSION "0.0.0")
        message(WARNING "Unable to set the project; using ${CURRENT_PROJECT_VERSION}")
    endif()
endif()

#   ________________________________________________
# - Set the Name of the project and the details
# - The namespace is set to "siddiqsoft"
# - The options are all prefixed opt_siddiqsoft_XXX
# - The option opt_siddiqsoft_testing is set to OFF and used by our CI to build, test and package

# -[]-------------------------------------
# We need to support "C" language in order for the googletest to build.
project(timethis
        VERSION "${CURRENT_PROJECT_VERSION}"
        LANGUAGES CXX C
        HOMEPAGE_URL "https://github.com/siddiqsoft/TimeThis"
        DESCRIPTION "A header-only library to measure time elapsed for a given scope."
)

# Based on the guide https://ryandeangraham.medium.com/building-a-header-only-library-hello-world-f9dd05ac1066
# With addition info https://cmake.org/cmake/help/v3.18/guide/tutorial/#adding-a-version-number-and-configured-header-file

include(GNUInstallDirs)
find_package(Git REQUIRED)

add_library(${PROJECT_NAME} INTERFACE)

target_compile_features(${PROJECT_NAME} INTERFACE cxx_std_20)

target_include_directories( ${PROJECT_NAME}
                            INTERFACE
                            $<BUILD_INTERFACE:${${PROJECT_NAME}_SOURCE_DIR}/include>
                            $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)


# opt_siddiqsoft_testing - Enabled via our CI build script
# If enabled, we will build the tests
if(opt_siddiqsoft_testing)
    add_subdirectory(tests)
endif()


# Installation Stuff
# For header-only libraries, we require
#       PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
install(TARGETS ${PROJECT_NAME}
        EXPORT ${PROJECT_NAME}_Targets
        COMPONENT ${PROJECT_NAME}
        PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
        INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
message(STATUS  ". install(TARGETS ${PROJECT_NAME}\n"
                "   .         EXPORT ${PROJECT_NAME}_Targets\n"
                "   .         COMPONENT ${PROJECT_NAME}\n"
                "   .         PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}\n"
                "   .         INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})\n")
include(CMakePackageConfigHelpers)
write_basic_package_version_file("${PROJECT_NAME}ConfigVersion.cmake"
                                 VERSION ${PROJECT_VERSION}
                                 COMPATIBILITY SameMajorVersion)
if(NOT INCLUDE_INSTALL_DIR)
  set(INCLUDE_INSTALL_DIR include)
endif()
configure_package_config_file(  "${PROJECT_SOURCE_DIR}/cmake/${PROJECT_NAME}Config.cmake.in"
                                "${PROJECT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
                                INSTALL_DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/${PROJECT_NAME}/cmake
                                PATH_VARS INCLUDE_INSTALL_DIR)
install(EXPORT ${PROJECT_NAME}_Targets
        FILE ${PROJECT_NAME}Targets.cmake
        NAMESPACE ${PROJECT_NAME}::
        DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/${PROJECT_NAME}/cmake)

install(FILES "${PROJECT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
              "${PROJECT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake"
        DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/${PROJECT_NAME}/cmake)

install(DIRECTORY ${PROJECT_SOURCE_DIR}/include DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME})

# Usage
# > mkdir build
# > cd build
# > cmake .. -DCMAKE_INSTALL_PREFIX:PATH=/your/installation/path
# > cmake --build . --config Release --target install -- -j $(nproc)
include(InstallRequiredSystemLibraries)
# Packaging Configuration
set(CPACK_PACKAGE_NAME ${PROJECT_NAME})
set(CPACK_PACKAGE_VERSION ${PROJECT_VERSION})
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY ${PROJECT_DESCRIPTION})
set(CPACK_PACKAGE_VENDOR "SiddiqSoft")
set(CPACK_PACKAGE_FILE_NAME "${PROJECT_NAME}-${PROJECT_VERSION}")
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE")
set(CPACK_RESOURCE_FILE_README "${CMAKE_CURRENT_SOURCE_DIR}/README.md")
set(CPACK_SOURCE_IGNORE_FILES "/gmock/.*;/gtest/.*;/lib64/.*;/_deps/*;/tests/*")

message(STATUS "Writing the package for `${CPACK_PACKAGE_NAME}` --> CPACK_PACKAGE_FILE_NAME: ${CPACK_PACKAGE_FILE_NAME}")
include(CPack)
