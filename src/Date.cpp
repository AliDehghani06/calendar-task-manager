#include "Date.hpp"

#include <iostream>

Date::Date(const int &day, const int &month, const int &year)
    : day_(day), month_(month), year_(year) {}

string Date::compare(const Date &other) const
{
    if (year_ != other.year_)
    {
        
        return (year_ < other.year_) ? EARLIER : LATER;
    }
    if (month_ != other.month_)
    {
        
        return (month_ < other.month_) ? EARLIER : LATER;
    }
    if (day_ != other.day_)
    {
        return (day_ < other.day_) ? EARLIER : LATER;
    }
    return EQUAL;
}

bool Date::checkYear(int year){
    return year == year_;
}
bool Date::checkMonth(int month){
    return month == month_;
}
bool Date::checkDay(int day){
    return day == day_;
}