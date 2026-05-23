
#include "CommandParser.hpp"

#include <sstream>
#include <cctype>
#include <algorithm>

namespace {


    const std::string INSERT_USAGE = "Usage: insert <id> <name> <age>";
    //const std::string UPDATE_USAGE = "Usage: update <id> <name> <age>";
    const std::string FIND_USAGE = "Usage: find <id>";
    //const std::string DELETE_USAGE = "Usage: delete <id>";
    const std::string BENCHMARK_USAGE = "Usage: benchmark <id>";
    const std::string SEED_USAGE = "Usage: seed <count>";
    const std::string NO_ARGUMENTS_USAGE = "This command does not accept arguments.";
    const std::string UNKNOWN_COMMAND = "Unknown command. Type 'help' for available commands.";
    
    // Start to make it SQL-Like..
    const std::string SELECT_USAGE ="Usage: SELECT * FROM users WHERE id = <id>";
    const std::string INSERT_INTO_USAGE ="Usage: INSERT INTO users VALUES <id> <name> <age>";
    const std::string DELETE_USAGE = "Usage: DELETE FROM users WHERE id = <id> OR DELETE * FROM users";
    const std::string UPDATE_USAGE = "Usage: UPDATE users SET name = <name> age = <age> WHERE id = <id>\n";
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

Command parseUpdateCommand(std::istringstream& iss) {
    std::string tableName;
    std::string setKeyword;
    std::string nameField;
    std::string nameEquals;
    std::string newName;
    std::string ageField;
    std::string ageEquals;
    int newAge;
    std::string whereKeyword;
    std::string idField;
    std::string idEquals;
    int id;
    
    if (!(iss >> tableName
              >> setKeyword
              >> nameField
              >> nameEquals
              >> newName
              >> ageField
              >> ageEquals
              >> newAge
              >> whereKeyword
              >> idField
              >> idEquals
              >> id)) {
        return makeInvalidCommand(UPDATE_USAGE);
    }

    tableName = toLower(tableName);
    setKeyword = toLower(setKeyword);
    nameField = toLower(nameField);
    ageField = toLower(ageField);
    whereKeyword = toLower(whereKeyword);
    idField = toLower(idField);

    if (tableName != "users" ||
        setKeyword != "set" ||
        nameField != "name" ||
        nameEquals != "=" ||
        ageField != "age" ||
        ageEquals != "=" ||
        whereKeyword != "where" ||
        idField != "id" ||
        idEquals != "=") {
        return makeInvalidCommand(UPDATE_USAGE);
    }

    if (isExtraArguments(iss)) {
        return makeInvalidCommand(UPDATE_USAGE);
    }

    Command command;
    command.type = CommandType::Update;
    command.id = id;
    command.name = newName;
    command.age = newAge;

    return command;
}

Command parseDeleteCommand(std::istringstream& iss) {
    std::string starOrFrom;
    
    if (!(iss >> starOrFrom)) {
        return makeInvalidCommand(DELETE_USAGE);
    }

    if (starOrFrom == "*") {
        std::string fromKeyword, tableName;
        
        if (!(iss >> fromKeyword >> tableName)) {
            return makeInvalidCommand(DELETE_USAGE);
        }
        fromKeyword = toLower(fromKeyword);
        tableName = toLower(tableName);

        if (fromKeyword != "from" || tableName != "users" || isExtraArguments(iss)) {
            return makeInvalidCommand(DELETE_USAGE);
        }

        Command command;
        command.type = CommandType::Clear;

        return command;
    } 

    starOrFrom = toLower(starOrFrom);

    if (starOrFrom != "from") {
        return makeInvalidCommand(DELETE_USAGE);
    }

    std::string tableName, whereKeyword, idKeyword, equals;
    int id;

    if (!(iss >> tableName >> whereKeyword >> idKeyword >> equals >> id)) {
        return makeInvalidCommand(DELETE_USAGE);
    }

    tableName = toLower(tableName);
    whereKeyword = toLower(whereKeyword);
    idKeyword = toLower(idKeyword);

    if (tableName != "users" || whereKeyword != "where" || idKeyword != "id" || equals != "=") {
        return makeInvalidCommand(DELETE_USAGE);
    }

    Command command;
    command.type = CommandType::Delete;
    command.id = id;

    if (isExtraArguments(iss)) {
        return makeInvalidCommand(DELETE_USAGE);
    }

    return command;
}

Command parseInsertIntoCommand(std::istringstream& iss) {
    std::string intoKeyword, tableName, valuesKeyword;

    if (!(iss >> intoKeyword >> tableName >> valuesKeyword)) {
        return makeInvalidCommand(INSERT_INTO_USAGE);
    }

    intoKeyword = toLower(intoKeyword);
    tableName = toLower(tableName);
    valuesKeyword = toLower(valuesKeyword);

    if (intoKeyword != "into" || tableName != "users" || valuesKeyword != "values") {
        return makeInvalidCommand(INSERT_INTO_USAGE);
    }

    Command command;
    command.type = CommandType::Insert;

    if (!(iss >> command.id >> command.name >> command.age) || isExtraArguments(iss)) {
        return makeInvalidCommand(INSERT_INTO_USAGE);
    }
    
    return command;
}


// SQL-Like functions
Command parseSelectCommand(std::istringstream& iss) {
    std::string star, fromKeyWord, tableName;

    if (!(iss >> star >> fromKeyWord >> tableName)) {
        return makeInvalidCommand(SELECT_USAGE);
    }

    fromKeyWord = toLower(fromKeyWord);
    tableName = toLower(tableName);

    if (star != "*" || fromKeyWord != "from" || tableName != "users") {
        return makeInvalidCommand(SELECT_USAGE);
    }

    std::string whereKeyword;

    // SELECT * from users *WHERE...*
    if (!(iss >> whereKeyword)) {
        Command command;
        command.type = CommandType::List;
        return command;
    }

    whereKeyword = toLower(whereKeyword);

    if (whereKeyword != "where") {
        return makeInvalidCommand(SELECT_USAGE);
    }

    std::string field, equls;
    int id;

    if (!(iss >> field >> equls >> id)) {
        return makeInvalidCommand(SELECT_USAGE);
    }

    field = toLower(field);

    if (field != "id" || equls != "=" || isExtraArguments(iss)) {
        return makeInvalidCommand(SELECT_USAGE);
    }

    Command command;
    command.type = CommandType::Find;
    command.id = id;
    return command;
}




Command parseCommand(const std::string& line) {
    std::istringstream iss(line);

    std::string action;
    iss >> action;

    action = toLower(action);


    // if (action == "insert") {
    //     return makeUserCommand(CommandType::Insert, iss ,INSERT_USAGE);
    // }

    // if (action =="list") {
    //     return makeZeroArgumentsCommand(CommandType::List, iss, NO_ARGUMENTS_USAGE);
    // }

    // if (action == "find") {
    //     return makeSingleArgumentCommand(CommandType::Find, iss, FIND_USAGE);
    // }
    
    // if (action == "update") {
    //     return makeUserCommand(CommandType::Update, iss, UPDATE_USAGE);
    // }

    // if (action == "delete") {
    //     return makeSingleArgumentCommand(CommandType::Delete, iss,DELETE_USAGE);
    // }

    // if (action == "clear") {
    //     return makeZeroArgumentsCommand(CommandType::Clear, iss, NO_ARGUMENTS_USAGE);
    // }

    if (action == "stats") {
        return makeZeroArgumentsCommand(CommandType::Stats, iss, NO_ARGUMENTS_USAGE);
    }

    if (action == "benchmark") {
        return makeSingleArgumentCommand(CommandType::Benchmark, iss, BENCHMARK_USAGE);
    }
    
    if (action == "seed") {
        return makeSingleArgumentCommand(CommandType::Seed, iss, SEED_USAGE);
    }

    if (action == "help") {
        return makeZeroArgumentsCommand(CommandType::Help, iss, NO_ARGUMENTS_USAGE);
    }

    if (action == "exit") {
        return makeZeroArgumentsCommand(CommandType::Exit, iss, NO_ARGUMENTS_USAGE);
    }


    // SQL - Like
    if (action == "select") {
       return parseSelectCommand(iss);
    }

    if (action == "insert") {
        return parseInsertIntoCommand(iss);
    }
    if (action == "delete") {
        return parseDeleteCommand(iss);
    }

    if (action == "update") {
        return parseUpdateCommand(iss);
    }
    return makeInvalidCommand(UNKNOWN_COMMAND);
}


