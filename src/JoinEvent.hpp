#ifndef JOINEVENT_HPP
#define JOINEVENT_HPP

#include "Event.hpp"

#include <string>
#include <vector>

using namespace std;

class JoinEvent : public Event{
public:
    JoinEvent(int id, const string& host, const string& date, const string& type, const string& title ,int start_time ,int duration 
        ,const string& description, int end_time, const vector<string>& guests);
    void printInvitation();
    void print(const string& date, string &report) override;
    void set_host_status() { added_to_host = true; }
    string get_date() { return date_; }
    string get_host() { return host_; }
    bool get_host_status() { return added_to_host; }
    void handlejson(ostream &json);
private:
    string host_;
    string date_;
    int end_time_;
    vector<string> guests_;
    bool added_to_host = false;
};



#endif