# MiniDB C++

MiniDB is a lightweight file-based database prototype written in modern C++.

The project was built as a systems programming exercise focused on binary file storage, fixed-size records, command parsing, error handling, indexing, and basic performance comparison between linear scan and indexed lookup.

MiniDB is not intended to replace a real database. Its purpose is to demonstrate how low-level database-like components can be implemented from scratch using C++.

## What the Project Demonstrates

MiniDB stores `User` records in a binary file and exposes a small command-line interface for interacting with the data.

Each record is stored in a fixed-size binary format, which allows the program to calculate record positions inside the file and perform direct access using file offsets.


## Core Idea

The database stores records sequentially in a binary file:

## Tech Stack
C++
Binary file I/O
Object-oriented design
Git/GitHub

## Project Structure

```text
MiniDB_cpp/
├── CMakeLists.txt
├── README.md
├── include/
│   ├── StorageEngine.hpp
│   ├── StorageErrors.hpp
│   ├── CommandParser.hpp
│   └── CommandExecutor.hpp
└── src/
    ├── main.cpp
    ├── StorageEngine.cpp
    ├── CommandParser.cpp
    └── CommandExecutor.cpp

```text
[User][User][User][User]