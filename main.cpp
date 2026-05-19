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


int main(){
    
    clearFile("users.db");

    User user1 = createUser(1, "Shoham", 32);
    User user2 = createUser(2, "Dana", 24);
    User user3 = createUser(3, "Noam", 29);

    saveUserToFile(user1, "users.db");
    saveUserToFile(user2, "users.db");
    saveUserToFile(user3, "users.db");

    std::cout << "\nAll users in file:\n";
    loadAllUsersFromFile("users.db");

    std::cout << "\nSearching for user with id 2:\n";

    User FoundUser;

    if (findUserById("users.db", 2, FoundUser)){
        std::cout <<"User found:\n";
        printUser(FoundUser);
    } else {
        std::cout << "User not found" << std::endl;
    }

    return 0;
}