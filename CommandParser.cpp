
#include "CommandParser.hpp"

#include <sstream>

Command parseCommand(const std::string& line) {
    std::istringstream iss(line);

    std::string action;
    iss >> action;

    Command command;

    if (action == "insert") {
        command.type = CommandType::Insert;

        if (!(iss >> command.id >> command.name >> command.age)) {
            command.type = CommandType::Invalid;
        }
        return command;
    }

    if (action =="list") {
        command.type = CommandType::List;
        return command;
    }

    if (action == "find") {
        command.type = CommandType::Find;

        if (!(iss >> command.id)){
            command.type = CommandType::Invalid;
        }
        return command;
    }

    if (action == "update") {
        command.type = CommandType::Update;

        if (!(iss >> command.id >> command.name >> command.age)) {
            command.type = CommandType::Invalid;
        }
        return command;
    }

    if (action == "delete") {
        command.type = CommandType::Delete;
        if (!(iss >> command.id)) {
            command.type = CommandType::Invalid;
        }
        return command;
    }

    if (action == "clear") {
        command.type = CommandType::Clear;
        return command;
    }

    if (action == "stats") {
        command.type = CommandType::Stats;
        return command;
    }

    if (action == "help") {
        command.type = CommandType::Help;
        return command;
    }

    if (action == "exit") {
        command.type = CommandType::Exit;
        return command;
    }

    command.type = CommandType::Invalid;
    return command;
}