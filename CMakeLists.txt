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
endif()

if((DEFINED opt_timethis_install) AND (NOT DEFINED CI_BUILDID))
    message(ERROR "When building from CI the CI_BUILDID must be set")
endif()

#   ________________________________________________
# - Set the Name of the project and the details
# - The namespace is set to "siddiqsoft"
# - The options are all prefixed opt_siddiqsoft_XXX
# - The option opt_siddiqsoft_testing is set to OFF and used by our CI to build, test and package

# -[]-------------------------------------
# We need to support "C" language in order for the googletest to build.
project(timethis
        VERSION         "${CURRENT_PROJECT_VERSION}"
        LANGUAGES       CXX C
        HOMEPAGE_URL    "https://github.com/siddiqsoft/TimeThis"
        DESCRIPTION     "A header-only library to measure time elapsed for a given scope."
)

# Based on the guide https://ryandeangraham.medium.com/building-a-header-only-library-hello-world-f9dd05ac1066
# With addition info https://cmake.org/cmake/help/v3.18/guide/tutorial/#adding-a-version-number-and-configured-header-file
# https://www.foonathan.net/2022/06/cmake-fetchcontent/

include(GNUInstallDirs)

add_library(${PROJECT_NAME} INTERFACE "${PROJECT_SOURCE_DIR}/include")
add_library(${PROJECT_NAME}::${PROJECT_NAME} ALIAS ${PROJECT_NAME})

target_compile_features(${PROJECT_NAME} INTERFACE cxx_std_20)

target_include_directories( ${PROJECT_NAME}
                            INTERFACE
                            $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
                            $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)
message(STATUS ".target_include_directories( ${PROJECT_NAME}\n"
               "   .                         SYSTEM INTERFACE\n"
               "   .                         $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>\n"
               "   .                         $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)")

# opt_siddiqsoft_testing - Enabled via our CI build script
# If enabled, we will build the tests
if(opt_${PROJECT_NAME}_testing)
    add_subdirectory(tests)
endif()

if(opt_${PROJECT_NAME}_install)
    include(CMakePackageConfigHelpers)

    # for testing
    # cmake --preset Apple-Debug -Dopt_timethis_testing=ON -Dopt_timethis_install=ON -DCI_BUILDID=2.0.4
    # cmake -E chdir build/Apple-Debug cpack -C Debug -G ZIP
    #
    message(STATUS "CMAKE_CURRENT_SOURCE_DIR.....${CMAKE_CURRENT_SOURCE_DIR}")
    message(STATUS "CMAKE_INSTALL_INCLUDEDIR.....${CMAKE_INSTALL_INCLUDEDIR}")
    message(STATUS "CMAKE_INSTALL_LIBDIR.........${CMAKE_INSTALL_LIBDIR}")
    message(STATUS "CMAKE_CURRENT_BINARY_DIR.....${CMAKE_CURRENT_BINARY_DIR}")
    message(STATUS "CMAKE_CURRENT_LIST_DIR.......${CMAKE_CURRENT_LIST_DIR}")
    message(STATUS "CMAKE_INSTALL_PREFIX.........${CMAKE_INSTALL_PREFIX}")
    message(STATUS "--------------------------------------------------------------------------")
    # testing end
    ###
    write_basic_package_version_file(   "${PROJECT_NAME}ConfigVersion.cmake"
                                        VERSION         ${PROJECT_VERSION}
                                        COMPATIBILITY   SameMajorVersion)
    configure_package_config_file(  "${PROJECT_SOURCE_DIR}/pack/${PROJECT_NAME}Config.cmake.in"
                                    "${PROJECT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
                                    INSTALL_DESTINATION
                                    ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME})
    install(TARGETS     ${PROJECT_NAME}
            EXPORT      "${PROJECT_NAME}Targets"
            COMPONENT   "${PROJECT_NAME}"
            FILE_SET    HEADERS
            LIBRARY     DESTINATION ${CMAKE_INSTALL_LIBDIR}
            RUNTIME     DESTINATION ${CMAKE_CURRENT_BINARY_DIR}
            ARCHIVE     DESTINATION ${CMAKE_INSTALL_LIBDIR})
    install(EXPORT      "${PROJECT_NAME}Targets"
            COMPONENT   "${PROJECT_NAME}"
            FILE        "${PROJECT_NAME}Targets.cmake"
            NAMESPACE   ${PROJECT_NAME}::
            DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME})
    install(FILES       "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake"
                        "${PROJECT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
            COMPONENT   "${PROJECT_NAME}"
            DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME})
    install(DIRECTORY   "${PROJECT_SOURCE_DIR}/include/"
            COMPONENT   "${PROJECT_NAME}"
            DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
    include_directories("${PROJECT_SOURCE_DIR}/include/")
    # Usage
    # > mkdir build
    # > cd build
    # > cmake .. -DCMAKE_INSTALL_PREFIX:PATH=/your/installation/path
    # > cmake --build . --config Release --target install -- -j $(nproc)
    include(InstallRequiredSystemLibraries)
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
    set(CPACK_SOURCE_IGNORE_FILES           gmock/ gtest/ lib64 _deps/ tests ${CMAKE_BINARY_DIR}/ ${PROJECT_BINARY_DIR}/)
    #set(CPACK_PACKAGE_DIRECTORY             ${PROJECT_BINARY_DIR}/package)

    if(WIN32)
        set(CPACK_GENERATOR ZIP)
    elseif(Darwin)
        set(CPACK_GENERATOR ZIP)
    elseif(APPLE)
        set(CPACK_GENERATOR TGZ productbuild)
    elseif(CMAKE_SYSTEM_NAME STREQUAL "Linux")
        set(CPACK_GENERATOR TGZ RPM)
    else()
        set(CPACK_GENERATOR TGZ)
    endif()

    message(STATUS "Writing the package for `${CPACK_PACKAGE_NAME}` --> CPACK_PACKAGE_FILE_NAME: ${CPACK_PACKAGE_FILE_NAME}")
    include(CPack)
endif()
