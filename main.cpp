#include <iostream>
#include <cstring>
#include <fstream>
#include <string>

struct User {
    int id;
    char name[32];
    int age;
};

void printUser(const User& user) {
    std::cout << "ID: " << user.id
              << ", Name: " << user.name
              << ", Age: " << user.age
              << std::endl;
}

bool saveUserToFile(const User& user, const std::string& filename){
    std::ofstream file(filename, std::ios::binary | std::ios::app);
    
    if (!file.is_open()) {
        std::cerr << "Error: failed to open file: " << filename << std::endl;
        return false;
    }
    
    file.write(reinterpret_cast<const char*>(&user), sizeof(User));

    if (!file) {
        std::cerr << "Error: failed to write user to file." << std::endl;
        return false;
    }

    return true;
}

bool loadAllUsersFromFile(const std::string& filename) {
    std::ifstream file(filename, std::ios::binary);

    if (!file.is_open()) {
        std::cerr << "Error: failed to open file: " << filename << std::endl;
        return false;
    }

    User user;

    while (file.read(reinterpret_cast<char*>(&user), sizeof(User))){
        printUser(user);
    }

    if (!file.eof()) {
        std::cerr << "Error: failed while reading from file." << std::endl;
        return false;
    }

    return true;
}

User createUser(int id, const std::string& name, int age){
    User newUser;
    newUser.id = id;
    newUser.age = age;

    strncpy(newUser.name, name.c_str(), sizeof(newUser.name) - 1);
    newUser.name[sizeof(newUser.name) - 1] = '\0'; // make sure its end.

    return newUser;
}

bool clearFile(const std::string& filename) {
    std::ofstream file(filename, std::ios::binary | std::ios::trunc);

    if (!file.is_open()) {
        std::cerr << "Error: failed to clear file: " << filename << std::endl;
        return false;
    }

    return true;
}

bool findUserById(const std::string& filename, int id, User& res) {
    std::ifstream file(filename, std::ios::binary);

    if(!file.is_open()) {
        std::cerr << "Error: failed to open file: " << filename << std::endl;
        return false;
    }

    User currUser;

    while (file.read(reinterpret_cast<char*>(&currUser), sizeof(User))){
        if (currUser.id == id){
            res = currUser;
            return true;
        }
    }

    if (!file.eof()) {
        std::cerr << "Error: failed while reading from file." << std::endl;
    }

    return false;
}


int main() {
    const std::string filename = "users.db";

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

                if (saveUserToFile(user, filename)) {
                    std::cout << "User inserted successfully.\n";
                } else {
                    std::cout << "Failed to insert user.\n";
                }

                break;
            }

            case 2: {
                std::cout << "\nAll users:\n";
                loadAllUsersFromFile(filename);
                break;
            }

            case 3: {
                int id;
                User foundUser;

                std::cout << "Enter id to search: ";
                std::cin >> id;

                if (findUserById(filename, id, foundUser)) {
                    std::cout << "User found:\n";
                    printUser(foundUser);
                } else {
                    std::cout << "User not found.\n";
                }

                break;
            }

            case 4: {
                if (clearFile(filename)) {
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
