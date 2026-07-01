#ifndef HANDLERS_HPP_INCLUDE
#define HANDLERS_HPP_INCLUDE

#include <map>
#include <string>

#include "Consts.hpp"
#include "System.hpp"
#include "../server/server.hpp"

class LoginHandler : public RequestHandler {
public:
    LoginHandler(AppSystem &system);
    Response* callback(Request*) override;
private:
    AppSystem &system_;
};

class SignUpHandler : public RequestHandler {
public:
    SignUpHandler(AppSystem &system);
    Response* callback(Request*) override;
private:
    AppSystem &system_;
};

class LogoutHandler : public RequestHandler {
public:
    LogoutHandler(AppSystem &system);
    Response* callback(Request*) override;
private:
    AppSystem &system_;
};

class AddTaskHandler : public RequestHandler {
public:
    AddTaskHandler(AppSystem &system);
    Response* callback(Request*) override;
private:
    AppSystem &system_;
};

class EditTaskHandler : public RequestHandler {
public:
    EditTaskHandler(AppSystem &system);
    Response* callback(Request*) override;
private:
    string CreateCommand(Request *req);

    AppSystem &system_;
};

class DeleteTaskHandler : public RequestHandler {
public:
    DeleteTaskHandler(AppSystem &system);
    Response* callback(Request*) override;
private:
    AppSystem &system_;
};

class AddEventHandler : public RequestHandler {
public:
    AddEventHandler(AppSystem &system);
    Response* callback(Request*) override;
private:
    string CreateCommandEvent(Request *req);
    string CreateCommandPeriodicEvent(Request *req);
    string CreateCommandJoinEvent(Request *req);

    AppSystem &system_;
};

class ShowInvitationHandler : public RequestHandler {
public:
    ShowInvitationHandler(AppSystem &system);
    Response* callback(Request*) override;
private:
    AppSystem &system_;
};

class ReportHandler : public RequestHandler {
public:
    ReportHandler(AppSystem &system);
    Response* callback(Request*) override;
private:
    AppSystem &system_;
};

class InvitationHandler : public RequestHandler {
public:
    InvitationHandler(AppSystem &system);
    Response* callback(Request*) override;
private:
    AppSystem &system_;
};

#endif 
