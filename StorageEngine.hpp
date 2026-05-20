#ifndef STORAGE_ENGINE_HPP
#define STORAGE_ENGINE_HPP

#include <string>
#include <vector>

struct User {
    int id;
    char name[32];
    int age;
};

class StorageEngine {
private:
    std::string filename;

public:
    explicit StorageEngine(const std::string& filename);

    bool saveUser(const User& user);
    bool updateUserById(int id, const User& updatedUser);
    bool deleteUserById(int id);
    std::vector<User> loadAllUsers() const;
    bool findUserById(int id, User& res) const;
    bool clear() const;

    std::size_t countUsers() const;
    std::size_t recordSize() const;
    std::string getFilename() const;    
};

User createUser(int id, const std::string& name, int age);
void printUser(const User& user);



#endif