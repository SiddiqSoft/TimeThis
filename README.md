TimeThis : Run a lambda in destructor
-------------------------------------------

[![CodeQL](https://github.com/SiddiqSoft/TimeThis/actions/workflows/codeql-analysis.yml/badge.svg)](https://github.com/SiddiqSoft/TimeThis/actions/workflows/codeql-analysis.yml)
[![Build Status](https://dev.azure.com/siddiqsoft/siddiqsoft/_apis/build/status/SiddiqSoft.TimeThis?branchName=main)](https://dev.azure.com/siddiqsoft/siddiqsoft/_build/latest?definitionId=10&branchName=main)
![](https://img.shields.io/nuget/v/SiddiqSoft.TimeThis)
![](https://img.shields.io/github/v/tag/SiddiqSoft/TimeThis)
![](https://img.shields.io/azure-devops/tests/siddiqsoft/siddiqsoft/10)
![](https://img.shields.io/azure-devops/coverage/siddiqsoft/siddiqsoft/10)


# Objective
- A simpler "run on end" vs. the custom deletor available via `std::unique_ptr<>`. It is important to note that we do not provide an "owning" helper as this is better accomplished via the destructor code for `std::unique_ptr<>`. Avoid writing code that exists in std.
- Use the nuget [SiddiqSoft.TimeThis](https://www.nuget.org/packages/SiddiqSoft.TimeThis/)
- Copy paste..whatever works.


```cpp
#include "gtest/gtest.h"
#include "siddiqsoft/TimeThis.hpp"


TEST(examples, Example1)
{
    bool passTest {false};

    try
    {
        // Use initializer list-style instantiation; we do not allow move/assignment construction.
        // Note that the `()` is not required when the lambda/function takes no argument.
        siddiqsoft::TimeThis roe {[&passTest] {
            // Runs when this scope ends
            passTest = true;
        }};
    }
    catch (...) {
        EXPECT_TRUE(false); // if we throw then the test fails.
    }

    // Iff the lambda runs, it should be true
    EXPECT_TRUE(passTest);
}
```


<small align="right">

&copy; 2021 Siddiq Software LLC. All rights reserved.

</small>
