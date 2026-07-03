#include "PostCommands.hpp"
#include "System.hpp"
#include "UserManager.hpp"

Post &Post::getInstance()
{
    static Post instance;
    return instance;
}

Post::Post()
    : system(AppSystem::getInstance()),
      holiday(HolidayManager::getInstance()),
      user(system.getUser()) {}

bool Post::handleSignUp()
{
    if (username == NOT_FOUND || password == NOT_FOUND)
        throw invalid_argument(BAD_REQUEST);
    else if (username == EMPTY || password == EMPTY)
        throw invalid_argument(BAD_REQUEST);
    else if (user_log_in)
        throw logic_error(PERMISSION_DENIED);
    else if (UserManager::getInstance().signUpUser(username, password))
    {
        system.setUser(UserManager::getInstance().findUser(username));
        user = system.getUser();
        return true;
    }
    return false;
}

bool Post::handleLogIn()
{
    shared_ptr<User> login_user = UserManager::getInstance().findUser(username);
    if (user_log_in)
        throw logic_error(PERMISSION_DENIED);
    else if (login_user == nullptr)
        throw invalid_argument(NOT_FOUND);
    else if (username == NOT_FOUND || password == NOT_FOUND)
        throw invalid_argument(BAD_REQUEST);
    else if (username == EMPTY || password == EMPTY)
        throw invalid_argument(BAD_REQUEST);
    else if (!login_user->checkPassword(password))
        throw invalid_argument(PERMISSION_DENIED);
    else
    {
        system.setUser(login_user);
        user = login_user;
        return true;
    }
    return false;
}

bool Post::handleLogOut()
{
    if (!user_log_in)
        throw invalid_argument(PERMISSION_DENIED);
    else
    {
        system.logOutUser();
        //user = nullptr;
        return true;
    }
}

bool Post::checkExceptions(string type)
{
    if (type == EVENT)
    {
        if (date == NOT_FOUND || !validateDate(date))
            throw invalid_argument(BAD_REQUEST);
    }
    else if (type == PERIODIC_EVENT)
    {
        if (start_date == NOT_FOUND || end_date == NOT_FOUND || type == NOT_FOUND)
            throw invalid_argument(BAD_REQUEST);
        else if (!validateDate(start_date) && !validateDate(end_date))
            throw invalid_argument(BAD_REQUEST);
    }
    if (start_time == NOT_FOUND || duration == NOT_FOUND || title == NOT_FOUND || title == EMPTY)
        throw invalid_argument(BAD_REQUEST);
    else if (!isValidStartTime(start_time) && !isValidDuration(duration))
        throw invalid_argument(BAD_REQUEST);
    else if (!user_log_in)
        throw invalid_argument(PERMISSION_DENIED);
    else
        return true;
    return false;
}

bool Post::handleEvent()
{
    if (!checkExceptions(EVENT))
        return false;
    if (user->checkOverlap(date, stoi(start_time), stoi(duration)))
        throw logic_error(OVERLAP);
    else if (holiday.checkHoliday(date))
        throw logic_error(HOLIDAY_FOUND);
    else
    {
        user->addEvent(date, make_shared<Event>(user->numOfAllEvents() + 1, EVENT, title, stoi(start_time), stoi(duration), description));
        return true;
    }
    return false;
}

bool Post::checkDaily()
{
    int distance = findDistance(start_date, end_date);
    string current_date = start_date;
    for (int i = 0; i < distance; i++)
    {
        if (user->checkOverlap(current_date, stoi(start_time), stoi(duration)))
        {
            return true;
        }
        current_date = nextDay(current_date);
    }
    return false;
}

bool Post::handleDailyEvent()
{
    bool hloiday_found = false;
    if (!checkExceptions(PERIODIC_EVENT))
        return false;
    if (checkDaily())
        throw logic_error(OVERLAP);
    else
    {
        int distance = findDistance(start_date, end_date);
        string current_date = start_date;
        for (int i = 0; i <= distance; i++)
        {
            if (HolidayManager::getInstance().checkHoliday(current_date))
            {
                hloiday_found = true;
                current_date = nextDay(current_date);
                continue;
            }
            user->addEvent(current_date, make_shared<Event>(user->numOfAllEvents() + 1, DAILY, title, stoi(start_time), stoi(duration), description));
            current_date = nextDay(current_date);
        }
        if (hloiday_found)
        {
            throw logic_error(HOLIDAY_FOUND);
        }
        return true;
    }
    return false;
}

