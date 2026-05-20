# MiniDB

MiniDB is a lightweight binary file-based database prototype written in C++.

The project is built as a learning-oriented systems programming project, focusing on low-level file I/O, binary serialization, record storage, indexing, modular design, and command-line interaction.

## Current Features

- Fixed-size `User` records
- Binary file persistence
- Append-based record insertion
- Sequential scan over stored records
- Search user by ID
- Duplicate user ID prevention
- Interactive command-line interface
- Menu-based insert, list, find, and clear operations
- Validated integer input handling
- Basic error handling for file operations
- Command parser for text-based CLI commands
- Text commands: insert, list, find, update, delete, clear, stats, help, exit
- `StorageEngine` class abstraction for file-based storage operations


## Tech Stack

- C++
- Binary file I/O
- Object-oriented design
- Git/GitHub

## Build and Run

Compile:

```bash
g++ -std=c++17 -Wall -Wextra -pedantic main.cpp StorageEngine.cpp -o minidb
./minidb