#   SiddiqSoft CMakeLists for header only libraries
#   Copyright 2024 Abdulkareem Siddiq. All rights reserved.
#   See LICENSE file
#
#   When building from our CI server, the following options must be set
#   opt_timethis_testing ON
#   opt_timethis_install ON
#   CI_BUILDID must be set to the 
cmake_minimum_required(VERSION 3.25)

# -[]-------------------------------------
if(DEFINED opt_timethis_install AND CI_BUILDID)
    message(STATUS "Using the CI supplied buildid: CURRENT_PROJECT_VERSION --> ${CI_BUILDID}")
    set(CURRENT_PROJECT_VERSION ${CI_BUILDID})
else()
    message(STATUS "Using the dev-only version")
    set(CURRENT_PROJECT_VERSION "0.0.0.0")
endif()

#if((DEFINED opt_timethis_install) AND (NOT DEFINED CI_BUILDID))
#    message(ERROR "When building from CI the CI_BUILDID must be set")
#endif()

#   ________________________________________________
# - Set the Name of the project and the details
# - The namespace is set to "siddiqsoft"
# - The options are all prefixed opt_siddiqsoft_XXX
# - The option opt_siddiqsoft_testing is set to OFF and used by our CI to build, test and package

# -[]-------------------------------------
project(timethis
        VERSION         "${CURRENT_PROJECT_VERSION}"
        LANGUAGES       CXX
        HOMEPAGE_URL    "https://github.com/siddiqsoft/TimeThis"
        DESCRIPTION     "A header-only library to measure time elapsed for a given scope.")

# Based on the guide https://ryandeangraham.medium.com/building-a-header-only-library-hello-world-f9dd05ac1066
# With addition info https://cmake.org/cmake/help/v3.18/guide/tutorial/#adding-a-version-number-and-configured-header-file
# https://www.foonathan.net/2022/06/cmake-fetchcontent/

set(${PROJECT_NAME}_DEVMODE OFF)
if(CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)
    set(${PROJECT_NAME}_DEVMODE ON)
    message(STATUS "Project is in `librarys developer's home` directory. ${${PROJECT_NAME}_DEVMODE}-->ON")
endif()

#############
#  Options  #
#############

# We run the tests by default when we're in development mode for this library
option(${PROJECT_NAME}_BUILD_TESTS "Build tests. Uncheck for install only runs" ${${PROJECT_NAME}_DEVMODE})
option(${PROJECT_NAME}_BUILD_PACKAGE "Build tests. Uncheck for install only runs" ${${PROJECT_NAME}_DEVMODE})

include(CMakePackageConfigHelpers)
include(CMakeDependentOption)
include(GNUInstallDirs)
include(CTest)

#########################
#  Library Definition   #
#########################

add_library(${PROJECT_NAME} INTERFACE)
	
target_include_directories( ${PROJECT_NAME} INTERFACE
                            $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
                            $<INSTALL_INTERFACE:include>)

target_compile_features(${PROJECT_NAME} INTERFACE cxx_std_20)

install(TARGETS ${PROJECT_NAME}
        EXPORT ${PROJECT_NAME}-targets)
install(DIRECTORY include/
        DESTINATION include
        FILES_MATCHING PATTERN *.hpp)

add_library(${PROJECT_NAME}::${PROJECT_NAME} ALIAS ${PROJECT_NAME})

# Write out the config file template.
file(WRITE "${PROJECT_BINARY_DIR}/${PROJECT_NAME}-config.cmake.in" [[
@PACKAGE_INIT@
include("${CMAKE_CURRENT_LIST_DIR}/@PROJECT_NAME@-targets.cmake")
]])
message(STATUS "Write out file `${PROJECT_BINARY_DIR}/${PROJECT_NAME}-config.cmake.in`.")

