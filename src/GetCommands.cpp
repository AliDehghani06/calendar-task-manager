#include "GetCommands.hpp"
#include "User.hpp"
#include "System.hpp"

Get &Get::getInstance()
{
    static Get instance;
    return instance;
}

Get::Get()
    : system(AppSystem::getInstance()),
      user(system.getUser()){}

bool Get::handleReport()
{
    if (to == NOT_FOUND || !validateDate(to))
        throw invalid_argument(BAD_REQUEST);
    else if (from != NOT_FOUND && !validateDate(from))
        throw invalid_argument(BAD_REQUEST);
    else if(type != NOT_FOUND && type != EVENT && type != PERIODIC_EVENT && type != TASK && type != JOIN_EVENT)
        throw invalid_argument(BAD_REQUEST);
    else if (!system.checkLogin())
        throw invalid_argument(PERMISSION_DENIED);
    else
    {
        user = system.getUser();
        return true;
    }
    return false;
}

void Get::parameterValues(const vector<string> &words)
{
    if (words[2] != "?")
        throw invalid_argument(BAD_REQUEST);
    from = DesiredWord(words, FROM);
    to = DesiredWord(words, TO);
    type = DesiredWord(words, TYPE);
}

bool Get::handleGET(const vector<string> &words)
{
    user = system.getUser();
    parameterValues(words);
    if (words[1] == REPORT)
    {
        return handleReport();
    }
    else if(words[1] == JOIN_EVENT){
        if(!system.checkLogin())
            throw invalid_argument(PERMISSION_DENIED);
    }
    else
    {
        throw invalid_argument(NOT_FOUND);
    }
    return false;
}
