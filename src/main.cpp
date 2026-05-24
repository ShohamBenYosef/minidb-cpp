#include <exception>
#include <iostream>
#include <string>

#include "CommandParser.hpp"
#include "CommandExecutor.hpp"
#include "StorageEngine.hpp"
#include "StorageErrors.hpp"


int main() {

    StorageEngine storage("users.db");
    
    std::cout << "MiniDB commnad-line interface." << std::endl << "Type 'help' if you need." << std::endl;
    
    while (true) {
        std::string line;

        std::cout << "\nminidb$ ";
        if (!std::getline(std::cin, line)) {
            std::cout << "\nExit." << std::endl;
        }

        Command command = parseCommand(line);

        try {
            if (!executeCommand(command, storage)) {
                break;
            }
        } catch (const StorageException& ex) {
            std::cout << "Storage errror: " << ex.what() << std::endl;
        } catch (const std::exception& ex) {
            std::cout << "Unexpected error: " << ex.what() << std::endl;
        }
        
    }

    return 0;
}