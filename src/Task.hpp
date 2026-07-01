#ifndef TASK_HPP
#define TASK_HPP

#include "Consts.hpp"

#include <string>
#include <iostream>

using namespace std;

class Task
{
public:
    Task(int id, const string &title, int time, const string &description);
    bool checkId(int id);
    void edit(const string &title, const string &time, const string &description);
    void print(const string& date, string &report);
    string get_title() { return title_; }
    int get_time() {return time_; }
    string get_description() { return description_; }
    string compareTime(int time);
    int get_id() { return id_; }
private:
    int id_;
    string title_;
    int time_;
    string description_;
};

#endif