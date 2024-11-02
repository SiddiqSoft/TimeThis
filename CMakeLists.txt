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

# ----------------------------------------
# Include some common stuff
include(FetchContent)
include(GNUInstallDirs)

# -[]-------------------------------------
# -- Options for our library -------------
# Controls whether we're built for testing and publication
option( opt_siddiqsoft_btp      "This option is for CI and internally used to build, test and package the library."
        OFF)
# This is valid for our local builds when we're building, testing and packing.
option( opt_siddiqsoft_install  "This indicates the library install location"
        ${PROJECT_IS_TOP_LEVEL})

if(opt_siddiqsoft_btp)
    FetchContent_Declare(googletest
            GIT_REPOSITORY https://github.com/google/googletest.git
            GIT_TAG	v1.15.2
            FIND_PACKAGE_ARGS NAMES GTest
    )
endif()

# -- 
add_library(${PROJECT_NAME} INTERFACE)
# This option is for use with subdirectory
add_library(${PROJECT_NAME}::${PROJECT_NAME} ALIAS ${PROJECT_NAME})

target_include_directories(
        ${PROJECT_NAME} INTERFACE   $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
                                    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

# -- 
# Make sure we're C++20 compliant
target_compile_features(${PROJECT_NAME} INTERFACE cxx_std_20)

# -- 
# Check and setup for tests
if(opt_siddiqsoft_btp)
    enable_testing()
    FetchContent_MakeAvailable(googletest)
    add_subdirectory(tests)
endif()

# -- 
# Install the library
if(opt_siddiqsoft_install)
    set_target_properties(${PROJECT_NAME} PROPERTIES PUBLIC_HEADER "${public_headers}")
    message(STATUS "set_target_properties(${PROJECT_NAME} PROPERTIES PUBLIC_HEADER \"${public_headers}\")")

    install(    TARGETS ${PROJECT_NAME}
                EXPORT  "${PROJECT_NAME}_Targets"
                ARCHIVE         DESTINATION ${CMAKE_INSTALL_LIBDIR}
                LIBRARY         DESTINATION ${CMAKE_INSTALL_LIBDIR}
                RUNTIME         DESTINATION ${CMAKE_INSTALL_BINDIR}
                PUBLIC_HEADER   DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/siddiqsoft
                INCLUDES        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
    )

    message(STATUS  ".    install(    TARGETS ${PROJECT_NAME}\n"
                    "   .                EXPORT  ${PROJECT_NAME}_Targets\n"
                    "   .                ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}\n"
                    "   .                LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}\n"
                    "   .                RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}\n"
                    "   .                PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/siddiqsoft)\n"
    )

    include(CMakePackageConfigHelpers)
    write_basic_package_version_file(   "${PROJECT_NAME}Version.cmake"
                                        VERSION ${PROJECT_VERSION}
                                        COMPATIBILITY SameMajorVersion
    )

    message(STATUS  ".    write_basic_package_version_file(   \"${PROJECT_NAME}Version.cmake\"\n"
                    "   .                                        VERSION ${PROJECT_VERSION}\n"
                    "   .                                        COMPATIBILITY SameMajorVersion)\n"
    )

    configure_package_config_file(
        "${PROJECT_SOURCE_DIR}/cmake/${PROJECT_NAME}Config.cmake.in"
        "${PROJECT_BINARY_DIR}/cmake/${PROJECT_NAME}Config.cmake"
        #INSTALL_DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/${PROJECT_NAME}/cmake
        INSTALL_DESTINATION cmake
    )

    message(STATUS ".    configure_package_config_file(\n"
                   "   .         \"${PROJECT_SOURCE_DIR}/cmake/${PROJECT_NAME}Config.cmake.in\n"
                   "   .         \"${PROJECT_BINARY_DIR}/cmake/${PROJECT_NAME}Config.cmake\n"
                   "   .         INSTALL_DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/${PROJECT_NAME}/cmake\n"
                   "   .         INSTALL_DESTINATION cmake)\n"
    )

    install(    EXPORT  ${PROJECT_NAME}_Targets
                FILE    ${PROJECT_NAME}Targets.cmake
                NAMESPACE   siddiqsoft::
                DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/${PROJECT_NAME}/cmake
    )

    message(STATUS  ".    install(    EXPORT  ${PROJECT_NAME}_Targets\n"
                    "   .                FILE    ${PROJECT_NAME}Targets.cmake\n"
                    "   .                NAMESPACE   siddiqsoft::\n"
                    "   .                DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/${PROJECT_NAME}/cmake)\n"
    )

    # Install the config files 
    install(FILES
                "${PROJECT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
                "${PROJECT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake"
            DESTINATION
                ${CMAKE_INSTALL_DATAROOTDIR}/${PROJECT_NAME}/cmake
    )
    message(STATUS ".     install(FILES\n"
                   "   .                 \"${PROJECT_BINARY_DIR}/${PROJECT_NAME}Config.cmake\"\n"
                   "   .                 \"${PROJECT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake\"\n"
                   "   .             DESTINATION\n"
                   "   .                 ${CMAKE_INSTALL_DATAROOTDIR}/${PROJECT_NAME}/cmake)\n"
    )

    # All of our libraries will be in the siddiqsoft folder
    install(DIRECTORY
                ${PROJECT_SOURCE_DIR}/include/siddiqsoft
            DESTINATION
                ${CMAKE_INSTALL_INCLUDEDIR}
    )

    message(STATUS ".    install(DIRECTORY   ${PROJECT_SOURCE_DIR}/include/siddiqsoft\n"
                   "   .             DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})\n"
    )
endif()

set(CPACK_RESOURCE_FILE_LICENSE "${PROJECT_SOURCE_DIR}/LICENSE")
set(CPACK_PACKAGE_DIRECTORY     ${PROJECT_BINARY_DIR}/packages)
set(CPACK_SOURCE_IGNORE_FILES   .git/
                                .github/
                                .vscode/
                                .mypy_cache/
                                _CPack_Packages/
                                ${CMAKE_BINARY_DIR}/
                                ${PROJECT_BINARY_DIR}/
)
set(CPACK_PACKAGE_DESCRIPTION       ${PROJECT_DESCRIPTION})
set(CPACK_PACKAGE_VERSION           ${PROJECT_VERSION})
#set(CPACK_PACKAGE_VERSION_MAJOR    "${VERSION_MAJOR}")
#set(CPACK_PACKAGE_VERSION_MINOR    "${VERSION_MINOR}")
#set(CPACK_PACKAGE_VERSION_PATCH    "${VERSION_PATCH}")
set(CPACK_PACKAGE_VENDOR            "SiddiqSoft")
set(CPACK_PACKAGE_CONTACT           "github@siddiqsoft.org")
set(CPACK_PACKAGE_FILE_NAME         "${CMAKE_PROJECT_NAME}-${PROJECT_VERSION}")

include(CPack)
