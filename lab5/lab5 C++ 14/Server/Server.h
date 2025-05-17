#pragma once
#include <thread>
#include <string>
#include <vector>
#include <windows.h>
#include "..\Shared\Employee.h"

class Server {
private:
    std::string fileName;
    std::vector<Employee> employees;
    int clientCount;

    std::vector<HANDLE> pipes;
    std::vector<std::thread> threads;
    std::vector<HANDLE> clientEvents;
    std::vector<HANDLE> processHandles;
    std::vector<std::pair<HANDLE, HANDLE>> accessAllowedEvents;

    std::vector<std::pair<std::string, std::string>> file_pairs;

public:
    Server(const std::string& fileName, int employeeCount, int clientCount, std::vector<std::pair<std::string, std::string>> _file_pairs);
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
