# TimeThis

<!-- badges -->
[![Build Status](https://dev.azure.com/siddiqsoft/siddiqsoft/_apis/build/status/SiddiqSoft.TimeThis?branchName=main)](https://dev.azure.com/siddiqsoft/siddiqsoft/_build/latest?definitionId=11&branchName=main)
[![NuGet Version](https://img.shields.io/nuget/v/SiddiqSoft.TimeThis?logo=nuget)](https://www.nuget.org/packages/SiddiqSoft.TimeThis/)
[![NuGet Downloads](https://img.shields.io/nuget/dt/SiddiqSoft.TimeThis?logo=nuget)](https://www.nuget.org/packages/SiddiqSoft.TimeThis/)
[![Tests](https://img.shields.io/azure-devops/tests/siddiqsoft/siddiqsoft/11/main.svg)](https://dev.azure.com/siddiqsoft/siddiqsoft/_build/latest?definitionId=11&branchName=main)
[![C++23](https://img.shields.io/badge/C%2B%2B-23-00599C?logo=cplusplus)](https://en.cppreference.com/w/cpp/23)
[![License BSD-3](https://img.shields.io/badge/License-BSD--3--Clause-blue)](license.md)
<!-- end badges -->

A lightweight, header-only Modern C++23 stopwatch utility for measuring code execution time with optional callbacks.

## Features

- **RAII-Based Timing**: Automatically measures elapsed time from construction to destruction.
- **Optional Callbacks**: Execute a custom callback function with the elapsed duration on scope exit.
- **std::format Support**: Native formatter specialization for C++20/C++23 `std::format`.
- **Source Location Tracking**: Automatically captures where the timer was constructed via `std::source_location`.
- **Stream Output**: Direct output stream integration via `operator<<`.
- **Header-only C++23**: Zero compiled library linkage; clean compilation on MSVC, Clang, and GCC.
- **CPM & FetchContent Integration**: Trivial one-line CMake dependency integration.
- **Native NuGet Package**: First-class Visual Studio / MSBuild packaging with Natvis visualization.

## Quick Example

=== "Basic Timing"

    ```cpp
    #include <iostream>
    #include "siddiqsoft/timethis.hpp"

    int main()
    {
        siddiqsoft::timethis timer;

        // ... do work ...

        std::cout << "Elapsed: " << timer.lap() << " us\n";
        return 0;
    }
    ```

=== "Destructor Callback"

    ```cpp
    #include <iostream>
    #include <chrono>
    #include "siddiqsoft/timethis.hpp"

    void processTask()
    {
        siddiqsoft::timethis timer([](const auto& duration) {
            auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(duration);
            std::cout << "Task took " << ms.count() << " ms\n";
        });

        // ... do work ...
    } // Callback invoked automatically on scope exit
    ```

=== "std::format"

    ```cpp
    #include <iostream>
    #include <format>
    #include "siddiqsoft/timethis.hpp"

    int main()
    {
        siddiqsoft::timethis timer;
        // ... do work ...
        std::cout << std::format("{}\n", timer);
        return 0;
    }
    ```

## Documentation

- [Getting Started](quickstart/index.md)
- [CMake & CPM Integration](quickstart/cmake.md)
- [NuGet Package](quickstart/nuget.md)
- [Dependencies (SBOM)](quickstart/dependencies.md)
- [API Reference](api/index.md)
- [Maintainer Guide](maintainers/pipelines.md)

## License

Licensed under the [BSD 3-Clause License](license.md).
