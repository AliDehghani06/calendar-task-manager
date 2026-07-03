#ifndef DELETECOMMANDS_HPP
#define DELETECOMMANDS_HPP

#include "Consts.hpp"
#include "Utilities.hpp"

#include <string>
#include <vector>
#include <memory>
#include <stdexcept>

using namespace std;

class AppSystem;
class User;

class Delete
{
public:
    static Delete &getInstance();
    Delete(const Delete &) = delete;
    void operator=(const Delete &) = delete;
    bool handleDELETE(const vector<string> &words);

private:
    Delete();
    AppSystem &system;
    shared_ptr<User> user;

    void parameterValues(const vector<string> &words);
    bool handleDeleteTask();

    string id;
};

#endif