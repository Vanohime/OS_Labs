#pragma once
#include "..\Shared\Employee.h"
#include <vector>
#include <string>
#include <windows.h>

class Server {
private:
    std::string fileName;
    std::vector<Employee> employees;
    int clientCount;

    std::vector<HANDLE> semaphores;
    std::vector<HANDLE> pipes;
    std::vector<HANDLE> threads;
    std::vector<HANDLE> clientEvents;

public:
    Server(const std::string& fileName, int employeeCount, int clientCount);
    ~Server();

    void initializeEmployees();
    void startClients();
    void startPipeListeners();
    void waitForClients();

private:
    void loadFromFile();
    void saveToFile();
    static DWORD WINAPI handleClient(LPVOID param);
};
