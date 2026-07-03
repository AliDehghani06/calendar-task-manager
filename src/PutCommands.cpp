#include "PutCommands.hpp"
#include "System.hpp"
#include "User.hpp"

Put& Put::getInstance(){
    static Put instance;
    return instance;
}

Put::Put()
    : system(AppSystem::getInstance()),
      user(system.getUser()) {}


bool Put::handleEditTask()
{
    if (date == NOT_FOUND && time == NOT_FOUND && title == NOT_FOUND && description == NOT_FOUND)
        throw invalid_argument(BAD_REQUEST);
    else if ((date != NOT_FOUND && !validateDate(date)) || (time != NOT_FOUND && !isValidDuration(time)) ||
             (title != NOT_FOUND && title == EMPTY) || (description != NOT_FOUND && description == EMPTY))
        throw invalid_argument(BAD_REQUEST);
    else if (id == NOT_FOUND || !isPositiveInteger(id))
        throw invalid_argument(BAD_REQUEST);
    else if (!system.checkLogin())
        throw invalid_argument(PERMISSION_DENIED);
    else
    {
        if (!user->editTask(stoi(id),date, title, time, description))
            throw logic_error(NOT_FOUND);
        return true;
    }
}

void Put::parameterValues(const vector<string>& words){
    if(words[2] != "?")
        throw invalid_argument(BAD_REQUEST);
    id = DesiredWord(words, TASK_ID);
    date = DesiredWord(words, DATE);
    time = DesiredWord(words, TIME);
    title = DesiredWord(words, TITLE);
    description = DesiredWord(words, DESCRIPTION);
    user = system.getUser();
}

bool Put::handlePUT(const vector<string> &words)
{
    parameterValues(words);
    cout << user.get()->get_username() << endl;
    if (words[1] == TASK)
    {
        return handleEditTask();
    }
    else{
        throw invalid_argument(NOT_FOUND);
    }
    return false;
}
