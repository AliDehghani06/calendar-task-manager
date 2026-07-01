#include "UserManager.hpp"

UserManager &UserManager::getInstance()
{
    static UserManager manager;
    return manager;
}

bool UserManager::signUpUser(const string &username, const string &password)
{
    if (findUser(username) != nullptr)
        throw invalid_argument(BAD_REQUEST);
    users_.emplace_back(make_shared<User>(username, password, users_.size() + 1));
    return true;
}

shared_ptr<User> UserManager::findUser(const std::string &username)
{
    for (auto &user : users_)
    {
        if (user->checkName(username))
            return user;
    }
    return nullptr;
}

bool UserManager::checkGuests(const vector<string> &guests)
{
    bool exist = false;
    for (const auto &guest : guests)
    {
        for (const auto &user : users_)
        {
            if (user->checkName(guest, JOIN_EVENT))
            {
                exist = true;
                break;
            }
        }
        if (!exist)
            return false;
        exist = false;
    }
    return true;
}

void UserManager::sendToGuests(const vector<string> &guests, shared_ptr<JoinEvent> new_one){
    for(const auto& guest : guests){
        for(const auto &user : users_){
            if(user->checkName(guest, JOIN_EVENT)){
                user->addInvitation(new_one);
            }
        }
    }
}

void UserManager::joinHost(shared_ptr<JoinEvent> event){
    for(const auto& user : users_){
        if(user->checkName(event->get_host())){
            user->addEvent(event->get_date(), event);
        }
    }
}