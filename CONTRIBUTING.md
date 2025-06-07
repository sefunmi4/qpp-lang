# Contributing to Q++

Thank you for considering contributing to Q++! This project experiments with blending classical C++ and quantum concepts. To keep the code base consistent, please follow these guidelines.

## Development workflow

1. Fork the repository and create feature branches from `main`.
2. Build using CMake:
   ```bash
   mkdir build && cd build
   cmake ..
   make -j
   ctest --output-on-failure
   ```
3. Submit pull requests with clear descriptions of what was added or changed.

## Code style

- Use modern C++17 features but avoid heavy template metaprogramming.
- Keep headers lightweight and minimize global state.
- Document TODOs where architectural decisions are still open.

## Issues and discussions

Bug reports and feature requests are welcome in GitHub issues. Please provide steps to reproduce when filing bugs.
