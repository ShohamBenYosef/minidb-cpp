
#include "CommandParser.hpp"

#include <sstream>
#include <cctype>
#include <algorithm>

namespace {

    std::string toLower(std::string text) {
        std::transform(text.begin(), text.end(),text.begin(),
                    [](unsigned char c) {
                        return static_cast<char>(std::tolower(c)); 
                    });
        return text;
    }

    bool isExtraArguments(std::istringstream& iss) {
        std::string extra;
        return static_cast<bool>(iss >> extra);
    }

    Command makeInvalidCommand() {
        return Command{}; // invalid commad is the defult..
    }

    Command makeZeroArgumentsCommand(CommandType type, std::istringstream& iss) {
        if (isExtraArguments(iss)) {
            return makeInvalidCommand(); 
        }

        Command command;
        command.type = type;
        return command;
    }

    Command makeSingleArgumentCommand(CommandType type, std::istringstream& iss) {
        Command command;
        command.type = type;

        if (!(iss >> command.id) || isExtraArguments(iss)) {
            return makeInvalidCommand();
        }

        return command;
    }

    Command makeUserCommand(CommandType type, std::istringstream& iss) {
        Command command;
        command.type = type;

        if (!(iss >> command.id >> command.name >> command.age) || isExtraArguments(iss)) {
            return makeInvalidCommand();
        }

        return command;
    }

} // namespace {

Command parseCommand(const std::string& line) {
    std::istringstream iss(line);

    std::string action;
    iss >> action;

    action = toLower(action);



    if (action == "insert") {
        return makeUserCommand(CommandType::Insert, iss);
    }

    if (action =="list") {
        return makeZeroArgumentsCommand(CommandType::List, iss);
    }

    if (action == "find") {
        return makeSingleArgumentCommand(CommandType::Find, iss);
    }
    
    if (action == "update") {
        return makeUserCommand(CommandType::Update, iss);
    }

    if (action == "delete") {
        return makeSingleArgumentCommand(CommandType::Delete, iss);
    }

    if (action == "clear") {
        return makeZeroArgumentsCommand(CommandType::Clear, iss);
    }

    if (action == "stats") {
        return makeZeroArgumentsCommand(CommandType::Stats, iss);
    }

    if (action == "help") {
        return makeZeroArgumentsCommand(CommandType::Help, iss);
    }

    if (action == "exit") {
        return makeZeroArgumentsCommand(CommandType::Exit, iss);
    }

    return makeInvalidCommand();
}