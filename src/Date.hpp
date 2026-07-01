#ifndef DATE_HPP
#define DATE_HPP

#include "Consts.hpp"

#include <string>

using namespace std;

class Date{
public:
    Date(const int& day, const int& month, const int& year);
    string compare(const Date& other) const;
    bool checkYear(int year);
    bool checkMonth(int month);
    bool checkDay(int day);
protected:
    int day_;
    int month_;
    int year_;
};

#endif