bool Post::checkWeekly(const vector<string> &week_days)
{
    int distance = findDistance(start_date, end_date);
    string current_date = start_date;
    int distance_from_first = findDistance(FIRST_DAY, start_date);
    for (int i = 0; i <= distance; i++)
    {
        if (i > 0)
            current_date = nextDay(current_date);
        int day_of_week = (distance_from_first + i) % 7;
        for (const auto &day : week_days)
        {
            if (day_of_week == system.dayCode(day))
            {
                if (user->checkOverlap(current_date, stoi(start_time), stoi(duration)))
                {
                    return true;
                }
                break;
            }
        }
    }
    return false;
}

bool Post::handleWeeklyEvent(const vector<string> &words)
{
    string week = DesiredWord(words, WEEK_DAYS);
    vector<string> week_days = Separator(week, ',');
    bool hloiday_found = false;
    if (!checkExceptions(PERIODIC_EVENT))
        return false;
    if (week == NOT_FOUND || week == EMPTY)
        throw invalid_argument(PERMISSION_DENIED);
    else if (checkWeekDays(week_days))
        throw invalid_argument(BAD_REQUEST);
    else if (checkWeekly(week_days))
        throw logic_error(OVERLAP);
    else
    {
        int distance = findDistance(start_date, end_date);
        string current_date = start_date;
        int distance_from_first = findDistance(FIRST_DAY, start_date);
        for (int i = 0; i <= distance; i++)
        {
            current_date = (i == 0) ? start_date : nextDay(current_date);
            int day_of_week = (distance_from_first + i) % 7;
            for (const auto &day : week_days)
            {
                if (day_of_week == system.dayCode(day))
                {
                    if (HolidayManager::getInstance().checkHoliday(current_date))
                    {
                        hloiday_found = true;
                        continue;
                    }
                    user->addEvent(current_date, make_shared<Event>(user->numOfAllEvents() + 1, WEEKLY, title, stoi(start_time), stoi(duration), description));
                    break;
                }
            }
        }
        if (hloiday_found)
        {
            throw logic_error(HOLIDAY_FOUND);
        }
        return true;
    }
    return false;
}

bool Post::checkMonthly(const string &day)
{
    int distance = findDistance(start_date, end_date);
    string current_date = start_date;
    for (int i = 0; i < distance; i++)
    {
        vector<string> current_date_parts = Separator(current_date, '/');
        string date_day = current_date_parts[2];
        if (date_day[0] == '0')
        {
            date_day = date_day[1];
        }
        if (date_day == day)
        {
            if (user->checkOverlap(current_date, stoi(start_time), stoi(duration)))
            {
                return true;
            }
        }
        current_date = nextDay(current_date);
    }
    return false;
}

bool Post::handleMonthlyEvent(const vector<string> &words)
{
    string day = DesiredWord(words, DAY);
    bool hloiday_found = false;
    if (!checkExceptions(PERIODIC_EVENT))
        return false;
    if (!isValidDay(day))
        throw invalid_argument(BAD_REQUEST);
    else if (checkMonthly(day))
        throw logic_error(OVERLAP);
    else
    {
        int distance = findDistance(start_date, end_date);
        string current_date = start_date;
        for (int i = 0; i <= distance; i++)
        {
            vector<string> current_date_parts = Separator(current_date, '/');
            string date_day = current_date_parts[2];
            if (date_day[0] == '0')
            {
                date_day = date_day[1];
            }
            if (date_day == day)
            {
                if (HolidayManager::getInstance().checkHoliday(current_date))
                {
                    hloiday_found = true;
                    continue;
                }
                user->addEvent(current_date, make_shared<Event>(user->numOfAllEvents() + 1, MONTHLY, title, stoi(start_time), stoi(duration), description));
            }
            current_date = nextDay(current_date);
        }
        if (hloiday_found)
        {
            throw logic_error(HOLIDAY_FOUND);
        }
        return true;
    }
    return false;
}

