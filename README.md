TimeThis : Simple stopwatch for scope
-------------------------------------------

[![Build Status](https://dev.azure.com/siddiqsoft/siddiqsoft/_apis/build/status/SiddiqSoft.TimeThis?branchName=main)](https://dev.azure.com/siddiqsoft/siddiqsoft/_build/latest?definitionId=11&branchName=main)
![](https://img.shields.io/nuget/v/SiddiqSoft.TimeThis)
![](https://img.shields.io/github/v/tag/SiddiqSoft/TimeThis)
![](https://img.shields.io/azure-devops/tests/siddiqsoft/siddiqsoft/11)


# Objective
Provide for a simple utility class where we can time the operation of a code block and allow
the time to be updated in an optional lambda.

```cpp
auto handleHTTPMessage(auto req)
{
    if( req == HTTP_GET )
    {
        if( req == "/v1/something" )
        {
            // Consider the scenario where we would like to inform
            // the client time taken.
            auto     resp= createResponse();
            // Declaration of the lambda takes the resp object
            // and sets the header and logs to our global logger
            TimeThis tt{ [&g_logger,&resp](const auto& delta) {
                // Get the time in milliseconds..
                            auto ttx= std::chrono::duration_cast<milliseconds>(tt.elapsed());
                            // Set the diagnostic header
                            resp->addHeader("X-Diagnostic-Time", std::to_string(ttx.count()) );
                            // Log this response
                            g_logger->debug("Processing /v1/something took {}ms", ttx );
                        }
                     }; // notes the call function and starts the stopwatch
            // Some other work..Perhaps lookup db, etc.
            ..
            ..
            resp= ...; // set the body
            return resp;
        }
    }
}
```

- Support formatter for `std::format` when available
- Support use of `std::source_location` when available
- Simple "api" where all of your work is in your lambda

# Usage

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
        siddiqsoft::TimeThis tt {[&passTest] {
            // Runs when this scope ends
            passTest = true;
        }};

        auto timeTaken= tt.elapsed();
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
