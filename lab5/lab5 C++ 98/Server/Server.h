#pragma once

#include <string>
#include <vector>
#include <windows.h>
#include "..\Shared\Employee.h"
class Server {
private:
    std::string fileName;
    std::vector<Employee> employees;
    int clientCount;

    std::vector<HANDLE> semaphores;
    std::vector<HANDLE> pipes;
    std::vector<HANDLE> threads;
    std::vector<HANDLE> clientEvents;
    std::vector<HANDLE> processHandles;
    std::vector<std::pair<HANDLE, HANDLE>> accessAllowedEvents;

public:
    Server(const std::string& fileName, int employeeCount, int clientCount);
    ~Server();

    void initializeEmployees();
    void startClients();
    void startPipeListeners();
    void waitForClients();
    void printFile();

private:
    void loadFromFile();
    void saveToFile();
    static DWORD WINAPI handleClient(LPVOID param);
};
