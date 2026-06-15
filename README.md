# highperf

A high-performance, from-scratch message-transport framework in modern C++ — a low-level framed message bus. Built in public as a deep-dive into operating systems, networking, and C++.

**🌐 Project page:** https://rknoche6.github.io/high-perf-transport-msg-framework/

> Status: M1 (reactor core) in progress. Built as a **test-driven workbook** — each module ships with a full test suite and a stub you implement.

## Build & test

```sh
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build --parallel
ctest --test-dir build --output-on-failure
```

## The learning loop

Each module comes as three pieces: a **header** (the API), a **test** that specifies its behavior, and a **stub `.cpp`** full of `// TODO`s.

1. Pick a module under `src/` — start with `src/base/Buffer.cpp`.
2. Run `ctest` — its test is **red**.
3. Implement the `// TODO`s, guided by the header and `tests/.../*Test.cpp`.
4. Run `ctest` until it's **green**.

> The `nc` echo manual-test arrives with the socket module (next up).
