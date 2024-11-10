cmake_minimum_required(VERSION 3.25)

if(${${PROJECT_NAME}_BUILD_TESTS})
    include(FetchContent)
    include(GNUInstallDirs)
    include(GoogleTest)

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
    # For GCC/Clang
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O0 -fprofile-arcs -ftest-coverage")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -O0 -fprofile-arcs -ftest-coverage")
    target_compile_options( ${TESTPROJ}
                            PRIVATE
                            $<$<CXX_COMPILER_ID:MSVC>:/std:c++20> )
    target_link_libraries(${TESTPROJ} PRIVATE GTest::gtest_main)
    gtest_discover_tests(${TESTPROJ} XML_OUTPUT_DIR "${PROJECT_SOURCE_DIR}/tests/results")

    include(CTest)
    message(STATUS "  Finished configuring for ${PROJECT_NAME}_BUILD_TESTS = ${${PROJECT_NAME}_BUILD_TESTS}")
endif()