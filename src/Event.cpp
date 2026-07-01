#include "Event.hpp"

Event::Event(int id, const string &type, const string &title, int start_time, int duration, const string &description)
    : id_(id), type_(type), title_(title), start_time_(start_time), duration_(duration), description_(description) {}

bool Event::checkOverlap(int start_time, int duration) const
{
    int this_end = start_time_ + duration_;
    int other_end = start_time + duration;
    return (start_time_ < other_end) && (this_end > start_time);
}

void Event::print(const string &date, string &report)
{
    string report_event;
    if (type_ == EVENT)
    {
        report_event = EVENT + ' ' + '"' + title_ + '"' + " on " + date + " from " + to_string(start_time_) + " for " + to_string(duration_) + " hours: ";
        cout << report_event;
        if (description_ != NOT_FOUND)
        {
            report_event = report_event + '"' + description_ + '"' + "\n";
            cout << '"' << description_ << '"' << endl;
        }
        else{
            report_event = report_event + "\n";
        cout << endl;
        }
        
    }
    else
    {
        report_event = PERIODIC_EVENT + ' ' + '"' + title_ + '"' + " on " + date + " from " + to_string(start_time_) + " for " + to_string(duration_) + " hours " + type_ + ": ";
        cout << report_event;
        if (description_ != NOT_FOUND)
        {
            report_event = report_event + '"' + description_ + '"' + "\n";
            cout << '"' << description_ << '"' << endl;
        }
        else{
            report_event = report_event + "\n";
        cout << endl;
        }
    }
    report = report + report_event;
}

string Event::compareTime(int time)
{
    if (start_time_ < time)
    {
        return EARLIER;
    }
    return LATER;
}

bool Event::checkId(int id)
{
    return id_ == id;
}