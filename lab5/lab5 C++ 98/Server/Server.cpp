#include "Server.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <conio.h>

static Server* g_server = nullptr;

Server::Server(const std::string& fileName, int employeeCount, int clientCount)
    : fileName(fileName), clientCount(clientCount) {
    employees.resize(employeeCount);
    semaphores.resize(employeeCount);
    for (int i = 0; i < employeeCount; ++i) {
        semaphores[i] = CreateSemaphore(NULL, clientCount, clientCount, NULL);
    }
    g_server = this;
}

Server::~Server() {
    for (HANDLE sem : semaphores) CloseHandle(sem);

    for (HANDLE thread : threads) CloseHandle(thread);
}

void Server::initializeEmployees() {
    for (size_t i = 0; i < employees.size(); ++i) {
        employees[i].id = i + 1;
        std::cout << "Имя сотрудника " << i + 1 << ": ";
        std::cin >> employees[i].name;
        std::cout << "Часы: ";
        std::cin >> employees[i].hours;
    }
    saveToFile();
}

void Server::saveToFile() {
    std::ofstream out(fileName);
    for (const auto& emp : employees) {
        emp.serialize(out);
    }
}

void Server::loadFromFile() {
    std::ifstream in(fileName);
    for (auto& emp : employees) {
        emp.deserialize(in);
    }
}

void Server::startClients() {
    for (int i = 0; i < clientCount; ++i) {
        STARTUPINFO si;
        PROCESS_INFORMATION pi;
        ZeroMemory(&si, sizeof(STARTUPINFO));
        si.cb = sizeof(STARTUPINFO);

        std::string cmd = "Client.exe";
        if (!CreateProcess(NULL, cmd.data(), NULL, NULL, TRUE, CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi)) {
            std::cout << "unable to create process number " << i << '\n';
        }
        HANDLE evt = CreateEvent(NULL, FALSE, FALSE, "Process Started");
        clientEvents.push_back(evt);
        CloseHandle(pi.hProcess);
    }
    WaitForMultipleObjects(clientCount, clientEvents.data(), TRUE, INFINITE);
    std::cout << "Client processes started\n";
}

void Server::startPipeListeners() {
    for (int i = 0; i < clientCount; ++i) {
        HANDLE pipe = CreateNamedPipe(
            "\\\\.\\pipe\\pipeName", PIPE_ACCESS_DUPLEX,
            PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE | PIPE_WAIT,
            PIPE_UNLIMITED_INSTANCES, 0, 0, INFINITE, NULL);
        pipes.push_back(pipe);
        threads.push_back(CreateThread(NULL, 0, handleClient, static_cast<LPVOID>(pipe), 0, NULL));
    }
}

void Server::waitForClients() {
    std::cout << "start waiting for clients\n";
    WaitForMultipleObjects(this->clientCount, this->threads.data(), TRUE, INFINITE);
    std::cout << "all clients finished\n";

    loadFromFile();
    for (const auto& emp : employees) {
        std::cout << "ID: " << emp.id << "\tИмя: " << emp.name << "\tЧасы: " << emp.hours << "\n";
    }
    
}

DWORD WINAPI Server::handleClient(LPVOID param) {
    HANDLE pipe = static_cast<HANDLE>(param);
    DWORD bytesRead = 0;
    int msg;

    while (ReadFile(pipe, &msg, sizeof(msg), &bytesRead, NULL)) {
        int id = msg / 10 - 1;
        int op = msg % 10;

        if (op == 1) {
            for (int i = 0; i < g_server->clientCount; i++) {
                WaitForSingleObject(g_server->semaphores[id], INFINITE);
            }

            Employee emp = g_server->employees[id];
            DWORD written;
            WriteFile(pipe, &emp, sizeof(emp), &written, NULL);
            ReadFile(pipe, &g_server->employees[id], sizeof(emp), &written, NULL);
            g_server->saveToFile();
            int ack;
            ReadFile(pipe, &ack, sizeof(ack), &written, NULL);
            if (ack == 1) {
                for (int i = 0; i < g_server->clientCount; i++) {
                    ReleaseSemaphore(g_server->semaphores[id], 1, NULL);
                }
            }
        }
        else if (op == 2) {
            WaitForSingleObject(g_server->semaphores[id], INFINITE);
            Employee emp = g_server->employees[id];
            DWORD written;
            WriteFile(pipe, &emp, sizeof(emp), &written, NULL);
            int ack;
            ReadFile(pipe, &ack, sizeof(ack), &written, NULL);
            if (ack == 1) {
                ReleaseSemaphore(g_server->semaphores[id], 1, NULL);
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
