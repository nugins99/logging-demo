# logging

A minimal example logging library demonstrating how to use `std::format_args` with `std::vformat`.

**Goal**: show a thin, thread-safe logging front-end that accepts zero or more format arguments, supports compile-time-checked format strings via `std::format_string`, and handles rvalue arguments (e.g. `std::getenv`) by creating lvalue copies before calling `std::make_format_args`.

**Files**:
- [log.hpp](log.hpp) : public header with `log` template wrapper and the `LOG` macro front-end.
- [log.cpp](log.cpp) : implementation of `log_impl` that calls `std::vformat` and writes to `std::cout`.
- [main.cpp](main.cpp) : example program that demonstrates usage.

**Usage**:

- Build (requires a C++20 compiler and standard library):

```bash
cmake -S . -B build
cmake --build build
```

- Run:

```bash
./build/test_logging
```

**Example calls** (see `main.cpp`):

- `LOG("program started");`  // zero format args
- `LOG("worker {} iteration {}", id, i);`  // compile-time-checked format when format string is a literal
- `LOG("Print an environment variable: {}", std::getenv("PATH"));`  // works even though `std::getenv` returns `char*`

**Implementation notes**:

- `LOG` macro accepts zero or more arguments via `##__VA_ARGS__` (removes trailing comma when empty).
- The header provides a single templated `log` that prefers `std::format_string<...>` when possible so format strings in code are checked at compile time.
- To support rvalue arguments (like the result of `std::getenv`) `make_format_args_for(...)` stores decayed copies in a tuple and then uses `std::make_format_args` with lvalue references. This avoids `std::make_format_args`' requirement for lvalue references.

**Notes for editors / clangd**:
- The project compiles with `-std=c++20`. If your language server reports missing `std::format` symbols, ensure clangd is using the `compile_commands.json` in `build/` or add `-std=c++20` to its compile flags.

