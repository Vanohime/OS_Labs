#include "Server.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <conio.h>
#include <tchar.h>


struct ThreadParam {
    HANDLE pipe;
    std::vector<Employee>* employees;
    std::vector<HANDLE>* semaphores;
    std::string fileName;
    int clientCount;
};

Server::Server(const std::string& fileName, int employeeCount, int clientCount)
    : fileName(fileName), clientCount(clientCount) {
    employees.resize(employeeCount);
    semaphores.resize(employeeCount);

    for (int i = 0; i < employeeCount; ++i) {
        semaphores[i] = CreateSemaphore(NULL, clientCount, clientCount, NULL);
    }
}

Server::~Server() {
    for (HANDLE h : semaphores) CloseHandle(h);
    //for (HANDLE h : pipes) CloseHandle(h);
    for (HANDLE h : threads) CloseHandle(h);
    for (HANDLE h : clientEvents) CloseHandle(h);
    for (HANDLE h : processHandles) CloseHandle(h);
}

void Server::initializeEmployees() {
    for (int i = 0; i < employees.size(); ++i) {

        std::cout << "Сотрудник " << i + 1 << ":\n";
        std::cout << "Имя: "; std::cin >> employees[i].name;
        std::cout << "Часы: "; std::cin >> employees[i].hours;
        employees[i].id = i + 1;
    }
    saveToFile();
}

void Server::startClients() {
    for (int i = 0; i < clientCount; ++i) {
        STARTUPINFO si;
        PROCESS_INFORMATION pi;
        ZeroMemory(&si, sizeof(STARTUPINFO));
        si.cb = sizeof(STARTUPINFO);
        std::string cmd = "Client.exe " + std::to_string(i);

        if (CreateProcess(NULL, cmd.data(), NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi)) {
            CloseHandle(pi.hThread);
            processHandles.push_back(pi.hProcess);
        }
        else {
            std::cerr << "Не удалось создать процесс клиента\n";
        }
    }
}

void Server::startPipeListeners() {
    for (int i = 0; i < clientCount; ++i) {
        std::string ss =  "\\\\.\\pipe\\pipe" +  std::to_string(i);

        HANDLE pipe = CreateNamedPipe(ss.data(), 
            PIPE_ACCESS_DUPLEX, PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE | PIPE_WAIT, 
            PIPE_UNLIMITED_INSTANCES, 0, 0, INFINITE, NULL);


        pipes.push_back(pipe);

        ThreadParam* param = new ThreadParam{ pipe, &employees, &semaphores, fileName, clientCount };
        HANDLE hThread = CreateThread(NULL, 0, handleClient, param, 0, NULL);
        threads.push_back(hThread); 
    }
}

void Server::waitForClients() {
    WaitForMultipleObjects(static_cast<DWORD>(threads.size()), threads.data(), TRUE, INFINITE);
    WaitForMultipleObjects(static_cast<DWORD>(processHandles.size()), processHandles.data(), TRUE, INFINITE);
}

void Server::loadFromFile() {
    std::ifstream in(fileName);
    for (int i = 0; i < employees.size(); ++i) {
        in >> employees[i].id >> employees[i].name >> employees[i].hours;
    }
    in.close();
}

void Server::saveToFile() {
    std::ofstream out(fileName);
    for (const auto& emp : employees) {
        out << emp.id << ' ' << emp.name << ' ' << emp.hours << '\n';
    }
    out.close();
}

void Server::printFile() {
    std::ifstream file;
    file.open(fileName, std::ios::in);
    std::string s;
    std::cout << "modified file: \n";
    while (!file.eof()) {
        std::getline(file, s);
        std::cout << s << '\n';
    }

}
DWORD WINAPI Server::handleClient(LPVOID param) {
    ThreadParam* p = static_cast<ThreadParam*>(param);
    HANDLE pipe = p->pipe;
    auto& employees = *p->employees;
    auto& semaphores = *p->semaphores;
    std::string fileName = p->fileName;
    int clientCount = p->clientCount;

    ConnectNamedPipe(pipe, NULL);

    DWORD bytesRead = 0, bytesWritten = 0;
    int message;

    while (true) {

        ReadFile(pipe, &message, sizeof(message), &bytesRead, NULL);

        int id = message / 10;
        int op = message % 10;

        if (op == 1) { //modify
            for (int i = 0; i < clientCount; ++i) {
                WaitForSingleObject(semaphores[id - 1], INFINITE);
            }
            Employee* emp = new Employee;
            *emp = employees[id - 1];
            bool written = WriteFile(pipe, emp, sizeof(Employee), &bytesWritten, NULL);
            if (written)
            {
                std::cout << "Данные отправлены\n";
            }
            else
            {
                std::cout << "Данные не были отправлены\n";
            }
            bool read = ReadFile(pipe, emp, sizeof(Employee), &bytesRead, NULL);

            if (!read)
            {
                std::cout << "Невозможно прочесть данные\n";
            }
            employees[id - 1] = *emp;
            std::ofstream out(fileName);
            for (auto& emp : employees) {
                out << emp.id << ' ' << emp.name << ' ' << emp.hours << '\n';
            }
            out.close();

            int ack;
            ReadFile(pipe, &ack, sizeof(int), &bytesRead, NULL);
            if (ack == 1) {
                for (int i = 0; i < clientCount; ++i) {
                    ReleaseSemaphore(semaphores[id - 1], 1, NULL);
                }
            }
        }
        else if (op == 2) { // Read
            WaitForSingleObject(semaphores[id - 1], INFINITE);
            WriteFile(pipe, &employees[id - 1], sizeof(Employee), &bytesWritten, NULL);

            int ack;
            ReadFile(pipe, &ack, sizeof(int), &bytesRead, NULL);
            if (ack == 1) {
                ReleaseSemaphore(semaphores[id - 1], 1, NULL);
            }
        }
        else if (op == 3) {
            break;
        }
    }

    DisconnectNamedPipe(pipe);
    CloseHandle(pipe);
    return 0;
}
