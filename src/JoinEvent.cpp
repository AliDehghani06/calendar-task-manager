#include "JoinEvent.hpp"

JoinEvent::JoinEvent(int id, const string &host, const string &date, const string &type, const string &title, int start_time, int duration, const string &description, int end_time, const vector<string> &guests)
    : Event(id, type, title, start_time, duration, description),
      host_(host), date_(date), end_time_(end_time), guests_(guests) {}

void JoinEvent::printInvitation()
{
    cout << id_ << ": \"" << title_ << "\" - " << date_ << " - " << start_time_ << " - " << end_time_ << endl;
}

void JoinEvent::print(const string &date, string &report)
{
    string report_event = JOIN_EVENT + ' ' + '"' + title_ + '"' + " on " + date + " from " + to_string(start_time_)
         + " to " + to_string(end_time_) + " hosted by \"" + host_ + "\" : ";
    cout << report_event;
    if (description_ != NOT_FOUND)
    {
        report_event = report_event + '"' + description_ + '"' + '\n';
        cout << '"' << description_ << '"' << endl;
    }
    else{
        report_event = report_event + '\n'; 
        cout << endl;
    }
    report = report + report_event;   
}

void JoinEvent::handlejson(ostream &json)
{
    json << "{"
         << "\"id\":\"" << to_string(id_) << "\","
         << "\"title\":\"" << title_ << "\","
         << "\"date\":\"" << date_ << "\","
         << "\"startHour\":\"" << to_string(start_time_) << "\","
         << "\"endHour\":\"" << to_string(end_time_) << "\""
         << "}";
}