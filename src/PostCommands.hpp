#ifndef POSTCOMMANDS_HPP
#define POSTCOMMANDS_HPP

#include "Consts.hpp"
#include "Utilities.hpp"

#include <string>
#include <vector>
#include <memory>
#include <iostream>
#include <stdexcept>

using namespace std;

class AppSystem;
class HolidayManager;
class User;

class Post
{
public:
    static Post &getInstance();
    Post(const Post &) = delete;
    void operator=(const Post &) = delete;
    bool handlePOST(const vector<string> &words);
    bool checkExceptions(string type);
    bool checkDaily();
    bool checkWeekly(const vector<string> &week_days);
    bool checkMonthly(const string &day);

private:
    Post();

    AppSystem &system;
    HolidayManager &holiday;
    shared_ptr<User> user;

    bool handleSignUp();
    bool handleLogIn();
    bool handleLogOut();
    bool handleEvent();
    bool handleDailyEvent();
    bool handleWeeklyEvent(const vector<string> &words);
    bool handleMonthlyEvent(const vector<string> &words);
    bool handleAddTask(const vector<string> &words);
    bool handleJoinEvent(const vector<string> &words);
    void parameterValues(const vector<string> &words);
    bool handleInvitation(const vector<string> &words, const string& command);

    string id;
    string username, password;
    string date;
    string start_time, duration;
    string title, description;
    string start_date, end_date;
    string type;
    bool user_log_in;
};

#endif