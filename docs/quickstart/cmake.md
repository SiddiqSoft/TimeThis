# CMake & CPM Integration

Modern C++ CMake integration using CPM.cmake:

```cmake
cmake_minimum_required(VERSION 3.31)
project(sample_app LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)

include(CPM.cmake)
cpmaddpackage("gh:SiddiqSoft/TimeThis#2.5.0")

add_executable(sample_app main.cpp)
target_link_libraries(sample_app PRIVATE timethis::timethis)
```
