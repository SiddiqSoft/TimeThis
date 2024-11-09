cmake_minimum_required(VERSION 3.25)

if(${${PROJECT_NAME}_BUILD_TESTS})
    include(FetchContent)
    include(GNUInstallDirs)

    set(TESTPROJ ${PROJECT_NAME}_tests)

    FetchContent_Declare(googletest URL https://github.com/google/googletest/releases/download/v1.15.2/googletest-1.15.2.tar.gz
                                    FIND_PACKAGE_ARGS NAMES GTest)

    add_executable(${TESTPROJ})

    target_sources( ${TESTPROJ}
                    PRIVATE
                    ${PROJECT_SOURCE_DIR}/tests/test.cpp)

    FetchContent_MakeAvailable(googletest)
    enable_testing()

    target_compile_features(${TESTPROJ} PRIVATE cxx_std_20)

    target_compile_options( ${TESTPROJ}
                            PRIVATE
                            $<$<CXX_COMPILER_ID:MSVC>:/std:c++20> )

    include(GoogleTest)

    target_link_libraries(${TESTPROJ} PRIVATE GTest::gtest_main)

    include(CTest)

    gtest_discover_tests(${TESTPROJ} XML_OUTPUT_DIR "${PROJECT_SOURCE_DIR}/tests/results")
    message(STATUS "  Finished configuring for ${PROJECT_NAME}_BUILD_TESTS = ${${PROJECT_NAME}_BUILD_TESTS}")
endif()