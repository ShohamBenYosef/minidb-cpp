#include <iostream>
#include <string>
#include <limits>
#include <vector>

#include "CommandParser.hpp"
#include "StorageEngine.hpp"
#include "StorageErrors.hpp"



void printHelp() {
    std::cout << "\nAvailable commands:\n"
              << "  insert <id> <name> <age>   Insert a new user\n"
              << "  list                       List all users\n"
              << "  find <id>                  Find user by ID\n"
              << "  update <id> <name> <age>   Update user by ID\n"
              << "  delete <id>                Delete user by ID\n"
              << "  clear                      Clear the database\n"
              << "  stats                      Show database statistics\n"
              << "  help                       Show this help message\n"
              << "  exit                       Exit MiniDB\n";
}



int main() {
    StorageEngine storage("users.db");
    while (true) {
        std::string line;
        std::cout << "\nminidb$ ";
        std::getline(std::cin, line);

        Command command = parseCommand(line);

        try {
            switch (command.type) {
                case CommandType::Exit: {
                    std::cout << "Goodbye!\n";
                    return 0;
                }

                case CommandType::Help:{
                    printHelp();
                    break;
                }

                case CommandType::Insert: {

                    // Enforce ID uniqueness, similar to a primary key.
                    User existingUser;

                    if (storage.findUserById(command.id, existingUser)) {
                        std::cout << "Error: user with ID " << command.id << " alredy exists." << std::endl;
                        break;
                    }
                    
                    User user = createUser(command.id, command.name, command.age);

                    storage.saveUser(user);
                    std::cout << "User inserted successfully.\n";

                    break;
                }

                case CommandType::List: {
                    std::vector<User> users = storage.loadAllUsers();
                    
                    if (users.empty()) {
                        std::cout << "There is no users." << std::endl;
                    } else {
                        std::cout << "\nAll users:\n";
                    
                        for (const User& user : users) {
                            printUser(user);
                        }
                    }

                    break;
                }

                case CommandType::Find: {

                    User foundUser;

                    if (storage.findUserById(command.id, foundUser)) {
                        std::cout << "User found:\n";
                        printUser(foundUser);
                    } else {
                        std::cout << "User not found.\n";
                    }

                    break;
                }

                case CommandType::Clear: {
                    storage.clear();
                    std::cout << "Database is clear.\n";
                    break;
                }

                case CommandType::Delete: {

                    if (storage.deleteUserById(command.id)) {
                        std::cout << "User delete successfully.\n";
                    } else {
                        std::cout << "User not found.\n";
                    }
                    break;
                }

                case CommandType::Update: {
                    User existingUser;

                    if (!storage.findUserById(command.id, existingUser)) {
                        std::cout << "User not found.\n";
                        break;
                    }

                    User updatedUser = createUser(command.id, command.name, command.age);

                    storage.updateUserById(command.id, updatedUser);
                    std::cout << "User updated successfully.\n";

                    break;
                }

                case CommandType::Stats: {
                    std::cout << "\nDB Stats:\n"
                            << "Users: " << storage.countUsers() << std::endl
                            << "Record size: " << storage.recordSize() << " bytes." << std::endl
                            << "Storage file: " << storage.getFilename() << std::endl << std::endl;
                    break;
                }

                case CommandType::Invalid: {
                    std::cout << "Invaid command. try 'help'.. " << std::endl;
                    break;
                }

                default: {
                    std::cout << "Invalid option.\n";
                    break;
                }
            }
        } catch (const StorageException& ex) {
            std::cout << "Storage errror: " << ex.what() << '\n';
        } catch (const std::exception& ex) {
            std::cout << "Unexpected error: " << ex.what() << '\n';
        }
        
    }

    return 0;
}