bool Post::handleAddTask(const vector<string> &words)
{
    string time = DesiredWord(words, TIME);
    if (date == NOT_FOUND || time == NOT_FOUND || title == NOT_FOUND)
        throw invalid_argument(BAD_REQUEST);
    else if (date == EMPTY || time == EMPTY || title == EMPTY)
        throw invalid_argument(BAD_REQUEST);
    else if (!validateDate(date) && !isValidDuration(time))
        throw invalid_argument(BAD_REQUEST);
    else if (!user_log_in)
        throw invalid_argument(PERMISSION_DENIED);
    else
    {
        user->addTask(user->numOfAllTasks() + 1, date, stoi(time), title, description);
        return true;
    }
    return false;
}

bool Post::handleJoinEvent(const vector<string> &words)
{
    string all_guests = DesiredWord(words, GUESTS);
    vector<string> guests = Separator(all_guests, ',');
    string start_time = DesiredWord(words, START_TIME);
    string end_time = DesiredWord(words, END_TIME);
    if (start_time == NOT_FOUND || end_time == NOT_FOUND || title == NOT_FOUND || title == EMPTY)
        throw invalid_argument(BAD_REQUEST);
    else if (!isValidStartTime(start_time) && !isValidStartTime(end_time))
        throw invalid_argument(BAD_REQUEST);
    else if (!validateDate(date))
        throw invalid_argument(BAD_REQUEST);
    else if (!UserManager::getInstance().checkGuests(guests))
        throw invalid_argument(NOT_FOUND);
    else if (user->checkOverlap(date, stoi(start_time), stoi(end_time) - stoi(start_time)))
        throw logic_error(OVERLAP);
    else if (holiday.checkHoliday(date))
        throw logic_error(HOLIDAY_FOUND);
    else
    {
        user->addToWaitingQueue(date, JOIN_EVENT, guests, stoi(start_time), stoi(end_time), title, description);
        return true;
    }
    return false;
}

bool Post::handleInvitation(const vector<string> &words, const string &command)
{
    string id = DesiredWord(words, INVITATION_ID);
    if (id == NOT_FOUND || !isPositiveInteger(id))
        throw invalid_argument(BAD_REQUEST);
    else if (!system.checkLogin())
        throw invalid_argument(PERMISSION_DENIED);
    else
    {
        if (command == CONFIRM_JOIN_EVENT)
            user->confirmInvitation(stoi(id));
        else
            user->rejectInvitation(stoi(id));
        return true;
    }
    return false;
}

void Post::parameterValues(const vector<string> &words)
{
    username = DesiredWord(words, USERNAME);
    password = DesiredWord(words, PASSWORD);
    date = DesiredWord(words, DATE);
    start_time = DesiredWord(words, START_TIME);
    duration = DesiredWord(words, DURATION);
    title = DesiredWord(words, TITLE);
    description = DesiredWord(words, DESCRIPTION);
    start_date = DesiredWord(words, START_DATE);
    end_date = DesiredWord(words, END_DATE);
    type = DesiredWord(words, TYPE);
    id = DesiredWord(words, TASK_ID);
    user_log_in = system.checkLogin();
}

bool Post::handlePOST(const vector<string> &words)
{
    if (words.size() < 3 || words[2] != "?")
    {
        throw invalid_argument(BAD_REQUEST);
    }
    parameterValues(words);
    const string &command = words[1];
    if (command == SIGNUP)
        return handleSignUp();
    else if (command == LOGIN)
        return handleLogIn();
    else if (command == LOGOUT)
        return handleLogOut();
    else if (command == EVENT)
        return handleEvent();
    else if (command == PERIODIC_EVENT)
    {
        if (type == DAILY)
            return handleDailyEvent();
        else if (type == WEEKLY)
            return handleWeeklyEvent(words);
        else if (type == MONTHLY)
            return handleMonthlyEvent(words);
    }
    else if (command == TASK)
        return handleAddTask(words);
    else if (command == JOIN_EVENT)
    {
        return handleJoinEvent(words);
    }
    else if (command == CONFIRM_JOIN_EVENT)
    {
        return handleInvitation(words, CONFIRM_JOIN_EVENT);
    }
    else if (command == REJECT_JOIN_EVENT)
    {
        return handleInvitation(words, REJECT_JOIN_EVENT);
    }
    else
        throw invalid_argument(NOT_FOUND);
    return false;
}