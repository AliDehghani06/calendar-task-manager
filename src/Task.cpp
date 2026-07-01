#include "Task.hpp"

Task::Task(int id, const string &title, int time, const string &description)
    : id_(id), title_(title), time_(time), description_(description) {}

bool Task::checkId(int id)
{
    return id_ == id;
}

void Task::edit(const string &title, const string &time, const string &description)
{
    if (title != NOT_FOUND)
        title_ = title;
    if (time != NOT_FOUND)
        time_ = stoi(time);
    if (description != NOT_FOUND)
        description_ = description;
}

void Task::print(const string &date, string &report)
{
    string report_task = TASK + ' ' + '"' + title_ + '"' + " on " + date + " at " + to_string(time_) + ": ";
    cout << report_task;
    if (description_ != NOT_FOUND)
    {
        report_task = report_task + '"' + description_ + '"' + "\n";
        cout << '"' << description_ << '"' << endl;
    }
    else{
        report_task = report_task + "\n";
        cout << endl;
    }
     report = report + report_task;   
}

string Task::compareTime(int time){
    if(time_ < time){
        return EARLIER;
    }
    else if(time_ == time){
        return EQUAL;
    }
    return LATER;
}