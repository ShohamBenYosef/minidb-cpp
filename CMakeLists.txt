cmake_minimum_required(VERSION 3.16)

project(MiniDBCpp VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(minidb
    src/main.cpp
    src/StorageEngine.cpp
    src/CommandParser.cpp
    src/CommandExecutor.cpp
)

target_include_directories(minidb PRIVATE
    include
)

target_compile_options(minidb PRIVATE
    -Wall
    -Wextra
    -pedantic
)

add_executable(test_parser
    tests/test_parser.cpp
    src/CommandParser.cpp
)

target_include_directories(test_parser PRIVATE
    include
)

target_compile_options(test_parser PRIVATE
    -Wall
    -Wextra
    -pedantic
)