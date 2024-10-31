cmake_minimum_required(VERSION 3.25)

# The project version is updated below with the tag from git
#   We need to support "C" language in order for the googletest to build.
project(TimeThis
        VERSION 0.0.0
        LANGUAGES CXX C
        HOMEPAGE_URL "%%myurl%%"
        DESCRIPTION "A header-only library to measure time elapsed for a given scope."
)

include(FetchContent)
include(GNUInstallDirs)

# ----------------------------------------
# -- Declarations for our library --------
# https://github.com/friendlyanon/cmake-init-header-only/tree/master
#

add_library(${PROJECT_NAME}_${PROJECT_NAME} INTERFACE .)
add_library(siddiqsoft_${PROJECT_NAME} ALIAS ${PROJECT_NAME}_${PROJECT_NAME})

set_property(
        TARGET ${PROJECT_NAME}_${PROJECT_NAME}
        PROPERTY
        EXPORT_NAME ${PROJECT_NAME}
        )


target_include_directories(
    ${PROJECT_NAME}_${PROJECT_NAME}
    INTERFACE
    "\$<BUILD_INTERFACE:${$PROJECT_SOURCE_DIR}/src>"
    "\$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>"
)

target_compile_features(${PROJECT_NAME}_${PROJECT_NAME} INTERFACE cxx_std_20)

# ---------------------------------------------
# -- Installtion rules for our library --------
# https://github.com/friendlyanon/cmake-init-header-only/blob/master/cmake/install-rules.cmake
