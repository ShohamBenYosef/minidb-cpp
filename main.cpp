#include <iostream>
#include <string>
#include "StorageEngine.hpp"



int main() {
    StorageEngine storage("users.db");

    int choice = 0;

    while (true) {
        std::cout << "\nMiniDB Menu:\n"
                  << "1. Insert user\n"
                  << "2. List all users\n"
                  << "3. Find user by id\n"
                  << "4. Clear database\n"
                  << "0. Exit\n"
                  << "Choose option: ";

        std::cin >> choice;

        switch (choice) {
            case 0: {
                std::cout << "Goodbye!\n";
                return 0;
            }

            case 1: {
                int id;
                std::string name;
                int age;

                std::cout << "Enter id: ";
                std::cin >> id;

                std::cout << "Enter name: ";
                std::cin >> name;

                std::cout << "Enter age: ";
                std::cin >> age;

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
                int id;
                User foundUser;

                std::cout << "Enter id to search: ";
                std::cin >> id;

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

            default: {
                std::cout << "Invalid option.\n";
                break;
            }
        }
    }

    return 0;
}    
