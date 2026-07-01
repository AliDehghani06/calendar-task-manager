#include "User.hpp"

User::User(const string &username, const string &password, int user_id)
    : user_id_(user_id), username_(username), password_(password) {}

bool User::checkName(const string &username, string type)
{
        if (type == JOIN_EVENT)
        {
                return username == username_.substr(0, username_.size() - 1);
        }
        return username == username_;
}

bool User::checkPassword(const string &password)
{
        return password_ == password;
}

bool User::checkDay(const string &date, shared_ptr<Day> day)
{
        vector<string> date_parts = Separator(date, '/');
        if (day->checkYear(stoi(date_parts[0])))
                if (day->checkMonth(stoi(date_parts[1])))
                        if (day->checkDay(stoi(date_parts[2])))
                                return true;
        return false;
}

bool User::checkOverlap(const string &date, int start_time, int duration)
{
        for (const auto &day : days_)
        {
                if (checkDay(date, day))
                {
                        for (const auto &event : day->get_events())
                        {
                                if (event->checkOverlap(start_time, duration))
                                        return true;
                        }
                }
        }
        return false;
}

int User::numOfAllEvents()
{
        int count = 0;
        for (const auto &day : days_)
        {
                count += day->numOfEvents();
        }
        return count;
}

int User::numOfAllTasks()
{
        int count = 0;
        for (const auto &day : days_)
        {
                count += day->numOfTasks();
        }
        return count;
}

void User::addEvent(const string &date, shared_ptr<Event> new_event)
{
        if (!days_.empty())
        {
                for (const auto &day : days_)
                {
                        if (checkDay(date, day))
                        {
                                day->addEvent(new_event);
                                return;
                        }
                }
        }
        vector<string> date_parts = Separator(date, '/');
        auto new_day = make_shared<Day>(stoi(date_parts[2]), stoi(date_parts[1]), stoi(date_parts[0]));
        new_day->addEvent(new_event);
        auto it = days_.begin();
        for (; it != days_.end(); ++it)
        {
                if ((*it)->compare(*new_day) == LATER)
                {
                        break;
                }
        }
        days_.insert(it, new_day);
}

void User::addTask(int id, const string &date, int time, const string &title, const string &description)
{

        if (!days_.empty())
        {
                for (const auto &day : days_)
                {
                        if (checkDay(date, day))
                        {
                                day->addTask(make_shared<Task>(id, title, time, description));
                                return;
                        }
                }
        }

        vector<string> date_parts = Separator(date, '/');
        auto new_day = make_shared<Day>(stoi(date_parts[2]), stoi(date_parts[1]), stoi(date_parts[0]));
        new_day->addTask(make_shared<Task>(id, title, time, description));
        auto it = days_.begin();

        for (; it != days_.end(); ++it)
        {
                if ((*it)->compare(*new_day) == LATER)
                {
                        break;
                }
        }

        days_.insert(it, new_day);
}

bool User::deleteTask(int id)
{
        for (const auto &day : days_)
        {
                if (day->removeTask(id, DELETE))
                        return true;
        }
        return false;
}

bool User::editTask(int id, string date, const string &title, const string &time, const string &description)
{
        if (days_.empty())
        {
                throw logic_error(NOT_FOUND);
        }
        if (date == NOT_FOUND)
        {
                for (const auto &day : days_)
                {
                        if (day->editTask(id, title, time, description))
                                return true;
                }
                return false;
        }
        shared_ptr<Day> task_day = nullptr;
        shared_ptr<Task> task = nullptr;
        for (const auto &day : days_)
        {
                task = day->findTask(id);
                if (task != nullptr)
                {
                        task_day = day;
                        break;
                }
        }
        if (task == nullptr)
        {
                return false;
        }
        vector<string> date_parts = Separator(date, '/');
        auto new_day = make_shared<Day>(stoi(date_parts[2]), stoi(date_parts[1]), stoi(date_parts[0]));
        if (task_day->compare(*new_day) == EQUAL)
        {
                return task_day->editTask(id, title, time, description);
        }
        else
        {
                string time_ = time != NOT_FOUND ? time : to_string(task->get_time());
                string title_ = title != NOT_FOUND ? title : task->get_title();
                string description_ = description != NOT_FOUND ? description : task->get_description();
                try
                {
                        addTask(id, date, stoi(time_), title_, description_);
                        task_day->removeTask(id, REPLACE);
                        return true;
                }
                catch (...)
                {
                        return false;
                }
        }
}

