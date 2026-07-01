#ifndef USER_HPP
#define USER_HPP

#include "Day.hpp"
#include "System.hpp"
#include "UserManager.hpp"
#include "JoinEvent.hpp"
#include "Utilities.hpp"

#include <string>
#include <vector>
#include <memory>

using namespace std;

class User
{
public:
    User(const string &username, const string &password, int user_id);
    bool checkName(const string &username, string type = NORMAL);
    bool checkPassword(const string &password);
    bool checkDay(const string &date, shared_ptr<Day> day);
    bool checkOverlap(const string &date, int start_time, int duration);
    void addEvent(const string &date, shared_ptr<Event> new_event);
    void addToWaitingQueue(const string &date, const string &type, const vector<string> &guests, int start_time, int end_time, const string &title, const string &description);
    void addTask(int id, const string &date, int time, const string &title, const string &description);
    bool editTask(int id, string date, const string &title, const string &time, const string &description);
    bool deleteTask(int id);
    int numOfAllEvents();
    int numOfAllTasks();
    void getReport(string from, const string &to, const string &type, string &report);
    bool handleInvitations(ostream &json);
    void addInvitation(shared_ptr<JoinEvent> new_one);
    void confirmInvitation(int id);
    void rejectInvitation(int id);
    string get_username() { return username_; }
    int get_user_id() { return user_id_;}
private:
    int user_id_;
    string username_;
    string password_;
    vector<shared_ptr<Day>> days_;
    vector<shared_ptr<JoinEvent>> waiting_queue_;
    vector<shared_ptr<JoinEvent>> invitations_;
};

#endif