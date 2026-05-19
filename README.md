# MiniDB

MiniDB is a lightweight binary file-based database prototype written in C++.

The project is built as a learning-oriented systems programming project, focusing on low-level file I/O, binary serialization, record storage, indexing, modular design, and command-line interaction.

## Current Features

- Fixed-size `User` records
- Binary file persistence
- Append-based record insertion
- Sequential scan over stored records
- Search user by ID
- Interactive command-line interface
- Menu-based insert, list, find, and clear operations
- Basic error handling for file operations
- `StorageEngine` class abstraction for file-based storage operations

## Planned Features

- In-memory hash index for faster lookups
- Basic query parser
- Delete/update support
- Unit tests
- Benchmarks for linear search vs indexed lookup
- CMake build system
- Improved input validation

## Tech Stack

- C++
- Binary file I/O
- Object-oriented design
- Git/GitHub
- macOS development environment

## Build and Run

Compile:

```bash
g++ -std=c++17 -Wall -Wextra -pedantic main.cpp StorageEngine.cpp -o minidb