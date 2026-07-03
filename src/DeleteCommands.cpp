#include "DeleteCommands.hpp"
#include "User.hpp"
#include "System.hpp"


Delete& Delete::getInstance(){
    static Delete instance;
    return instance;
}
Delete::Delete()
    : system(AppSystem::getInstance()),
      user(system.getUser()) {}



bool Delete::handleDeleteTask()
{
    if (id == NOT_FOUND || !isPositiveInteger(id))
        throw invalid_argument(BAD_REQUEST);
    else if (!system.checkLogin())
        throw invalid_argument(PERMISSION_DENIED);
    else
    {
        if (!user->deleteTask(stoi(id)))
            throw logic_error(NOT_FOUND);
        return true;
    }
}

void Delete::parameterValues(const vector<string> &words)
{
    if (words[2] != "?")
        throw invalid_argument(BAD_REQUEST);
    id = DesiredWord(words, TASK_ID);
}


bool Delete::handleDELETE(const vector<string>& words){
    parameterValues(words);
    if(words[1] == TASK){
        return handleDeleteTask();
    }
    else{
        throw invalid_argument(NOT_FOUND);
    }
    return false;
}