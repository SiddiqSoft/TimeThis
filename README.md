# TimeThis

A lightweight, header-only C++23 stopwatch utility for measuring code execution time with optional callbacks.

[![Build Status](https://dev.azure.com/siddiqsoft/siddiqsoft/_apis/build/status/SiddiqSoft.TimeThis?branchName=main)](https://dev.azure.com/siddiqsoft/siddiqsoft/_build/latest?definitionId=11&branchName=main)
![NuGet](https://img.shields.io/nuget/v/SiddiqSoft.TimeThis)
![GitHub Tag](https://img.shields.io/github/v/tag/SiddiqSoft/TimeThis)
![Tests](https://img.shields.io/azure-devops/tests/siddiqsoft/siddiqsoft/11)
![Coverage](https://img.shields.io/azure-devops/coverage/siddiqsoft/siddiqsoft/11)

## Features

- **RAII-based timing**: Automatically measures elapsed time from construction to destruction
- **Optional callbacks**: Execute a function with the elapsed duration on scope exit
- **std::format support**: Format timing information using C++20 `std::format`
- **Source location tracking**: Automatically captures where the timer was created
- **Header-only**: No compilation required, just include and use
- **C++23 ready**: Uses modern C++ features and best practices
- **No copy/move semantics**: Single ownership prevents accidental misuse

## Quick Start

### Basic Usage

```cpp
#include "siddiqsoft/timethis.hpp"

// Simple timing without callback
{
    siddiqsoft::timethis timer;
    // ... do work ...
    auto elapsed = timer.elapsed();
}

// With callback
{
    siddiqsoft::timethis timer([](const auto& duration) {
        auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(duration);
        std::cout << "Operation took " << ms.count() << "ms\n";
    });
    // ... do work ...
} // Callback invoked here with elapsed time
```

### With std::format

```cpp
#include "siddiqsoft/timethis.hpp"
#include <format>

siddiqsoft::timethis timer;
std::this_thread::sleep_for(std::chrono::milliseconds(100));
std::cout << std::format("{}\n", timer);
```

### Stream Output

```cpp
siddiqsoft::timethis timer;
std::this_thread::sleep_for(std::chrono::milliseconds(100));
std::cout << timer << std::endl;  // Outputs: function_name took 100000000ns
```

## Installation

### NuGet
```
Install-Package SiddiqSoft.TimeThis
```

### CMake (FetchContent)
```cmake
FetchContent_Declare(
    timethis
    GIT_REPOSITORY https://github.com/SiddiqSoft/TimeThis.git
    GIT_TAG main
)
FetchContent_MakeAvailable(timethis)

target_link_libraries(${PROJECT_NAME} timethis::timethis)
```

## Development

### Build with Tests
```bash
cmake --preset Apple-Debug
cmake --build build/Apple-Debug
```

### Run Tests
```bash
./build/Apple-Debug/tests/timethis_tests
```

### Available Presets
- `Apple-Debug` / `Apple-Release` - macOS with LLVM 22+
- `Linux-Clang-Debug` / `Linux-Clang-Release` - Linux with Clang
- `Linux-GCC-Debug` / `Linux-GCC-Release` - Linux with GCC
- `Windows-Debug` / `Windows-Release` - Windows with MSVC

## Requirements

- C++23 compiler (Clang 18+, GCC 13+, MSVC 2022+)
- CMake 3.29+

## License

BSD 3-Clause License - See LICENSE file for details

