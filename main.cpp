#include <iostream>
#include <string>
#include <limits>
#include <vector>

#include "StorageEngine.hpp"
#include "StorageErrors.hpp"

int readInt(const std::string& prompt) {
    int value;

    while (true) {
        std::cout << prompt;
        std::cin >> value;

        if (std::cin) {
            return value;
        }

        std::cout << "Invalid input. Please enter a number.\n";

        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
}

int main() {
    StorageEngine storage("users.db");

    while (true) {
        std::cout << "\nMiniDB Menu:\n"
                  << "1. Insert user\n"
                  << "2. List all users\n"
                  << "3. Find user by id\n"
                  << "4. Clear database\n"
                  << "5. Delete user by id\n"
                  << "6. Update user by id\n"
                  << "0. Exit\n";

        int choice = readInt("Choose option: ");

        try {
            switch (choice) {
                case 0: {
                    std::cout << "Goodbye!\n";
                    return 0;
                }

                case 1: {
                    int id = readInt("Enter id: ");

                    // Enforce ID uniqueness, similar to a primary key.
                    User existingUser;

                    if (storage.findUserById(id, existingUser)) {
                        std::cout << "Error: user with ID " << id
                                  << " alredy exists.\n";
                        break;
                    }

                    std::string name;
                    std::cout << "Enter name: ";
                    std::cin >> name;

                    int age = readInt("Enter age: ");

                    User user = createUser(id, name, age);

                    storage.saveUser(user);
                    std::cout << "User inserted successfully.\n";

                    break;
                }

                case 2: {
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

                case 3: {
                    int id = readInt("Enter id to search: ");

                    User foundUser;

                    if (storage.findUserById(id, foundUser)) {
                        std::cout << "User found:\n";
                        printUser(foundUser);
                    } else {
                        std::cout << "User not found.\n";
                    }

                    break;
                }

                case 4: {
                    storage.clear();
                    std::cout << "Database is clear.\n";
                    break;
                }

                case 5: {
                    int id = readInt("Enter id to delete: ");

                    if (storage.deleteUserById(id)) {
                        std::cout << "User delete successfully.\n";
                    } else {
                        std::cout << "User not found.\n";
                    }

                    break;
                }

                case 6: {
                    int id = readInt("Enter user id to update: ");

                    User existingUser;

                    if (!storage.findUserById(id, existingUser)) {
                        std::cout << "User not found.\n";
                        break;
                    }

                    std::string newName;
                    std::cout << "Enter new name: ";
                    std::cin >> newName;

                    int newAge = readInt("Enter new age: ");

                    User updatedUser = createUser(id, newName, newAge);

                    storage.updateUserById(id, updatedUser);
                    std::cout << "User updated successfully.\n";

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