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

#endif