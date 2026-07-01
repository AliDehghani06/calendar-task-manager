#ifndef HOLIDAYMANAGER_HPP
#define HOLIDAYMANAGER_HPP

#include "Utilities.hpp"
#include "Date.hpp"

#include <string>
#include <vector>
#include <memory>
#include <fstream>
#include <istream>
#include <sstream>
#include <iostream>

using namespace std;

class HolidayManager{
public:
    static HolidayManager& getInstance(); 
    bool openFile(const string& file_name);
    bool readFromFile(ifstream& Holidays);
    void findBestPlace(shared_ptr<Date> new_holiday);
    bool checkDate(const string& date, shared_ptr<Date> holiday);
    bool checkHoliday(const string& date);
    HolidayManager(const HolidayManager&) = delete;
    void operator=(const HolidayManager&) = delete;
private:
    HolidayManager() = default;
    vector<shared_ptr<Date>> holidays_;
};

#endif