void User::getReport(string from, const string &to, const string &type, string &report)
{
        if (days_.empty())
        {
                throw invalid_argument(EMPTY);
        }
        bool empty = true;
        if (from == NOT_FOUND)
        {
                from = FIRST_DAY;
        }
        vector<string> from_parts = Separator(from, '/');
        vector<string> to_parts = Separator(to, '/');
        Date from_date(stoi(from_parts[2]), stoi(from_parts[1]), stoi(from_parts[0]));
        Date to_date(stoi(to_parts[2]), stoi(to_parts[1]), stoi(to_parts[0]));
        for (const auto &day : days_)
        {
                string cmp_from = day->compare(from_date);
                string cmp_to = day->compare(to_date);
                if ((cmp_from == EQUAL || cmp_from == LATER) &&
                    (cmp_to == EQUAL || cmp_to == EARLIER))
                {
                        if (day->dayReport(type, report))
                                empty = false;
                }
                else if(cmp_to == LATER)
                        break;
        }
        if (empty)
        {
                throw invalid_argument(EMPTY);
        }
}

void User::addToWaitingQueue(const string &date, const string &type, const vector<string> &guests, int start_time, int end_time, const string &title, const string &description)
{
        int duration = end_time - start_time;
        int num_of_join_events = AppSystem::getInstance().get_num_of_join_events();
        shared_ptr<JoinEvent> new_one = make_shared<JoinEvent>(num_of_join_events + 1, username_, date, type, title,
                                                               start_time, duration, description, end_time, guests);
        waiting_queue_.push_back((new_one));
        UserManager::getInstance().sendToGuests(guests, new_one);
        AppSystem::getInstance().increase_num_of_join_events();
}

void User::addInvitation(shared_ptr<JoinEvent> new_one)
{
        invitations_.push_back(new_one);
}

bool User::handleInvitations(ostream &json)
{
        if (invitations_.empty())
                throw logic_error(EMPTY);
        json << "[";

        for (size_t i = 0; i < invitations_.size(); ++i)
        {
                invitations_[i]->handlejson(json);
                if (i < invitations_.size() - 1)
                        json << ",";
                invitations_[i]->printInvitation();
        }

        json << "]";
        return true;
}

void User::confirmInvitation(int id)
{
        for (const auto &invitation : invitations_)
        {
                if (invitation->checkId(id))
                {
                        if (checkOverlap(invitation->get_date(), invitation->get_start_time(), invitation->get_duration()))
                        {
                                rejectInvitation(id);
                                throw logic_error(OVERLAP);
                        }
                        else if (HolidayManager::getInstance().checkHoliday(invitation->get_date()))
                        {
                                rejectInvitation(id);
                                throw logic_error(HOLIDAY_FOUND);
                        }
                        else
                        {
                                addEvent(invitation->get_date(), invitation);
                                if (!invitation->get_host_status())
                                {
                                        UserManager::getInstance().joinHost(invitation);
                                }
                                invitation->set_host_status();
                                rejectInvitation(id);
                                return;
                        }
                }
        }
        throw invalid_argument(NOT_FOUND);
}

void User::rejectInvitation(int id)
{
        auto it = std::find_if(invitations_.begin(), invitations_.end(),
                               [id](const auto &inv)
                               { return inv->checkId(id); });
        if (it != invitations_.end())
        {
                invitations_.erase(it);
        }
        else
        {
                throw std::invalid_argument(NOT_FOUND);
        }
}