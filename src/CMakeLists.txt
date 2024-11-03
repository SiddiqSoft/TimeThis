set(header_path "${CMAKE_SOURCE_DIR}/include")
set(headers ${header_path}/siddiqsoft/timethis.hpp)

add_library(${PROJECT_NAME} INTERFACE ${headers})
target_include_directories( ${PROJECT_NAME}
                            INTERFACE "${CMAKE_SOURCE_DIR}/include")

target_compile_features(${PROJECT_NAME} INTERFACE cxx_std_20)
