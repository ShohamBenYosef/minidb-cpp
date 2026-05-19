# MiniDB

MiniDB is a small C++ project I’m building to better understand how simple database systems work under the hood.

The idea is to implement a lightweight binary file-based storage system from scratch, while practicing lower-level C++ topics such as file I/O, binary serialization, record storage, indexing, and basic command-line interaction.

This is mainly a learning project, but the goal is to gradually turn it into a cleaner and more structured mini database engine.

## Current Features

- Stores fixed-size `User` records
- Saves records to a binary file
- Supports append-based insertion
- Reads all stored records using a sequential scan
- Finds a user by ID
- Includes basic error handling for file operations

## Planned Features

- Interactive command-line interface
- `StorageEngine` class to separate storage logic from the main program
- In-memory hash index for faster user lookup
- Basic query parser
- Delete and update operations
- Unit tests
- Benchmarks comparing linear search and indexed lookup
- CMake build setup

## Tech Stack

- C++
- Binary file I/O
- Git / GitHub
- macOS development environment

## Build and Run

At the moment, the project can be compiled directly with:

```bash
g++ -std=c++17 -Wall -Wextra -pedantic main.cpp -o minidb
