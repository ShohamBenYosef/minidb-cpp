
#include "CommandParser.hpp"

#include <sstream>
#include <cctype>
#include <algorithm>

namespace {


    const std::string INSERT_USAGE = "Usage: insert <id> <name> <age>";
    const std::string UPDATE_USAGE = "Usage: update <id> <name> <age>";
    const std::string FIND_USAGE = "Usage: find <id>";
    const std::string DELETE_USAGE = "Usage: delete <id>";
    const std::string BENCHMARK_USAGE = "Usage: benchmark <id>";
    const std::string NO_ARGUMENTS_USAGE = "This command does not accept arguments.";
    const std::string UNKNOWN_COMMAND = "Unknown command. Type 'help' for available commands.";


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

    Command makeInvalidCommand(const std::string& message) {
        Command command;
        command.type = CommandType::Invalid;
        command.errorMessage = message;
        return command; 
    }

    Command makeZeroArgumentsCommand(CommandType type, std::istringstream& iss, const std::string& message) {
        if (isExtraArguments(iss)) {
            return makeInvalidCommand(message); 
        }

        Command command;
        command.type = type;
        return command;
    }

    Command makeSingleArgumentCommand(CommandType type, std::istringstream& iss, const std::string& message) {
        Command command;
        command.type = type;

        if (!(iss >> command.id) || isExtraArguments(iss)) {
            return makeInvalidCommand(message);
        }

        return command;
    }

    Command makeUserCommand(CommandType type, std::istringstream& iss, const std::string& message) {
        Command command;
        command.type = type;

        if (!(iss >> command.id >> command.name >> command.age) || isExtraArguments(iss)) {
            return makeInvalidCommand(message);
        }

        return command;
    }

} // namespace 

Command parseCommand(const std::string& line) {
    std::istringstream iss(line);

    std::string action;
    iss >> action;

    action = toLower(action);



    if (action == "insert") {
        return makeUserCommand(CommandType::Insert, iss ,INSERT_USAGE);
    }

    if (action =="list") {
        return makeZeroArgumentsCommand(CommandType::List, iss, NO_ARGUMENTS_USAGE);
    }

    if (action == "find") {
        return makeSingleArgumentCommand(CommandType::Find, iss, FIND_USAGE);
    }
    
    if (action == "update") {
        return makeUserCommand(CommandType::Update, iss, UPDATE_USAGE);
    }

    if (action == "delete") {
        return makeSingleArgumentCommand(CommandType::Delete, iss,DELETE_USAGE);
    }

    if (action == "clear") {
        return makeZeroArgumentsCommand(CommandType::Clear, iss, NO_ARGUMENTS_USAGE);
    }

    if (action == "stats") {
        return makeZeroArgumentsCommand(CommandType::Stats, iss, NO_ARGUMENTS_USAGE);
    }

    if (action == "benchmark") {
        return makeSingleArgumentCommand(CommandType::Benchmark, iss, BENCHMARK_USAGE);
    }

    if (action == "help") {
        return makeZeroArgumentsCommand(CommandType::Help, iss, NO_ARGUMENTS_USAGE);
    }

    if (action == "exit") {
        return makeZeroArgumentsCommand(CommandType::Exit, iss, NO_ARGUMENTS_USAGE);
    }

    return makeInvalidCommand(UNKNOWN_COMMAND);
}