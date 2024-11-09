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
    if((DEFINED timethis_BUILD_PACKAGE) AND (NOT DEFINED CI_BUILDID))
        message(ERROR "When building from CI the CI_BUILDID must be set")
    endif()
    message(STATUS "Using the dev-only version")
    set(CURRENT_PROJECT_VERSION "0.0.0.0")
endif()

message(STATUS "Parsed out the CI_BUILDID --> CURRENT_PROJECT_VERSION:${CURRENT_PROJECT_VERSION}")

#   ________________________________________________
# - Set the Name of the project and the details
# - Change the name to your name: timethis
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

set(timethis_DEVMODE OFF)
if(CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)
    set(timethis_DEVMODE ON)
    message(STATUS "Project is in the `library developer's home` directory. ${timethis_DEVMODE}-->ON")
endif()

# Global configuration

# Only set the cxx_standard if it is not set by someone else
if (NOT DEFINED CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 20)
endif()

# strongly encouraged to enable this globally to avoid conflicts between
# -Wpedantic being enabled and -std=c++20 and -std=gnu++20 for example
# when compiling with PCH enabled
set(CMAKE_CXX_EXTENSIONS OFF)


#############
#  Options  #
#############

# We run the tests by default when we're in development mode for this library
option(timethis_BUILD_TESTS "Build tests. Uncheck for install only runs" OFF)
option(timethis_BUILD_PACKAGE "Package. Uncheck for install only runs" OFF)

message(STATUS  "Configuration options: timethis_BUILD_TESTS  -- > ${timethis_BUILD_TESTS}\n"
                "                          CI_BUILDID            -- > ${CI_BUILDID} --> ${CURRENT_PROJECT_VERSION}\n"
                "                          timethis_BUILD_PACKAGE --> ${timethis_BUILD_PACKAGE}")

include(CMakePackageConfigHelpers)
include(CMakeDependentOption)
include(GNUInstallDirs)
include(CTest)

#########################
#  Library Definition   #
#########################
add_library(timethis INTERFACE)
add_library(timethis::timethis ALIAS timethis)

#target_include_directories( timethis INTERFACE
#                            $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
#                            $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)
target_include_directories( timethis INTERFACE
                            $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
                            $<INSTALL_INTERFACE:${INCLUDE_INSTALL_DIR}>)
target_compile_features(timethis INTERFACE cxx_std_20)

install(TARGETS timethis
        EXPORT timethis-targets)
install(TARGETS timethis
        EXPORT timethis-config.cmake)
install(DIRECTORY
        ${CMAKE_CURRENT_LIST_DIR}/include
        DESTINATION
        ${CMAKE_INSTALL_INCLUDEDIR})
include_directories(AFTER ${CMAKE_INSTALL_INCLUDEDIR} ${CMAKE_CURRENT_LIST_DIR}/include)

install(TARGETS timethis
        EXPORT  ${PROJECT_NAME}
        LIBRARY         DESTINATION "${CMAKE_INSTALL_LIBDIR}" COMPONENT shlib
        ARCHIVE         DESTINATION "${CMAKE_INSTALL_LIBDIR}" COMPONENT lib
        RUNTIME         DESTINATION "${CMAKE_INSTALL_LIBDIR}" COMPONENT bin
        PUBLIC_HEADER   DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}" COMPONENT dev)

configure_package_config_file(  "${PROJECT_SOURCE_DIR}/pack/${PROJECT_NAME}-config.cmake.in"
                                "${PROJECT_BINARY_DIR}/${PROJECT_NAME}-config.cmake"

                                INSTALL_DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}")
install(FILES       "${PROJECT_BINARY_DIR}/${PROJECT_NAME}-config.cmake"
        DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}")
write_basic_package_version_file(   ${PROJECT_NAME}-config-version.cmake
                                    VERSION ${PROJECT_VERSION}
                                    COMPATIBILITY SameMajorVersion)
install(FILES "${PROJECT_BINARY_DIR}/${PROJECT_NAME}-config-version.cmake"
        DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}")

install(DIRECTORY include
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
        FILES_MATCHING
        PATTERN "*.hpp")
