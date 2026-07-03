#include "System.hpp"

AppSystem &AppSystem::getInstance()
{
    static AppSystem Instance;
    return Instance;
}

void AppSystem::initialize(const char* file_name) {
    HolidayManager::getInstance().openFile(file_name);
}

shared_ptr<User> AppSystem::getUser() { 
        return loged_in_user_; }

void AppSystem::command(string command)
{
    words = Separator(command, ' ');
        if (words.empty())
    {
        throw invalid_argument(BAD_REQUEST);
    }
    else if (words[0] == GET)
    {
        if(Get::getInstance().handleGET(words)) cout << OK << endl;
    }
    else if (words[0] == PUT)
    {
        if(Put::getInstance().handlePUT(words)) cout << OK << endl;
    }
    else if (words[0] == POST)
    {
        if(Post::getInstance().handlePOST(words)) cout << OK << endl;
    }
    else if (words[0] == DELETE)
    {
        if(Delete::getInstance().handleDELETE(words)) cout << OK << endl;
    }
    else
    {
        throw invalid_argument(BAD_REQUEST);
    }
}

bool AppSystem::checkLogin(){
    return loged_in_;
}

void AppSystem::setUser(shared_ptr<User> user){
    loged_in_user_ = user;
    loged_in_ = true;
}

void AppSystem::logOutUser(){
    loged_in_user_ = nullptr;
    loged_in_ = false;
}

int AppSystem::dayCode(const string& day){
    return day_code_.find(day)->second;
}

void AppSystem::increase_num_of_join_events(){
    num_of_join_events++;
}