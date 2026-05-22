cmake_minimum_required(VERSION 3.16)

project(MiniDBCpp VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(minidb
    main.cpp
    StorageEngine.cpp
    CommandParser.cpp
    CommandExecutor.cpp
)

target_compile_options(minidb PRIVATE
    -Wall
    -Wextra
    -pedantic
)