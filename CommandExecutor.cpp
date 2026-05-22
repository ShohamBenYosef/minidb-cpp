#include "CommandExecutor.hpp"

#include <iostream>
#include <vector>

namespace {

    void printHelp() {
        std::cout << "\nAvailable commands:\n"
                << "  insert <id> <name> <age>   Insert a new user\n"
                << "  list                       List all users\n"
                << "  find <id>                  Find user by ID\n"
                << "  update <id> <name> <age>   Update user by ID\n"
                << "  delete <id>                Delete user by ID\n"
                << "  clear                      Clear the database\n"
                << "  stats                      Show database statistics\n"
                << "  benchmark <id>             Compare linear scan vs indexed lookup\n"
                << "  seed <count>               Generate test users for benchmarking\n"              
                << "  help                       Show this help message\n"
                << "  exit                       Exit MiniDB\n";
    }
} // namespace


bool executeCommand(const Command& command, StorageEngine& storage) {
    switch (command.type)
    {
        case CommandType::Exit: {
            std::cout << "Goodbye!" << std::endl;
            return false;
        }

        case CommandType::Help:{
            printHelp();
            return true;
        }

        case CommandType::Insert: {

            // Enforce ID uniqueness, similar to a primary key.
            User existingUser;

            if (storage.findUserByIdIndexed(command.id, existingUser)) {
                std::cout << "Error: user with ID " << command.id << " alredy exists." << std::endl;
                return true;

            }
            
            User user = createUser(command.id, command.name, command.age);

            storage.saveUser(user);
            std::cout << "User inserted successfully.\n";

            return true;
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

            return true;
        }

        case CommandType::Find: {

            User foundUser;

            if (storage.findUserByIdIndexed(command.id, foundUser)) {
                std::cout << "User found:\n";
                printUser(foundUser);
            } else {
                std::cout << "User not found.\n";
            }

            return true;
        }

        case CommandType::Clear: {
            storage.clear();
            std::cout << "Database is clear.\n";
            return true;
        }

        case CommandType::Delete: {

            if (storage.deleteUserById(command.id)) {
                std::cout << "User delete successfully.\n";
            } else {
                std::cout << "User not found.\n";
            }
            return true;
        }

        case CommandType::Update: {
            User existingUser;

            if (!storage.findUserByIdIndexed(command.id, existingUser)) {
                std::cout << "User not found.\n";
                return true;
            }

            User updatedUser = createUser(command.id, command.name, command.age);

            storage.updateUserById(command.id, updatedUser);
            std::cout << "User updated successfully.\n";

            return true;
        }

        case CommandType::Benchmark: {
            User linearUser, indexedUser;

            auto linearStart = std::chrono::high_resolution_clock::now();
            bool linearSearch = storage.findUserByIdLinear(command.id, linearUser);
            auto linearEnd = std::chrono::high_resolution_clock::now();
            
            auto linearDuration = std::chrono::duration_cast<std::chrono::microseconds>(linearEnd - linearStart).count();


            auto indexedStart = std::chrono::high_resolution_clock::now();
            bool indexedSearch = storage.findUserByIdIndexed(command.id, indexedUser);
            auto indexedEnd = std::chrono::high_resolution_clock::now();

            auto indexedDuration = std::chrono::duration_cast<std::chrono::microseconds>(indexedEnd - indexedStart).count();

            std::cout << "\nBenchmark result for id " <<command.id << std::endl
                    << "Linear search: " << linearDuration <<" ms" << std::endl
                    << "Indexed search: " << indexedDuration << " ms" << std::endl;
                
            return true;
        }

        case CommandType::Seed: {
            int count = command.id;

            if (count <= 0) {
                std::cout << "Seed count must be positive." << std::endl;
                return true;
            }

            storage.clear();

            std::vector<User> users;
            users.reserve(count);

            for (int i = 0; i <= count; ++i) {
                std::string name = "User " + std::to_string(i);
                int age = 18  +(i % 60);

                users.push_back(createUser(i, name, age));
            }

            storage.saveUsersBulk(users);
        }

        case CommandType::Stats: {
            std::cout << "\nDB Stats:\n"
                    << "Users: " << storage.countUsers() << std::endl
                    << "Record size: " << storage.recordSize() << " bytes." << std::endl
                    << "Storage file: " << storage.getFilename() << std::endl
                    << "Indexed records: " << storage.indexSize() << std::endl << std::endl;
            return true;
        }

        case CommandType::Invalid: {
            if (!command.errorMessage.empty()) {
                std::cout << "Invalid command: " << command.errorMessage << '\n';
            } else {
                std::cout << "Invalid command. Type 'help' for available commands.\n";
            }
            return true;
        }

        default: {
            std::cout << "Invalid option.\n";
            return true;
        }
    }
}