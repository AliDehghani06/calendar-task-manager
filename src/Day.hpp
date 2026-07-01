#ifndef DAY_HPP
#define DAY_HPP

#include "Date.hpp"
#include "Event.hpp"
#include "Task.hpp"
#include "JoinEvent.hpp"
#include "Utilities.hpp"

#include <string>
#include <vector>
#include <memory>

using namespace std;

class Day : public Date{
public:
    Day(const int& day, const int& month, const int& year);
    vector<shared_ptr<Event>> get_events() { return events; }
    int  numOfEvents();
    int numOfTasks();
    void addEvent(shared_ptr<Event> new_event);
    void addTask(shared_ptr<Task> new_task);
    bool removeTask(int id, const string& type);
    bool editTask(int id, const string& title, const string& time, const string& description);
    bool dayReport(const string& type, string &report);
    shared_ptr<Task> findTask(int id);
private:
    void allWorkds(const string &date, bool & found, string &report);

    int num_of_deleted_tasks_ = 0;
    vector<shared_ptr<Event>> events;
    vector<shared_ptr<Task>> tasks;
};

#endif