cmake_minimum_required(VERSION 3.25)

if(opt_${PROJECT_NAME}_testing)
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

    include(CTest)

    gtest_discover_tests(${TESTPROJ} XML_OUTPUT_DIR "${PROJECT_SOURCE_DIR}/tests/results")
endif()