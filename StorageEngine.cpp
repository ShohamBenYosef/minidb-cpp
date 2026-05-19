#include "StorageEngine.hpp"
#include <iostream>
#include <cstring>
#include <fstream>
#include <cstdio>


StorageEngine::StorageEngine(const std::string& filename)
    : filename(filename) {
}

bool StorageEngine::saveUser(const User& user){
    
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

bool StorageEngine::deleteUserById(int id){
    std::ifstream file(filename, std::ios::binary);

    if (!file.is_open()) {
        std::cerr << "Error: failed to open file: " << filename << std::endl;
        return false;
    }

    std::string tmpFilename = filename + ".tmp";
    std::ofstream tmp(tmpFilename, std::ios::binary);

    if (!tmp.is_open()) {
        std::cerr << "Error: failed to open temp file: " << tmpFilename << std::endl;
        return false;
    }

    User user;
    bool found = false;

    while (file.read(reinterpret_cast<char*>(&user), sizeof(User))) {
        if (user.id == id){
            found = true;
            continue;
        }
        tmp.write(reinterpret_cast<const char*>(&user), sizeof(User));

        if(!tmp) {
            std::cerr << "Error: faild whilewriting to temporary file." << std::endl;
            file.close();
            tmp.close();
            std::remove(tmpFilename.c_str());
            return false;
        }
    }

    if (!file.eof()) {
        std::cerr << "Error: failed while reading from file." << std::endl;
        file.close();
        tmp.close();
        std::remove(tmpFilename.c_str());
        return false;
    }

    file.close();
    tmp.close();

    if (!found) {
        std::remove(tmpFilename.c_str());
        return false;
    }

    if (std::remove(filename.c_str()) != 0) {
        std::cerr << "Error: faild to remove original file." << std::endl;
        std::remove(tmpFilename.c_str());
        return false;
    }

    if (std::rename(tmpFilename.c_str(), filename.c_str()) != 0) {
        std::cerr << "Error: faild to rename temp file." << std::endl;
        return false;
    }
    return true;
}

bool StorageEngine::loadAllUsers() const {
    std::ifstream file(filename, std::ios::binary);

    if (!file.is_open()) {
        std::cerr << "Error: failed to open file: " << filename << std::endl;
        return false;
    }

    User user;

    while (file.read(reinterpret_cast<char*>(&user), sizeof(User))) {
        printUser(user);
    }

    if (!file.eof()) {
        std::cerr << "Error: failed while reading from file." << std::endl;
        return false;
    }

    return true;
}

bool StorageEngine::findUserById(int id, User& res) const {
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

bool StorageEngine::clear() const {
    std::ofstream file(filename, std::ios::binary | std::ios::trunc);

    if (!file.is_open()) {
        std::cerr << "Error: failed to clear file: " << filename << std::endl;
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

void printUser(const User& user) {
    std::cout << "ID: " << user.id
              << ", Name: " << user.name
              << ", Age: " << user.age
              << std::endl;
}