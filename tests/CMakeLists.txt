cmake_minimum_required(VERSION 3.25)

if(opt_siddiqsoft_testing)
    message(STATUS "....We're building ${PROJECT} for distribution; testing enabled.")
    include(FetchContent)
    include(GNUInstallDirs)

    set(TESTPROJ ${PROJECT_NAME}_tests)
    message(STATUS "....Setting the TESTPROJ --> ${PROJECT_NAME}_tests --> ${TESTPROJ}")

    message(STATUS "....Fetch googletest..")
    FetchContent_Declare(googletest URL https://github.com/google/googletest/releases/download/v1.15.2/googletest-1.15.2.tar.gz
                                    FIND_PACKAGE_ARGS NAMES GTest)
    message(STATUS "....Fetch googletest..Completed.")

    # Typically only one executable
    add_executable(${TESTPROJ})
    message(STATUS "....Adding sources for testing from: `${PROJECT_SOURCE_DIR}/tests/test.cpp`")
    target_sources( ${TESTPROJ}
                    PRIVATE
                    ${PROJECT_SOURCE_DIR}/tests/test.cpp)

    FetchContent_MakeAvailable(googletest)
    enable_testing()

    target_include_directories( ${TESTPROJ}
                                PRIVATE
                                $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
                                $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/include>)

    target_compile_features(${TESTPROJ} PRIVATE cxx_std_20)

    target_compile_options( ${TESTPROJ}
                            PRIVATE
                            $<$<CXX_COMPILER_ID:MSVC>:/std:c++20> )

    include(GoogleTest)

    target_link_libraries(${TESTPROJ} PRIVATE GTest::gtest_main)
    message(STATUS "....target_link_libraries(  ${TESTPROJ} PRIVATE GTest::gtest_main)")

    include(CTest)

    gtest_discover_tests(${TESTPROJ} XML_OUTPUT_DIR "${PROJECT_SOURCE_DIR}/tests/results")
    message(STATUS "....gtest_discover_tests(${TESTPROJ}    XML_OUTPUT_DIR \"${PROJECT_SOURCE_DIR}/tests/results\")")

    set_target_properties(${TESTPROJ} PROPERTIES EXCLUDE_FROM_ALL True)
    message(STATUS "....Completed the setup/configuration for testing.")
endif()