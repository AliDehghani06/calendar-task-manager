#ifndef USERMANAGER_HPP
#define USERMANAGER_HPP

#include "User.hpp"
#include "Consts.hpp"
#include "JoinEvent.hpp"

#include <vector>
#include <memory>
#include <stdexcept>

using namespace std;

class User;

class UserManager
{
public:
    static UserManager &getInstance();
    shared_ptr<User> findUser(const std::string &username);
    bool signUpUser(const string &username, const string &password);
    UserManager(const UserManager &) = delete;
    UserManager &operator=(const UserManager &) = delete;
    bool checkGuests(const vector<string> &guests);
    void sendToGuests(const vector<string> &guests, shared_ptr<JoinEvent> new_one);
    void joinHost(shared_ptr<JoinEvent> event);

private:
    UserManager() = default;
    vector<shared_ptr<User>> users_;
};

#endif