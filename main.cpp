#include <iostream>
#include <string>
#include "StorageEngine.hpp"
#include <limits>


int readInt(const std::string& prompt){
    int value;

    while (true) {
        std::cout << prompt;
        std::cin >> value;

        if(std::cin) {
            return value;
        }

        std::cout << "Invalid input." << std::endl;

        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
}


int main() {
    StorageEngine storage("users.db");

    int choice = 0;

    while (true) {
        std::cout << "\nMiniDB Menu:\n"
                  << "1. Insert user\n"
                  << "2. List all users\n"
                  << "3. Find user by id\n"
                  << "4. Clear database\n"
                  << "5. Delete user by id\n"
                  << "0. Exit\n";

        choice = readInt("Choose option: ");

        switch (choice) {
            case 0: {
                std::cout << "Goodbye!\n";
                return 0;
            }

            case 1: {
                std::string name;

                int id = readInt("Enter id: ");
                
                // check "primary key"..
                User existingUser;
                
                if (storage.findUserById(id, existingUser)){ 
                    std::cout << "Error: user with ID " << id << " is alredy exists." << std::endl;
                    break;
                }

                std::cout << "Enter name: ";
                std::cin >> name;

                int age = readInt("Enter age: ");

                User user = createUser(id, name, age);
                

                if (storage.saveUser(user)) {
                    std::cout << "User inserted successfully.\n";
                } else {
                    std::cout << "Failed to insert user.\n";
                }

                break;
            }

            case 2: {
                std::cout << "\nAll users:\n";
                storage.loadAllUsers();
                break;
            }

            case 3: {
                User foundUser;

                int id = readInt("Enter id to search: ");

                if (storage.findUserById(id, foundUser)) {
                    std::cout << "User found:\n";
                    printUser(foundUser);
                } else {
                    std::cout << "User not found.\n";
                }

                break;
            }

            case 4: {
                if (storage.clear()) {
                    std::cout << "Database cleared successfully.\n";
                } else {
                    std::cout << "Failed to clear database.\n";
                }

                break;
            }

            case 5: {
                int id = readInt("Enter id to delete: ");

                if(storage.deleteUserById(id)) {
                    std::cout << "User deleted." << std::endl;
                } else {
                    std::cout << "User not found." << std::endl;
                }
                break;
            }

            default: {
                std::cout << "Invalid option.\n";
                break;
            }
        }
    }

    return 0;
}    
