#include "handlers.hpp"

#include <cstdlib>
#include <iostream>

LoginHandler::LoginHandler(AppSystem &system) : system_(system) {}

Response *LoginHandler::callback(Request *req)
{
    Response *res;
    std::string username = req->getBodyParam("username");
    std::string password = req->getBodyParam("password");

    if (username.empty() || password.empty())
    {
        return Response::redirect("/login?message=" + urlencode(BAD_REQUEST));
    }

    try
    {
        std::string command = POST + ' ' + LOGIN + " ? " + USERNAME + " \"" + username + " \" " + PASSWORD + " \"" + password + "\"";
        system_.command(command);
        string id = to_string(system_.getUser()->get_user_id());
        res = Response::redirect("/dashboard");
        res->setSessionId(id);
    }
    catch (const invalid_argument &e)
    {
        std::cerr << e.what() << std::endl;
        res = Response::redirect("/login?message=" + urlencode(e.what()));
    }
    catch (const logic_error &e)
    {
        std::cerr << e.what() << std::endl;
        res = Response::redirect("/login?message=" + urlencode(e.what()));
    }

    return res;
}

SignUpHandler::SignUpHandler(AppSystem &system) : system_(system) {}

Response *SignUpHandler::callback(Request *req)
{
    Response *res;
    std::string username = req->getBodyParam("username");
    std::string password = req->getBodyParam("password");

    if (username.empty() || password.empty())
    {
        return Response::redirect("/signup?message=" + urlencode(BAD_REQUEST));
    }

    try
    {
        std::string command = POST + ' ' + SIGNUP + " ? " + USERNAME + " \"" + username + " \" " + PASSWORD + " \"" + password + "\"";
        system_.command(command);
        string id = to_string(system_.getUser()->get_user_id());
        res = Response::redirect("/dashboard");
        res->setSessionId(id);
    }
    catch (const invalid_argument &e)
    {
        cerr << e.what() << endl;
        res = Response::redirect("/signup?message=" + urlencode(e.what()));
    }
    catch (const logic_error &e)
    {
        cerr << e.what() << endl;
        res = Response::redirect("/signup?message=" + urlencode(e.what()));
    }

    return res;
}

LogoutHandler::LogoutHandler(AppSystem &system) : system_(system) {}

Response *LogoutHandler::callback(Request *req)
{
    Response *res;
    try
    {
        std::string command = POST + ' ' + LOGOUT + " ? ";
        system_.command(command);
        res = Response::redirect("/");
        res->setSessionId("");
    }
    catch (const invalid_argument &e)
    {
        cerr << e.what() << endl;
        res = Response::redirect("/?message=" + urlencode(e.what()));
    }
    catch (const logic_error &e)
    {
        cerr << e.what() << endl;
        res = Response::redirect("/?message=" + urlencode(e.what()));
    }

    return res;
}

AddTaskHandler::AddTaskHandler(AppSystem &system) : system_(system) {}

Response *AddTaskHandler::callback(Request *req)
{
    if (req->getMethod() == Request::Method::GET)
    {
        return Response::redirect("/add-task");
    }
    else if (req->getMethod() == Request::Method::POST)
    {
        try
        {
            std::string yearStr = req->getBodyParam("taskYear");
            std::string monthStr = req->getBodyParam("taskMonth");
            std::string dayStr = req->getBodyParam("taskDay");
            std::string hourStr = req->getBodyParam("taskHour");
            std::string title = req->getBodyParam("taskTitle");
            std::string description = req->getBodyParam("taskDescription");

            if (yearStr.empty() || monthStr.empty() || dayStr.empty() || hourStr.empty() || title.empty())
            {
                return Response::redirect("/add-task?message=" + urlencode(BAD_REQUEST));
            }

            std::string date = formatDate(stoi(yearStr), stoi(monthStr), stoi(dayStr));

            std::string command = POST + ' ' + TASK + " ? " + DATE + ' ' + date + ' ' + TIME + ' ' + hourStr + ' ' + TITLE + " \"" + title + '\"';
            if (!description.empty())
            {
                command += ' ' + DESCRIPTION + " \"" + description + '\"';
            }

            std::cout << command << std::endl;
            system_.command(command);
            return Response::redirect("/dashboard");
        }
        catch (const std::exception &e)
        {
            return Response::redirect("/add-task?message=" + urlencode(e.what()));
        }
    }

    return Response::redirect("/add-task?message=" + urlencode(BAD_REQUEST));
}

EditTaskHandler::EditTaskHandler(AppSystem &system) : system_(system) {}

