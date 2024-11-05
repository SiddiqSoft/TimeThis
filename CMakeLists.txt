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

include(GNUInstallDirs)

add_library(${PROJECT_NAME} INTERFACE)

target_compile_features(${PROJECT_NAME} INTERFACE cxx_std_20)

target_include_directories( ${PROJECT_NAME}
                            INTERFACE
                            $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
                            $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)
message(STATUS  ".target_include_directories( ${PROJECT_NAME}\n"
                "   .                            INTERFACE\n"
                "   .                            $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>\n"
                "   .                            $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)")

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

    if(NOT INCLUDE_INSTALL_DIR)
        set(INCLUDE_INSTALL_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)
    endif()

    target_sources( ${PROJECT_NAME}
                    INTERFACE
                    FILE_SET    HEADERS
                    BASE_DIRS   ${CMAKE_CURRENT_SOURCE_DIR}/include
                    DIRECTORIES include)

    install(TARGETS     ${PROJECT_NAME}
            EXPORT      ${PROJECT_NAME}Targets
            FILE_SET    HEADERS
            DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME})
    
    #install(EXPORT      ${PROJECT_NAME}Targets
    #        FILE        ${PROJECT_NAME}Config.cmake
    #        NAMESPACE   siddiqsoft::
    #        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME})
    
    install(EXPORT ${PROJECT_NAME}Targets
            FILE ${PROJECT_NAME}Targets.cmake
            NAMESPACE ${PROJECT_NAME}::
            DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/${PROJECT_NAME}/cmake)

    install(FILES       ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake
                        ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}TargetsVersion.cmake
            DESTINATION ${CMAKE_INSTALL_LIBDIR}/${PROJECT_NAME}/cmake)
    message(STATUS  "install(FILES       ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake\n"
                    "  .                    ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}TargetsVersion.cmake\n"
                    "  .        DESTINATION ${CMAKE_INSTALL_LIBDIR}/${PROJECT_NAME}/cmake)\n")
    
    #export( EXPORT      ${PROJECT_NAME}Targets
    #        FILE        ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake
    #        NAMESPACE   siddiqsoft::)
    
    install(DIRECTORY ${PROJECT_SOURCE_DIR}/include DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME})

    configure_package_config_file(  "${CMAKE_CURRENT_SOURCE_DIR}/pack/${PROJECT_NAME}Config.cmake.in"
                                    "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
                                    INSTALL_DESTINATION
                                    ${CMAKE_INSTALL_DATAROOTDIR}/${PROJECT_NAME}/cmake
                                    PATH_VARS INCLUDE_INSTALL_DIR)
    message(STATUS "{CMAKE_CURRENT_SOURCE_DIR}/pack/{PROJECT_NAME}Config.cmake.in --> ${CMAKE_CURRENT_SOURCE_DIR}/${PROJECT_NAME}Config.cmake.in")
    message(STATUS "{CMAKE_CURRENT_BINARY_DIR}/{PROJECT_NAME}Config.cmake          --> ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake")
    
    write_basic_package_version_file(   ${PROJECT_BINARY_DIR}/${PROJECT_NAME}Version.cmake
                                        VERSION         ${PROJECT_VERSION}
                                        COMPATIBILITY   SameMajorVersion)
    message(STATUS "{PROJECT_BINARY_DIR}/{PROJECT_NAME}Version.cmake               --> ${PROJECT_BINARY_DIR}/${PROJECT_NAME}Version.cmake")

    # Usage
    # > mkdir build
    # > cd build
    # > cmake .. -DCMAKE_INSTALL_PREFIX:PATH=/your/installation/path
    # > cmake --build . --config Release --target install -- -j $(nproc)
    include(InstallRequiredSystemLibraries)
    # Packaging Configuration
    set(CPACK_PACKAGE_NAME                  ${PROJECT_NAME})
    set(CPACK_PACKAGE_VERSION               ${PROJECT_VERSION})
    set(CPACK_PACKAGE_DESCRIPTION_SUMMARY   ${PROJECT_DESCRIPTION})
    set(CPACK_PACKAGE_VENDOR                "SiddiqSoft")
    set(CPACK_PACKAGE_FILE_NAME             "${PROJECT_NAME}-${PROJECT_VERSION}")
    set(CPACK_RESOURCE_FILE_LICENSE         "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE")
    set(CPACK_RESOURCE_FILE_README          "${CMAKE_CURRENT_SOURCE_DIR}/README.md")
    set(CPACK_SOURCE_IGNORE_FILES           "/gmock/.*;/gtest/.*;/lib64/.*;/_deps/*;/tests/*")

    message(STATUS "Writing the package for `${CPACK_PACKAGE_NAME}` --> CPACK_PACKAGE_FILE_NAME: ${CPACK_PACKAGE_FILE_NAME}")
    include(CPack)
endif()