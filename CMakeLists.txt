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


# opt_siddiqsoft_btp - Enabled via our CI build script
# If enabled, we will build the tests
if(opt_siddiqsoft_btp)
    add_subdirectory(tests)
endif()


# Installation Stuff
install(TARGETS ${PROJECT_NAME}
        EXPORT ${PROJECT_NAME}_Targets
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})
include(CMakePackageConfigHelpers)
write_basic_package_version_file("${PROJECT_NAME}ConfigVersion.cmake"
                                 VERSION ${PROJECT_VERSION}
                                 COMPATIBILITY SameMajorVersion)
if(NOT INCLUDE_INSTALL_DIR)
  set(INCLUDE_INSTALL_DIR include/hello)
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

install(DIRECTORY ${PROJECT_SOURCE_DIR}/include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME})
