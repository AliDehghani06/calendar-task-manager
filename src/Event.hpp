#ifndef EVENT_HPP
#define EVENT_HPP

#include "Consts.hpp"

#include <string>
#include <iostream>

using namespace std;

class Event
{
public:
    Event(int id, const string &type, const string &title, int start_time, int duration, const string &description);
    virtual ~Event() = default;
    bool checkOverlap(int start_time, int duration) const;
    virtual void print(const string& date, string &report);
    int get_start_time() { return start_time_; }
    string get_type() { return type_; }
    int get_duration() { return duration_; }
    string compareTime(int time);
    bool checkId(int id);

protected:
    int id_;
    string type_;
    string title_;      
    int start_time_;
    int duration_;
    string description_;
};

#endif