string EditTaskHandler::CreateCommand(Request *req)
{
    std::string idStr = req->getBodyParam("taskId");
    std::string yearStr = req->getBodyParam("taskYear");
    std::string monthStr = req->getBodyParam("taskMonth");
    std::string dayStr = req->getBodyParam("taskDay");
    std::string hourStr = req->getBodyParam("taskHour");
    std::string title = req->getBodyParam("taskTitle");
    std::string description = req->getBodyParam("taskDescription");
    std::string command = PUT + ' ' + TASK + " ? " + TASK_ID + ' ' + idStr;
    if (!yearStr.empty())
    {
        std::string date = formatDate(stoi(yearStr), stoi(monthStr), stoi(dayStr));
        command = command + ' ' + DATE + ' ' + date;
    }
    if (!hourStr.empty())
    {
        command = command + ' ' + TIME + ' ' + hourStr;
    }
    if (!title.empty())
    {
        command = command + ' ' + TITLE + " \"" + title + '\"';
    }
    if (!description.empty())
    {
        command += ' ' + DESCRIPTION + " \"" + description + '\"';
    }
    return command;
}

Response *EditTaskHandler::callback(Request *req)
{
    if (req->getMethod() == Request::Method::GET)
    {
        return Response::redirect("/edit-task");
    }
    else if (req->getMethod() == Request::Method::POST)
    {
        try
        {
            string command = CreateCommand(req);
            std::cout << command << std::endl;
            system_.command(command);
            return Response::redirect("/dashboard");
        }
        catch (const std::exception &e)
        {
            return Response::redirect("/edit-task?message=" + urlencode(e.what()));
        }
    }

    return Response::redirect("/edit-task?message=" + urlencode("Invalid request method."));
}

DeleteTaskHandler::DeleteTaskHandler(AppSystem &system) : system_(system) {}

Response *DeleteTaskHandler::callback(Request *req)
{
    if (req->getMethod() == Request::Method::GET)
    {
        return Response::redirect("/delete-task");
    }
    else if (req->getMethod() == Request::Method::POST)
    {
        try
        {
            std::string idStr = req->getBodyParam("taskId");
            std::string command = DELETE + ' ' + TASK + " ? " + TASK_ID + ' ' + idStr;
            std::cout << command << std::endl;
            system_.command(command);
            return Response::redirect("/dashboard");
        }
        catch (const std::exception &e)
        {
            return Response::redirect("/delete-task?message=" + urlencode(e.what()));
        }
    }

    return Response::redirect("/delete-task?message=" + urlencode("Invalid request method."));
}

AddEventHandler::AddEventHandler(AppSystem &system) : system_(system) {}

string AddEventHandler::CreateCommandEvent(Request *req)
{
    string command;
    std::string title = req->getBodyParam("eventTitle");
    std::string yearStr = req->getBodyParam("eventDateYear");
    std::string monthStr = req->getBodyParam("eventDateMonth");
    std::string dayStr = req->getBodyParam("eventDateDay");
    std::string start_time_Str = req->getBodyParam("eventStartHour");
    std::string duration = req->getBodyParam("eventDuration");
    std::string date = formatDate(stoi(yearStr), stoi(monthStr), stoi(dayStr));
    command = POST + ' ' + EVENT + " ? " + DATE + ' ' + date + ' ' +
              START_TIME + ' ' + start_time_Str + ' ' + DURATION +
              ' ' + duration + ' ' + TITLE + " \"" + title + '\"';
    return command;
}

string AddEventHandler::CreateCommandPeriodicEvent(Request *req)
{
    string command;
    string title = req->getBodyParam("eventTitle");
    string start_yearStr = req->getBodyParam("periodicStartYear");
    string start_monthStr = req->getBodyParam("periodicStartMonth");
    string start_dayStr = req->getBodyParam("periodicStartDay");
    string end_yearStr = req->getBodyParam("periodicEndYear");
    string end_monthStr = req->getBodyParam("periodicEndMonth");
    string end_dayStr = req->getBodyParam("periodicEndDay");
    string start_time_Str = req->getBodyParam("periodicStartHour");
    string duration = req->getBodyParam("periodicDuration");
    string week_days = req->getBodyParam("periodicDaysOfWeek");
    string start_date = formatDate(stoi(start_yearStr), stoi(start_monthStr), stoi(start_dayStr));
    string end_date = formatDate(stoi(end_yearStr), stoi(end_monthStr), stoi(end_dayStr));
    command = POST + ' ' + PERIODIC_EVENT + " ? " + START_DATE + ' ' + start_date + ' ' + END_DATE + ' ' + end_date + ' ' + START_TIME + ' ' + start_time_Str + ' ' + DURATION + ' ' + duration + ' ' + TYPE + ' ' + WEEKLY + ' ' + WEEK_DAYS + ' ' + week_days + ' ' + TITLE + " \"" + title + '\"';
    return command;
}