configure_package_config_file(  "${PROJECT_SOURCE_DIR}/pack/${PROJECT_NAME}-config.cmake.in"
                                "${PROJECT_BINARY_DIR}/${PROJECT_NAME}-config.cmake"
                                INSTALL_DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}"
                                NO_SET_AND_CHECK_MACRO
                                NO_CHECK_REQUIRED_COMPONENTS_MACRO)

write_basic_package_version_file(   "${PROJECT_NAME}-config-version.cmake"
                                    VERSION ${PROJECT_VERSION}
                                    COMPATIBILITY SameMajorVersion)

install(EXPORT ${PROJECT_NAME}-targets 
        DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}"
        NAMESPACE ${PROJECT_NAME}::)
install(FILES
        "${PROJECT_BINARY_DIR}/${PROJECT_NAME}-config-version.cmake"
        "${PROJECT_BINARY_DIR}/${PROJECT_NAME}-config.cmake"
        DESTINATION
        ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME})

if(${PROJECT_NAME}_BUILD_TESTS)
  add_subdirectory(tests)
endif()

if (${PROJECT_NAME}_BUILD_PACKAGE)
    # Continue on to the install/package stage..
    set(CPACK_SOURCE_GENERATOR TBZ2 TGZ TXZ ZIP)
    set(CPACK_SOURCE_PACKAGE_FILE_NAME "${CPACK_PACKAGE_FILE_NAME}")

    set(PKG_CONFIG_FILE_NAME "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}.pc")
    configure_file("${CMAKE_CURRENT_SOURCE_DIR}/pack/pkgconfig.pc.in" "${PKG_CONFIG_FILE_NAME}" @ONLY)
    install(FILES "${PKG_CONFIG_FILE_NAME}"
            DESTINATION "${CMAKE_INSTALL_LIBDIR}/pkgconfig")

    # Packaging Configuration
    set(CPACK_PACKAGE_NAME                  "${PROJECT_NAME}")
    string(REGEX MATCH "v([0-9]+)\\.([0-9]+)\\.([0-9]+)" VERSION_MATCH "${PROJECT_VERSION}")
    set(CPACK_PACKAGE_VERSION_MAJOR         "${CMAKE_MATCH_1}")
    set(CPACK_PACKAGE_VERSION_MINOR         "${CMAKE_MATCH_2}")
    set(CPACK_PACKAGE_VERSION_MINOR         "${CMAKE_MATCH_3}")
    set(CPACK_PACKAGE_DESCRIPTION_SUMMARY   ${PROJECT_DESCRIPTION})
    set(CPACK_PACKAGE_VENDOR                "SiddiqSoft")
    set(CPACK_PACKAGE_FILE_NAME             "${PROJECT_NAME}-${PROJECT_VERSION}")
    set(CPACK_RESOURCE_FILE_LICENSE         "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE")
    set(CPACK_RESOURCE_FILE_README          "${CMAKE_CURRENT_SOURCE_DIR}/README.md")
    set(CPACK_SOURCE_IGNORE_FILES           gmock/ gtest/ build/ .DS_Store .gitignore _deps/ tests/ ${CMAKE_BINARY_DIR}/ ${PROJECT_BINARY_DIR}/)
    #set(CPACK_PACKAGE_DIRECTORY             ${PROJECT_BINARY_DIR}/package)

    if(WIN32)
        set(CPACK_GENERATOR ZIP)
    elseif(Darwin)
        set(CPACK_GENERATOR ZIP)
    elseif(APPLE)
        set(CPACK_GENERATOR TGZ productbuild)
    elseif(CMAKE_SYSTEM_NAME STREQUAL "Linux")
        set(CPACK_GENERATOR TGZ)
    else()
        set(CPACK_GENERATOR TGZ)
    endif()

    message(STATUS "Writing the package for `${CPACK_PACKAGE_NAME}` --> CPACK_PACKAGE_FILE_NAME: ${CPACK_PACKAGE_FILE_NAME}")
    include(CPack)
endif()