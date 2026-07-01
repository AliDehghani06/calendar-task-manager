#include "Day.hpp"

Day::Day(const int &day, const int &month, const int &year)
    : Date(day, month, year) {};

void Day::addEvent(shared_ptr<Event> new_event)
{
    if (events.empty())
    {
        events.push_back(new_event);
        return;
    }
    auto it = events.begin();
    int i = 0;
    for (; it != events.end(); ++it)
    {
        if ((*it)->compareTime(new_event->get_start_time()) == LATER)
        {
            break;
        }
        i++;
    }
    events.insert(it, new_event);
}

void Day::addTask(shared_ptr<Task> new_task)
{
    if (tasks.empty())
    {
        tasks.push_back(new_task);
        return;
    }
    auto it = tasks.begin();
    int i = 0;
    for (; it != tasks.end(); ++it)
    {
        string compare = (*it)->compareTime(new_task->get_time());
        if (compare == LATER)
        {
            break;
        }
        else if (compare == EQUAL)
        {
            if (new_task->get_id() < (*it)->get_id())
                break;
        }
        i++;
    }
    tasks.insert(it, new_task);
}

int Day::numOfEvents()
{
    return events.size();
}

int Day::numOfTasks()
{
    return tasks.size() + num_of_deleted_tasks_;
}

bool Day::removeTask(int id, const string &type)
{
    for (size_t i = 0; i < tasks.size(); i++)
    {
        if (tasks[i]->checkId(id))
        {
            tasks.erase(tasks.begin() + i);
            if (type == DELETE)
            {
                num_of_deleted_tasks_++;
            }
            return true;
        }
    }
    return false;
}

bool Day::editTask(int id, const string &title, const string &time, const string &description)
{
    for (const auto &task : tasks)
    {
        if (task->checkId(id))
        {
            task->edit(title, time, description);
            return true;
        }
    }
    return false;
}

void Day::allWorkds(const string &date, bool &found, string &report)
{
    size_t i = 0, j = 0;
    bool hasEvents = !events.empty();
    bool hasTasks = !tasks.empty();

    while (i < events.size() && j < tasks.size())
    {
        found = true;
        if (events[i]->get_start_time() <= tasks[j]->get_time())
        {
            events[i]->print(date, report);
            ++i;
        }
        else
        {
            tasks[j]->print(date, report);
            ++j;
        }
    }

    while (i < events.size())
    {
        found = true;
        events[i]->print(date, report);
        ++i;
    }

    while (j < tasks.size())
    {
        found = true;
        tasks[j]->print(date, report);
        ++j;
    }

    if (!found && (hasEvents || hasTasks))
    {
        found = true;
        for (const auto &event : events)
        {
            event->print(date, report);
        }
        for (const auto &task : tasks)
        {
            task->print(date, report);
        }
    }
}


bool Day::dayReport(const string &type, string &report)
{
    if (events.empty() && tasks.empty())
    {
        return false;
    }
    bool found = false;
    string date = dateToString(year_, month_, day_);
    if (type == NOT_FOUND)
    {
        allWorkds(date, found, report);
    }
    else if (type == EVENT || type == PERIODIC_EVENT || type == JOIN_EVENT)
    {
        for (const auto &event : events)
        {
            string event_type = event->get_type();
            if (((event_type == DAILY || event_type == WEEKLY || event_type == MONTHLY) && type == PERIODIC_EVENT) || event_type == type)
            {
                found = true;
                event->print(date, report);
            }
        }
    }
    else if (type == TASK)
    {
        for (const auto &task : tasks)
        {
            found = true;
            task->print(date, report);
        }
    }
    if (!found)
    {
        return false;
    }
    return true;
}

shared_ptr<Task> Day::findTask(int id)
{
    for (const auto &task : tasks)
    {
        if (task->checkId(id))
        {
            return task;
        }
    }
    return nullptr;
}