string AddEventHandler::CreateCommandJoinEvent(Request *req)
{
    string command;
    std::string title = req->getBodyParam("eventTitle");
    std::string yearStr = req->getBodyParam("jointEventDateYear");
    std::string monthStr = req->getBodyParam("jointEventDateMonth");
    std::string dayStr = req->getBodyParam("jointEventDateDay");
    std::string start_time_Str = req->getBodyParam("jointStartHour");
    std::string end_time_Str = req->getBodyParam("jointEndHour");
    std::string guest = req->getBodyParam("guestUsername");
    std::string date = formatDate(stoi(yearStr), stoi(monthStr), stoi(dayStr));
    command = POST + ' ' + JOIN_EVENT + " ? " + GUESTS + " \"" + guest + "\" " + DATE + ' ' + date + ' ' +
              START_TIME + ' ' + start_time_Str + ' ' + END_TIME + ' ' + end_time_Str + ' ' + TITLE + " \"" + title + '\"';
    return command;
}

Response *AddEventHandler::callback(Request *req)
{
    if (req->getMethod() == Request::Method::GET)
    {
        return Response::redirect("/add-task");
    }
    else if (req->getMethod() == Request::Method::POST)
    {
        try
        {
            std::string command;
            std::string type = req->getBodyParam("eventType");
            std::string description = req->getBodyParam("eventDescription");

            if (type == EVENT)
            {
                command = CreateCommandEvent(req);
            }
            else if (type == PERIODIC_EVENT)
            {
                command = CreateCommandPeriodicEvent(req);
            }
            else if (type == JOIN_EVENT)
            {
                command = CreateCommandJoinEvent(req);
            }
            if (!description.empty())
            {
                command += ' ' + DESCRIPTION + " \"" + description + '\"';
            }

            std::cout << command << std::endl;
            system_.command(command);
            return Response::redirect("/dashboard");
        }
        catch (const std::exception &e)
        {
            return Response::redirect("/add-event?message=" + urlencode(e.what()));
        }
    }

    return Response::redirect("/add-event?message=" + urlencode(BAD_REQUEST));
}

ShowInvitationHandler::ShowInvitationHandler(AppSystem &system) : system_(system) {}

Response *ShowInvitationHandler::callback(Request *req)
{
    Response *res = new Response;
    try
    {
        std::shared_ptr<User> user = system_.getUser();

        std::stringstream buffer;
        user->handleInvitations(buffer);

        res->setBody(buffer.str());
        res->setHeader("Content-Type", "application/json");
    }
    catch (const std::exception &e)
    {
        res->setBody("{\"error\":\"" + std::string(e.what()) + "\"}");
        res->setHeader("Content-Type", "application/json");
    }
    return res;
}

ReportHandler::ReportHandler(AppSystem &system) : system_(system) {}

Response *ReportHandler::callback(Request *req)
{
    Response *res = new Response;
    res->setHeader("Content-Type", "text/plain; charset=utf-8");
    try
    {
        std::shared_ptr<User> user = system_.getUser();
        if (!user)
        {
            throw std::runtime_error("User not authenticated");
        }

        std::string start_year = req->getQueryParam("startYear");
        std::string start_month = req->getQueryParam("startMonth");
        std::string start_day = req->getQueryParam("startDay");
        std::string end_year = req->getQueryParam("endYear");
        std::string end_month = req->getQueryParam("endMonth");
        std::string end_day = req->getQueryParam("endDay");

        std::string start_date = formatDate(stoi(start_year), stoi(start_month), stoi(start_day));
        std::string end_date = formatDate(stoi(end_year), stoi(end_month), stoi(end_day));

        std::string report;
        user->getReport(start_date, end_date, NOT_FOUND, report);
        res->setBody(report);
    }
    catch (const std::exception &e)
    {
        std::ostringstream errorBody;
        errorBody << "{\"error\": " << std::quoted(e.what()) << "}";
        res->setBody(errorBody.str());
    }

    return res;
}

InvitationHandler::InvitationHandler(AppSystem &system) : system_(system) {}

Response *InvitationHandler::callback(Request *req)
{
    Response *res = new Response;

    try
    {
        string command;
        string id = req->getBodyParam("invitationId");
        string action = req->getBodyParam("actionType");
        if (action != CONFIRM_JOIN_EVENT || action != REJECT_JOIN_EVENT)
            cout << BAD_REQUEST << endl;
        cout << action << endl;
        command = POST + ' ' + action + " ? " + INVITATION_ID + ' ' + id;
        std::cout << command << std::endl;
        system_.command(command);
        return Response::redirect("/show-invitations");
    }
    catch (const std::exception &e)
    {
        return Response::redirect("/show-invitations?message=" + urlencode(e.what()));
    }

    return res;
}