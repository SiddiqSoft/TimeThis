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
if(DEFINED CI_BUILDID)
    message(STATUS "Using the CI supplied buildid: CURRENT_PROJECT_VERSION --> ${CI_BUILDID}")

    # Check if we have a last fragment.
    # When building a non release we end up with fragmented/temporary gitversion
    # 1.2.3-branch-name-5.9+99
    # where the `99` at the end indicates the commit count on this branch.
    set(VERSION_LAST "")
    string(REGEX MATCH "[\+]\+([0-9]+)$" VERSION_END_MATCH ${CI_BUILDID})
    message(STATUS "Match count: ${CMAKE_MATCH_COUNT}")
    if(CMAKE_MATCH_COUNT EQUAL 1)
        set(VERSION_LAST         "${CMAKE_MATCH_1}")
    endif()

    # Match the first full version
    string(REGEX MATCH "([0-9]+)\\.([0-9]+)\\.([0-9]+)\\.*([0-9]+)*" VERSION_MATCH ${CI_BUILDID})
    message(STATUS "Match count: ${CMAKE_MATCH_COUNT}")
    if(CMAKE_MATCH_COUNT EQUAL 3)
        set(VERSION_MAJOR         "${CMAKE_MATCH_1}")
        set(VERSION_MINOR         "${CMAKE_MATCH_2}")
        set(VERSION_PATCH         "${CMAKE_MATCH_3}")
        set(CURRENT_PROJECT_VERSION ${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH})
    elseif(CMAKE_MATCH_COUNT EQUAL 4)
        set(VERSION_MAJOR         "${CMAKE_MATCH_1}")
        set(VERSION_MINOR         "${CMAKE_MATCH_2}")
        set(VERSION_PATCH         "${CMAKE_MATCH_3}")
        set(CURRENT_PROJECT_VERSION ${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}.${CMAKE_MATCH_4})
    endif()
    if(NOT (VERSION_LAST STREQUAL ""))
        string(APPEND CURRENT_PROJECT_VERSION ".${VERSION_LAST}")
    endif()
else()
    if((DEFINED ${PROJECT_NAME}_BUILD_PACKAGE) AND (NOT DEFINED CI_BUILDID))
        message(ERROR "When building from CI the CI_BUILDID must be set")
    endif()
    message(STATUS "Using the dev-only version")
    set(CURRENT_PROJECT_VERSION "0.0.0.0")
endif()

message(STATUS "Parsed out the CI_BUILDID --> CURRENT_PROJECT_VERSION:${CURRENT_PROJECT_VERSION}")

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
    message(STATUS "Project is in the `library developer's home` directory. ${${PROJECT_NAME}_DEVMODE}-->ON")
endif()

#############
#  Options  #
#############

# We run the tests by default when we're in development mode for this library
option(${PROJECT_NAME}_BUILD_TESTS "Build tests. Uncheck for install only runs" OFF)
option(${PROJECT_NAME}_BUILD_PACKAGE "Package. Uncheck for install only runs" OFF)

message(STATUS  "Configuration options: ${PROJECT_NAME}_BUILD_TESTS  -- > ${${PROJECT_NAME}_BUILD_TESTS}\n"
                "                          CI_BUILDID            -- > ${CI_BUILDID} --> ${CURRENT_PROJECT_VERSION}\n"
                "                          ${PROJECT_NAME}_BUILD_PACKAGE --> ${${PROJECT_NAME}_BUILD_PACKAGE}")

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

if(${${PROJECT_NAME}_BUILD_TESTS})
    message(STATUS "Asked to build tests.. ${PROJECT_NAME}_BUILD_TESTS = ${${PROJECT_NAME}_BUILD_TESTS}")
    add_subdirectory(tests)
endif()

if (${${PROJECT_NAME}_BUILD_PACKAGE})
    message(STATUS "Asked to build package: ${PROJECT_NAME}_BUILD_PACKAGE = ${${PROJECT_NAME}_BUILD_PACKAGE}")
    # Continue on to the install/package stage..
    set(CPACK_SOURCE_GENERATOR TBZ2 TGZ TXZ ZIP)
    set(CPACK_SOURCE_PACKAGE_FILE_NAME "${CPACK_PACKAGE_FILE_NAME}")

    set(PKG_CONFIG_FILE_NAME "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}.pc")
    configure_file("${CMAKE_CURRENT_SOURCE_DIR}/pack/pkgconfig.pc.in" "${PKG_CONFIG_FILE_NAME}" @ONLY)
    install(FILES "${PKG_CONFIG_FILE_NAME}"
            DESTINATION "${CMAKE_INSTALL_LIBDIR}/pkgconfig")

    # Packaging Configuration
    set(CPACK_PACKAGE_NAME                  "${PROJECT_NAME}")
    set(CPACK_PACKAGE_VERSION_MAJOR         "${VERSION_MAJOR}")
    set(CPACK_PACKAGE_VERSION_MINOR         "${VERSION_MINOR}")
    set(CPACK_PACKAGE_VERSION_PATCH         "${VERSION_PATCH}")
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