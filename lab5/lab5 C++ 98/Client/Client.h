#pragma once
#include "..\Shared\Employee.h"
#include <windows.h>

class Client {
private:
    HANDLE pipe;

public:
    Client();
    ~Client();

    void run();

private:
    void modifyEmployee(int id);
    void readEmployee(int id);
    void sendMessage(int message);
    void sendEmployee(const Employee& emp);
    Employee receiveEmployee();
};
