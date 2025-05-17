#pragma once
#include "..\Shared\Employee.h"
#include <windows.h>
#include <string>

class Client {
private:
    HANDLE pipe;
    std::string pipeName;

public:
    Client(int pipeIndex);
    ~Client();

    void run();

private:
    void modifyEmployee(int id);
    void readEmployee(int id);
    void sendMessage(int message);
    void sendEmployee(Employee emp);
    int getStatus();
    Employee receiveEmployee();
};
