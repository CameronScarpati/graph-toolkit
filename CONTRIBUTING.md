# Contributing to Graph Toolkit

Thank you for your interest in contributing! This document provides guidelines and instructions for contributing to the project.

## Development Setup

### Prerequisites

- **C++ Compiler**: Clang 14+ or GCC 12+ with C++20 support
- **CMake**: Version 3.27 or newer
- **clang-format**: For code formatting (version 14+)

### Building

```sh
git clone https://github.com/CameronScarpati/graph-toolkit.git
cd graph-toolkit
cmake -B build -S .
cmake --build build
```

### Running Tests

```sh
cd build && ctest --output-on-failure
```

Or run the test binary directly:

```sh
./bin/testing
```

## Code Style

This project uses [clang-format](https://clang.llvm.org/docs/ClangFormat.html) with a WebKit-based configuration. The rules are defined in `.clang-format`:

- **Column limit**: 100 characters
- **Style**: WebKit
- **No short functions on a single line**

Format your code before committing:

```sh
clang-format -i -style=file src/*.cpp include/*.h tests/*.cpp
```

The CI pipeline will reject code that does not conform to the formatting rules.

## Compiler Warnings

All code must compile cleanly with the following flags:

```
-Wall -Werror -Wextra -pedantic -pedantic-errors
```

Warnings are treated as errors. Fix all warnings before submitting a pull request.

## Making Changes

1. Fork the repository
2. Create a feature branch from `main`: `git checkout -b feature/your-feature`
3. Make your changes
4. Ensure all tests pass and code is formatted
5. Commit with a clear, descriptive message
6. Push to your fork and open a pull request

### Commit Messages

Write clear, concise commit messages that describe what changed and why:

- `feat: add Dijkstra shortest path algorithm`
- `fix: correct edge weight validation for zero values`
- `test: add boundary condition tests for empty graphs`
- `docs: update API reference with complexity analysis`

### Pull Request Guidelines

- Keep PRs focused on a single change
- Include a description of what changed and why
- Add or update tests for any new functionality
- Ensure the CI pipeline passes before requesting review

## Testing

- All new features must include corresponding tests
- Use the existing `GraphTest` fixture in `tests/graph_test.cpp`
- Test both normal operation and error/edge cases
- Use `EXPECT_THROW` for exception path testing

## Reporting Issues

When reporting bugs, please include:

- Steps to reproduce the issue
- Expected vs. actual behavior
- Compiler and OS information
- Minimal code example if applicable
