#ifndef GETCOMMANDS_HPP
#define GETCOMMANDS_HPP

#include "Consts.hpp"
#include "Utilities.hpp"

#include <string>
#include <vector>
#include <memory>

using namespace std;

class AppSystem;
class User;

class Get
{
public:
    static Get &getInstance();
    Get(const Get &) = delete;
    void operator=(const Get &) = delete;
    bool handleGET(const vector<string> &words);

private:
    Get();
    AppSystem &system;
    shared_ptr<User> user;

    bool handleReport();
    void parameterValues(const vector<string> &words);

    string from;
    string to;
    string type;
};

#endif