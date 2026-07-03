#include <fstream>
#include <iostream>
#include <string>

#include "../server/server.hpp"
#include "handlers.hpp"
#include "System.hpp"

void mapServerPaths(Server &server, AppSystem &system)
{
    server.setNotFoundErrPage("static/404.html");

    server.get("/", new ShowPage("static/index.html"));
    server.get("/login", new ShowPage("static/login.html"));
    server.get("/signup", new ShowPage("static/signup.html"));
    server.get("/dashboard", new ShowPage("static/dashboard.html"));
    server.get("/add-task", new ShowPage("static/add-task.html"));
    server.get("/edit-task", new ShowPage("static/edit-task.html"));
    server.get("/delete-task", new ShowPage("static/delete-task.html"));
    server.get("/add-event", new ShowPage("static/add-event.html"));
    server.get("/show-invitations", new ShowPage("static/show-invitations.html"));
    server.get("/report", new ShowPage("static/report.html"));

    server.post("/login", new LoginHandler(system));
    server.post("/signup", new SignUpHandler(system));
    server.post("/logout", new LogoutHandler(system));
    server.post("/add-task", new AddTaskHandler(system));
    server.post("/edit-task", new EditTaskHandler(system));
    server.post("/delete-task", new DeleteTaskHandler(system));
    server.post("/add-event", new AddEventHandler(system));
    server.post("/action/invitations", new InvitationHandler(system));

    server.get("/api/invitations", new ShowInvitationHandler(system));
    server.get("/api/report", new ReportHandler(system));
}


int main(int argc, char **argv)
{
    try
    {
        AppSystem &system = AppSystem::getInstance();
        system.initialize(argv[2]);
        int port = argc > 1 ? std::stoi(argv[1]) : 5000;
        Server server(port);
        mapServerPaths(server, system);
        std::cout << "Server running on port: " << port << std::endl;
        server.run();
    }
    catch (const std::invalid_argument &e)
    {
        std::cerr << e.what() << std::endl;
    }
    catch (const Server::Exception &e)
    {
        std::cerr << e.getMessage() << std::endl;
    }
    return 0;
}
