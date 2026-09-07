# Project Dependencies

This document is automatically generated from `CMakeLists.txt` files for `timethis`.

## Dependency Diagram

```mermaid
graph TD
    timethis["timethis::timethis"]

    subgraph Test["Test Dependencies (Optional)"]
        GTEST["gtest v1.17.0"]
    end

    timethis -. "timethis_BUILD_TESTS=ON" .-> GTEST
```

## Dependency Breakdown

| Dependency | Repository / Target | Version | Type | Scope / Platform |
| :--- | :--- | :--- | :--- | :--- |
| **gtest** | [`google/googletest`](https://github.com/google/googletest) | v1.17.0 | `CPM` | Test Target Only (`timethis_BUILD_TESTS=ON`) |
