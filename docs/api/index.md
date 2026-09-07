# API Reference

All declarations reside in `namespace siddiqsoft`.

## Header

```cpp
#include "siddiqsoft/timethis.hpp"
using namespace siddiqsoft;
```

---

## Classes

### `class timethis`

RAII-based stopwatch class measuring elapsed time with optional callback invocation on destruction.

```cpp
namespace siddiqsoft {
    class timethis;
}
```

#### Type Aliases

| Alias | Definition | Description |
| :--- | :--- | :--- |
| `duration_type` | `std::chrono::system_clock::duration` | System clock duration type used for elapsed measurements |
| `time_point_type` | `std::chrono::system_clock::time_point` | Time point type captured upon construction or reset |
| `callback_type` | `std::function<void(const std::chrono::system_clock::duration&)>` | Callback invoked with elapsed duration upon destruction |

#### Constructors & Destructor

```cpp
explicit timethis(const std::source_location& sl = std::source_location::current()) noexcept;
```
Constructs the timer, capturing the current time and caller source location.

```cpp
explicit timethis(callback_type&& callback,
                  const std::source_location& sl = std::source_location::current()) noexcept;
```
Constructs the timer with a completion callback to be invoked upon destruction.

```cpp
~timethis() noexcept;
```
Destructor. If a callback was provided, invokes it with `elapsed()`.

#### Semantics (Copy & Move)

```cpp
timethis(const timethis&) = delete;
timethis& operator=(const timethis&) = delete;
timethis(timethis&&) = delete;
timethis& operator=(timethis&&) = delete;
```
Single-ownership RAII semantics. Timers cannot be copied or moved.

#### Member Functions

##### `elapsed()`

```cpp
[[nodiscard]] duration_type elapsed() const noexcept;
```
Calculates and returns the duration elapsed since construction or the last `reset()`.

##### `lap()`

```cpp
template <typename DC = std::chrono::microseconds>
[[nodiscard]] std::string lap() const noexcept;
```
Returns elapsed time formatted as a numeric string in the duration unit specified by `DC` (defaults to `std::chrono::microseconds`).

##### `to_string()`

```cpp
template <typename charT = char>
[[nodiscard]] auto to_string() const;
```
Returns a human-readable string containing the source location function name, start timestamp, and elapsed microseconds.

##### `reset()`

```cpp
void reset() noexcept;
```
Resets the timer's start time to the current clock time.

---

## Stream & Formatter Support

### Stream Operator

```cpp
std::ostream& operator<<(std::ostream& os, const timethis& src);
```
Writes `<function_name> took <elapsed>ns` to the output stream.

### `std::formatter` Specialization

```cpp
template <class charT>
struct std::formatter<siddiqsoft::timethis, charT>;
```
Enables direct formatting via `std::format("{}", timer)` matching `to_string()`.