export( TARGETS  ${PROJECT_NAME}
        NAMESPACE ${PROJECT_NAME}::
        FILE timethis-targets.cmake)
export( PACKAGE ${PROJECT_NAME})                            
install(EXPORT timethis-targets 
        DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/timethis"
        NAMESPACE timethis::)
install(FILES
        "${PROJECT_BINARY_DIR}/timethis-config-version.cmake"
        "${PROJECT_BINARY_DIR}/timethis-config.cmake"
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/timethis)

if(timethis_BUILD_TESTS)
    message(STATUS "Asked to build tests.. timethis_BUILD_TESTS = ${timethis_BUILD_TESTS}")
    add_subdirectory(tests)
endif()

message(STATUS "The value of CMAKE_SKIP_INSTALL_RULES == ${CMAKE_SKIP_INSTALL_RULES}")

if (timethis_BUILD_PACKAGE)
    message(STATUS "Asked to build package: timethis_BUILD_PACKAGE = ${timethis_BUILD_PACKAGE}")
    # Continue on to the install/package stage..
    set(CPACK_SOURCE_GENERATOR TBZ2 TGZ TXZ ZIP)

    set(PKG_CONFIG_FILE_NAME "${CMAKE_CURRENT_BINARY_DIR}/timethis.pc")
    configure_file("${CMAKE_CURRENT_SOURCE_DIR}/pack/pkgconfig.pc.in" "${PKG_CONFIG_FILE_NAME}" @ONLY)
    install(FILES       "${PKG_CONFIG_FILE_NAME}"
            DESTINATION "${CMAKE_INSTALL_LIBDIR}/pkgconfig")

    # Packaging Configuration
    set(CPACK_PACKAGE_NAME                  "${PROJECT_NAME}")
    set(CPACK_PACKAGE_VERSION_MAJOR         "${VERSION_MAJOR}")
    set(CPACK_PACKAGE_VERSION_MINOR         "${VERSION_MINOR}")
    set(CPACK_PACKAGE_VERSION_PATCH         "${VERSION_PATCH}")
    set(CPACK_PACKAGE_DESCRIPTION_SUMMARY   ${PROJECT_DESCRIPTION})
    set(CPACK_PACKAGE_VENDOR                "SiddiqSoft")
    set(CPACK_RESOURCE_FILE_LICENSE         "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE")
    set(CPACK_RESOURCE_FILE_README          "${CMAKE_CURRENT_SOURCE_DIR}/README.md")
    set(CPACK_SOURCE_IGNORE_FILES           gmock/ gtest/ build/ .DS_Store .gitignore _deps/ tests/ ${CMAKE_BINARY_DIR}/ ${PROJECT_BINARY_DIR}/)
    #set(CPACK_PACKAGE_DIRECTORY             ${PROJECT_BINARY_DIR}/package)

    # Experience shows that explicit package naming can help make it easier to sort
    # out potential ABI related issues before they start, while helping you
    # track a build to a specific GIT SHA
    set(CPACK_PACKAGE_FILE_NAME             "${CMAKE_PROJECT_NAME}-${CMAKE_PROJECT_VERSION}-${GIT_SHORT_SHA}-${CMAKE_SYSTEM_NAME}-${CMAKE_BUILD_TYPE}-${CMAKE_CXX_COMPILER_ID}-${CMAKE_CXX_COMPILER_VERSION}")
    set(CPACK_SOURCE_PACKAGE_FILE_NAME      ${CPACK_PACKAGE_FILE_NAME})

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

    set(package_files include/ CMakeLists.txt LICENSE)
    add_custom_command(OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/${CPACK_PACKAGE_FILE_NAME}-src.zip
        COMMAND ${CMAKE_COMMAND} -E tar c ${CMAKE_CURRENT_BINARY_DIR}/${CPACK_PACKAGE_FILE_NAME}-src.zip --format=zip -- ${package_files}
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        DEPENDS ${package_files})
    add_custom_target(${PROJECT_NAME}_package DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/${CPACK_PACKAGE_FILE_NAME}-src.zip)    
endif()
