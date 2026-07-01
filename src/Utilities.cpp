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

