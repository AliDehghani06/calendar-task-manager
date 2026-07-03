#ifndef PUTCOMMANDS_HPP
#define PUTCOMMANDS_HPP

#include "Consts.hpp"
#include "Utilities.hpp"

#include <string>
#include <vector>
#include <memory>
#include <stdexcept>

using namespace std;

class AppSystem;
class User;

class Put
{
public:

    Put(const Put &) = delete;
    void operator=(const Put &) = delete;
    static Put &getInstance();
    bool handlePUT(const vector<string> &words);

private:
    Put();
    AppSystem &system;
    shared_ptr<User> user;

    bool handleEditTask();
    void parameterValues(const vector<string> &words);

    string id;
    string date;
    string time;
    string title;
    string description;
};

#endif