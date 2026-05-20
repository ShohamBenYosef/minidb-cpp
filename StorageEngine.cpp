#include <iostream>
#include <cstring>
#include <fstream>
#include <cstdio>
#include <vector>
#include <cstddef>

#include "StorageEngine.hpp"
#include "StorageErrors.hpp"


StorageEngine::StorageEngine(const std::string& filename)
    : filename(filename) {
}

bool StorageEngine::saveUser(const User& user) {
    std::ofstream file(filename, std::ios::binary | std::ios::app);

    if (!file.is_open())
        throw StorageException(StorageErrorCode::FileOpenFailed, std::string(OPEN_DB_FILE) + filename);

    file.write(reinterpret_cast<const char*>(&user), sizeof(User));

    if (!file)
        throw StorageException(StorageErrorCode::FileWriteFailed, std::string(WRITE_DB_FILE) + filename);
    

    return true;
}

bool StorageEngine::updateUserById(int id, const User& updatedUser) {
    std::fstream file(filename, std::ios::binary | std::ios::in | std::ios::out);

    if (!file.is_open()) {
        throw StorageException(StorageErrorCode::FileOpenFailed, std::string(OPEN_DB_FILE) + filename);
    }

    User user;

    while (file.read(reinterpret_cast<char*>(&user), sizeof(User))) {
        if (user.id == id) {
            // Move the write cursor back to the beginning of this record.
            file.seekp(-static_cast<std::streamoff>(sizeof(User)), std::ios::cur);

            if (!file) {
                throw StorageException(StorageErrorCode::FileSeekFailed, std::string(SEEK_DB_FILE) + filename);
            }

            file.write(reinterpret_cast<const char*>(&updatedUser), sizeof(User));

            if (!file) {
                throw StorageException(StorageErrorCode::FileWriteFailed, std::string(UPDATE_DB_FILE) + filename);
            }

            return true;
        }
    }

    if (!file.eof()) {
        throw StorageException(StorageErrorCode::FileReadFailed, std::string(READ_DB_FILE) + filename);
    }

    // User not found is not a storage error.
    return false;
}

bool StorageEngine::deleteUserById(int id) {
    std::ifstream file(filename, std::ios::binary);

    if (!file.is_open()) {
        throw StorageException(StorageErrorCode::FileOpenFailed, std::string(OPEN_DB_FILE) + filename);
    }

    const std::string tmpFilename = filename + ".tmp";
    std::ofstream tmp(tmpFilename, std::ios::binary);

    if (!tmp.is_open()) {
        throw StorageException(StorageErrorCode::TempFileOpenFailed, std::string(OPEN_TEMP_FILE) + tmpFilename);
    }

    User user;
    bool found = false;

    while (file.read(reinterpret_cast<char*>(&user), sizeof(User))) {
        if (user.id == id) {
            found = true;
            continue;
        }

        tmp.write(reinterpret_cast<const char*>(&user), sizeof(User));

        if (!tmp) {
            file.close();
            tmp.close();
            std::remove(tmpFilename.c_str());

            throw StorageException(StorageErrorCode::FileWriteFailed, std::string(WRITE_DB_FILE) + tmpFilename);
        }
    }

    if (!file.eof()) {
        file.close();
        tmp.close();
        std::remove(tmpFilename.c_str());

        throw StorageException(StorageErrorCode::FileReadFailed, std::string(READ_DB_FILE) + filename);
    }

    file.close();
    tmp.close();

    // User not found is not a storage error.
    // Remove the temporary file and return false.
    if (!found) {
        std::remove(tmpFilename.c_str());
        return false;
    }

    if (std::remove(filename.c_str()) != 0) {
        std::remove(tmpFilename.c_str());

        throw StorageException(StorageErrorCode::FileRemoveFailed, std::string(REMOVE_DB_FILE) + filename);
    }

    if (std::rename(tmpFilename.c_str(), filename.c_str()) != 0) {
        throw StorageException(StorageErrorCode::FileRenameFailed, std::string(RENAME_DB_FILE) + tmpFilename + " -> " + filename);
    }

    return true;
}

std::vector<User> StorageEngine::loadAllUsers() const {
    std::ifstream file(filename, std::ios::binary);

    if (!file.is_open()) {
        throw StorageException(StorageErrorCode::FileOpenFailed, std::string(OPEN_DB_FILE) + filename);
    }

    std::vector<User> users;
    User user;

    while (file.read(reinterpret_cast<char*>(&user), sizeof(User))) {
        users.push_back(user);
    }

    if (!file.eof()) {
        throw StorageException(StorageErrorCode::FileReadFailed, std::string(READ_DB_FILE) + filename);
    }

    return users;
}

bool StorageEngine::findUserById(int id, User& res) const {
    std::ifstream file(filename, std::ios::binary);

    if (!file.is_open()) {
        throw StorageException(StorageErrorCode::FileOpenFailed, std::string(OPEN_DB_FILE) + filename);
    }

    User currUser;

    while (file.read(reinterpret_cast<char*>(&currUser), sizeof(User))) {
        if (currUser.id == id) {
            res = currUser;
            return true;
        }
    }

    if (!file.eof()) {
        throw StorageException(StorageErrorCode::FileReadFailed, std::string(READ_DB_FILE) + filename);
    }

    // User not found is not an exception.
    return false;
}

bool StorageEngine::clear() const {
    std::ofstream file(filename, std::ios::binary | std::ios::trunc);

    if (!file.is_open()) {
        throw StorageException(StorageErrorCode::FileOpenFailed, std::string(OPEN_DB_FILE) + filename);
    }

    return true;
}

User createUser(int id, const std::string& name, int age) {
    User newUser;
    newUser.id = id;
    newUser.age = age;

    std::strncpy(newUser.name, name.c_str(), sizeof(newUser.name) - 1);
    newUser.name[sizeof(newUser.name) - 1] = '\0';

    return newUser;
}

void printUser(const User& user) {
    std::cout << "ID: " << user.id
              << ", Name: " << user.name
              << ", Age: " << user.age
              << std::endl;
}

std::size_t StorageEngine::countUsers() const{
    // return loadAllUsers().size(); // read all record to the memory..

    std::ifstream file (filename, std::ios::binary | std::ios::ate);
    std::streamsize fileSize = file.tellg();

    if (fileSize % sizeof(User) != 0) {
        throw StorageException(StorageErrorCode::FileReadFailed, std::string(READ_DB_FILE) + filename);
    }
    return (fileSize / sizeof(User));
}

std::size_t StorageEngine::recordSize() const {
    return sizeof(User);
}

std::string StorageEngine::getFilename() const {
    return filename;
}