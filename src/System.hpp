#ifndef SYSTEM_HPP
#define SYSTEM_HPP

#include "User.hpp"
#include "PostCommands.hpp"
#include "PutCommands.hpp"
#include "GetCommands.hpp"
#include "DeleteCommands.hpp"
#include "HolidayManager.hpp"

#include <map>
#include <string>
#include <iostream>

using namespace std;

class User;

class AppSystem{
public:
    static AppSystem& getInstance();
    void initialize(const char * file_name);
    void command(string command);
    bool checkLogin();
    void setUser(shared_ptr<User> user);
    void logOutUser();
    shared_ptr<User> getUser();
    int dayCode(const string& day);
    int get_num_of_join_events() { return num_of_join_events; }
    void increase_num_of_join_events();
private:
    AppSystem() = default;
    AppSystem(const AppSystem&) = delete;
    AppSystem& operator=(const AppSystem&) = delete;
    int num_of_join_events = 0;
    vector<string> words;
    bool loged_in_ = false;
    shared_ptr<User> loged_in_user_ = nullptr;
    map<string, int> day_code_ = {{SATURDAY, 1}, {SUNDAY, 2},
                             {MONDAY, 3}, {TUESDAY, 4},
                             {WEDNESDAY, 5}, {THURSDAY, 6}, {FRIDAY, 0}};
};

#endif