#include <iostream>
#include <string>

#include "StorageErrors.hpp"
#include "CommandExecutor.hpp"




int main() {

    StorageEngine storage("users.db");
    
    std::cout << "MiniDB commnad-line interface." << std::endl << "Type 'help' if you need." << std::endl;
    
    while (true) {
        std::string line;

        std::cout << "\nminidb$ ";
        std::getline(std::cin, line);

        Command command = parseCommand(line);

        try {
            if (!executeCommand(command, storage)) {
                break;
            }
        } catch (const StorageException& ex) {
            std::cout << "Storage errror: " << ex.what() << '\n';
        } catch (const std::exception& ex) {
            std::cout << "Unexpected error: " << ex.what() << '\n';
        }
        
    }

    return 0;
}