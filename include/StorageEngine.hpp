#ifndef STORAGE_ENGINE_HPP
#define STORAGE_ENGINE_HPP


#include <cstddef>
#include <ios>
#include <string>
#include <unordered_map>
#include <vector>



inline constexpr std::size_t USER_NAME_SIZE = 32;

struct User {
    int id;
    char name[USER_NAME_SIZE];
    int age;
};


class StorageEngine {
private:
    std::string filename;
    std::unordered_map<int, std::streampos> index;
    void buildIndex();

public:
    explicit StorageEngine(const std::string& filename);
    bool saveUser(const User& user);
    bool updateUserById(int id, const User& updatedUser);
    bool deleteUserById(int id);
    bool saveUsersBulk(const std::vector<User>& users);
    std::vector<User> loadAllUsers() const;
    bool findUserByIdIndexed(int id, User& res) const;
    bool findUserByIdLinear(int id, User& res) const;
    bool clear();
    
    std::size_t indexSize() const;
    std::size_t countUsers() const;
    std::size_t recordSize() const;
    std::string getFilename() const;    
};

User createUser(int id, const std::string& name, int age);



#endif