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
