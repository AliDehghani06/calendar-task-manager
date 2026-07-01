#include "Utilities.hpp"

vector<string> Separator(const string &line, char separator, char enclosure) {
    vector<string> result;
    string token;
    bool inside_enclosure = false;

    for (size_t i = 0; i < line.length(); ++i) {
        char c = line[i];

        if (c == enclosure) {
            inside_enclosure = !inside_enclosure;
        }
        else if (c == separator && !inside_enclosure) {
            if (!token.empty()) {
                result.push_back(token);
                token.clear();
            }
        }
        else {
            token += c;
        }
    }

    if (!token.empty()) {
        result.push_back(token);
    }

    return result;
}

string RemoveQuotation(const string &word)
{
    if (word.size() >= 2 && word.front() == '"' && word.back() == '"')
    {
        return word.substr(1, word.length() - 2);
    }
    return word;
}

string DesiredWord(const vector<string>& words, const string& wanted) {
    if (words.size() < 2) return NOT_FOUND;

    for (size_t i = 0; i < words.size() - 1; i++) {
        if (words[i] == wanted) {
            if (i + 1 < words.size()) {
                string next_word = words[i+1];
                return RemoveQuotation(next_word);
            }
            return EMPTY;
        }
    }
    return NOT_FOUND;
}

int findDistance(const string &start_date, const string &end_date)
{
    int start_days = dateToDays(start_date);
    int end_days = dateToDays(end_date);
    return abs(start_days - end_days);
}

int dateToDays(const string &date)
{
    vector<string> date_parts = Separator(date, '/');
    return stoi(date_parts[0]) * 360 +
           stoi(date_parts[1]) * 30 +
           stoi(date_parts[2]);
}

string nextDay(const string &date)
{
    std::vector<std::string> date_parts = Separator(date, '/');
    int year = std::stoi(date_parts[0]);
    int month = std::stoi(date_parts[1]);
    int day = std::stoi(date_parts[2]);
    day++;
    if (day > 30)
    {
        day = 1;
        month++;
        if (month > 12)
        {
            month = 1;
            year++;
        }
    }
    day = std::max(1, std::min(day, 31));
    month = std::max(1, std::min(month, 12));

    char buffer[11];
    snprintf(buffer, sizeof(buffer), "%04d/%02d/%02d", year, month, day);
    return string(buffer);
}

bool checkWeekDays(const vector<string> &week_days)
{
    for (const auto &day : week_days)
    {
        if (day != SATURDAY && day != SUNDAY && day != MONDAY &&
             day != TUESDAY && day != WEDNESDAY && day != THURSDAY && day != FRIDAY)
        {
            return true;
        }
    }
    return false;
}

bool validateDate(const string& date) {
    regex date_pattern(
        "^"
        "(14[0-9]{2}|1[5-9][0-9]{2}|[2-9][0-9]{3})"
        "/"
        "(0[1-9]|1[0-2])"
        "/"
        "(0[1-9]|[12][0-9]|30)"
        "$"
    );
    return regex_match(date, date_pattern);
}

bool isValidStartTime(const string& time) {
    regex time_pattern("^([0-9]|0[0-9]|1[0-9]|2[0-3])$");
    return regex_match(time, time_pattern);
}

bool isValidDuration(const string& dur) {
    regex dur_pattern("^[1-9][0-9]*$");
    return regex_match(dur, dur_pattern);
}

bool isValidDay(const string& day) {
    regex day_pattern("^([1-9]|0[1-9]|[12][0-9]|30)$");
    if (!regex_match(day, day_pattern)) {
        return false;
    }
    int day_num = stoi(day);
    return (day_num >= 1 && day_num <= 30);
}

string dateToString(int year_, int month_, int day_){
    string month, day;
    if (month_ < 10)
    {
        month = '0' + to_string(month_);
    }
    else
        month = to_string(month_);
    if (day_ < 10)
    {
        day = '0' + to_string(day_);
    }
    else
        day = to_string(day_);
    return to_string(year_) + '/' + month + '/' + day;
}

bool isPositiveInteger(const std::string& str) {
    std::regex integer_pattern("^\\d+$");
    return std::regex_match(str, integer_pattern);
}

string urlencode(const std::string &value) {
    ostringstream escaped;
    escaped.fill('0');
    escaped << std::hex;

    for (char c : value) {
        if (isalnum(static_cast<unsigned char>(c)) || c == '-' || c == '_' || c == '.' || c == '~') {
            escaped << c;
        } else if (c == ' ') {
            escaped << '+';
        } else {
            escaped << '%' << setw(2) << uppercase << int(static_cast<unsigned char>(c));
        }
    }

    return escaped.str();
}

string formatDate(int year, int month, int day) {
    ostringstream oss;
    oss << year << '/';
    oss << setw(2) << setfill('0') << month << '/';
    oss << setw(2) << setfill('0') << day;
    return oss.str();
}