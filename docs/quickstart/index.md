# Getting Started

## Integration Options

### CPM (Recommended)

```cmake
include(CPM.cmake)
cpmaddpackage("gh:SiddiqSoft/TimeThis#2.5.0")
target_link_libraries(myapp PRIVATE timethis::timethis)
```

### CMake FetchContent

```cmake
include(FetchContent)
FetchContent_Declare(
    timethis
    GIT_REPOSITORY https://github.com/SiddiqSoft/TimeThis.git
    GIT_TAG        v2.5.0
)
FetchContent_MakeAvailable(timethis)
target_link_libraries(myapp PRIVATE timethis::timethis)
```

### NuGet (Windows / Visual Studio)

```powershell
Install-Package SiddiqSoft.TimeThis
```

### Manual Include

Add `include/` to your include paths:

```cpp
#include "siddiqsoft/timethis.hpp"
using namespace siddiqsoft;
```
