#ifndef UTILITIES_HPP   
#define UTILITIES_HPP

#include "Consts.hpp"

#include <sstream>
#include <iomanip>
#include <cctype>
#include <regex>
#include <string>
#include <vector>
#include <iostream>
#include <unordered_set>

using namespace std;

vector<string> Separator(const string &line, char separator, char enclosure = '"');
string RemoveQuotation(const string& word);
string DesiredWord(const vector<string>& words, const string& wanted);
int findDistance(const string &start_date, const string &end_date);
int dateToDays(const string &date);
string nextDay(const string &date);
bool checkWeekDays(const vector<string> &week_days);
bool validateDate(const string& date);
bool isValidStartTime(const string& time);
bool isValidDuration(const string& dur);
bool isValidDay(const string& day);
string dateToString(int year_, int month_, int day_);
bool isPositiveInteger(const std::string& str);
string urlencode(const std::string &value);
string formatDate(int year, int month, int day);

#endif