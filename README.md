# MiniDB C++

MiniDB is a lightweight binary file based database prototype written in C++.

The project was built as a systems programming exercise focused on binary file storage, fixed size records, command parsing, error handling, indexing, validation, and performance comparison between linear scan and indexed lookup.

MiniDB is not intended to replace a real database. Its purpose is to demonstrate low level database like components can be implemented from scratch using C++.

## What the Project Demonstrates

MiniDB stores `User` records in a binary file and exposes a small SQL like command line interface for interacting with the data.

Each record is stored in a fixedsize binary format, which allows the program to calculate record positions inside the file and perform direct access using file offsets.


## Storage Model

MiniDB currently stores one fixed schema:

```cpp
struct User {
    int id;
    char name[32];
    int age;
};
```


## Indexing

MiniDB builds an in memory index when the storage engine starts.

The index maps user IDs to file offsets:

```text
user id -> file offset
```

Example:

```text
1 -> 0
2 -> 40
3 -> 80
```

This allows indexed lookup by id instead of scanning the entire file.

## Supported Commands

```sql
INSERT INTO users VALUES <id> <name> <age>

SELECT * FROM users
SELECT * FROM users WHERE id = <id>

UPDATE users SET name = <name> age = <age> WHERE id = <id>

DELETE FROM users WHERE id = <id>
DELETE * FROM users

stats
benchmark <id>
seed <count>
help
exit
```
d search: 3 microseconds
```

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
├── src/
│   ├── main.cpp
│   ├── StorageEngine.cpp
│   ├── CommandParser.cpp
│   └── CommandExecutor.cpp
└── tests/
    └── test_parser.cpp
```

## Build and Run

Using CMake:

```bash
cmake -S . -B build
cmake --build build
./build/minidb
```

Manual compilation:

```bash
g++ -std=c++17 -Wall -Wextra -pedantic \
    src/main.cpp \
    src/StorageEngine.cpp \
    src/CommandParser.cpp \
    src/CommandExecutor.cpp \
    -Iinclude \
    -o minidb

./minidb
```

## Tests

The project includes basic parser tests using standard C++ `assert`.

Build and run tests:

```bash
cmake -S . -B build
cmake --build build
./build/test_parser
```
