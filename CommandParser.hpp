#ifndef COMMAND_PARSER_HPP
#define COMMAND_PARSER_HPP

#include <iostream>
#include <string>



enum class CommandType {
    Insert,
    List,
    Find,
    Delete,
    Update,
    Clear,
    Stats,
    Benchmark,
    Help,
    Exit,
    Invalid
};

struct Command {
    CommandType type = CommandType::Invalid;
    int id = 0;
    std::string name;
    int age = 0;
    std::string errorMessage;
};


Command parseCommand(const std::string& line);


#endif