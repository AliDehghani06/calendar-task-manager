#include "HolidayManager.hpp"

HolidayManager& HolidayManager::getInstance(){
    static HolidayManager instance;
    return instance;
}

void HolidayManager::findBestPlace(std::shared_ptr<Date> new_holiday) {
    if (holidays_.empty()) {
        holidays_.push_back(new_holiday);
        return;
    }
    auto it = holidays_.begin();
    while (it != holidays_.end()) {
        string comparison = new_holiday->compare(**it);
        if (comparison == EQUAL) {
        }
        else if (comparison == EARLIER) {
            holidays_.insert(it, new_holiday);
            return;
        }
        ++it;
    }
    holidays_.push_back(new_holiday);
}

bool HolidayManager::readFromFile(ifstream& Holidays){
    string line;
    getline(Holidays, line);
    line.clear();
    while(getline(Holidays, line)){
        stringstream iss(line);
        vector<string> datas = Separator(line, ',');
        findBestPlace(make_shared<Date>(stoi(datas[0]), stoi(datas[1]), stoi(datas[2])));
    }
    return true;
}

bool HolidayManager::openFile(const string& file_name) {
    ifstream Holidays(file_name);
    if (!Holidays) {
        throw runtime_error("Failed to open file: " + file_name);
    }
    return readFromFile(Holidays);
}

bool HolidayManager::checkDate(const string& date, shared_ptr<Date> holiday){
    vector<string> date_parts = Separator(date, '/');
    if(holiday->checkYear(stoi(date_parts[0])))
        if(holiday->checkMonth(stoi(date_parts[1])))
            if(holiday->checkDay(stoi(date_parts[2])))
                return true;
    return false;
}

bool HolidayManager::checkHoliday(const string& date){
    for(const auto& holiday : holidays_){
        if(checkDate(date, holiday))
            return true;
    }
    return false;
}