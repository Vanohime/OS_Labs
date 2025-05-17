#include "Server.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <conio.h>
#include <tchar.h>




struct ThreadParam {
    HANDLE pipe;
    std::vector<Employee>* employees;
    std::vector<std::pair<HANDLE, HANDLE>>* accessEvents;
    std::string fileName;
    int clientCount;
};

Server::Server(const std::string& fileName, int employeeCount, int clientCount,
    std::vector<std::pair<std::string, std::string>> _file_pairs)
    : fileName(fileName), clientCount(clientCount), file_pairs(_file_pairs) {
    employees.resize(employeeCount);
    accessAllowedEvents.resize(employeeCount);

    for (int i = 0; i < employeeCount; ++i) {
        accessAllowedEvents[i] = { CreateEvent(NULL, FALSE, TRUE, "read event"),
            CreateEvent(NULL, FALSE, TRUE, "write event") };
    }

}

Server::~Server() {
    for (HANDLE h : clientEvents) CloseHandle(h);
    for (HANDLE h : processHandles) CloseHandle(h);
    for (std::pair<HANDLE, HANDLE> p : accessAllowedEvents) {
        CloseHandle(p.first); 
        CloseHandle(p.second);
    }
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
        if (file_pairs[i].first != "" && file_pairs[i].second != "") {
            cmd += " ";
            cmd += file_pairs[i].first;
            cmd += " ";
            cmd += file_pairs[i].second;
        }
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
        std::string pipeName = "\\\\.\\pipe\\pipe" + std::to_string(i);

        HANDLE pipe = CreateNamedPipe(
            pipeName.data(),
            PIPE_ACCESS_DUPLEX,
            PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE | PIPE_WAIT,
            PIPE_UNLIMITED_INSTANCES,
            0, 0, INFINITE, nullptr
        );

        pipes.push_back(pipe);

        ThreadParam* param = new ThreadParam{ pipe, &employees, &accessAllowedEvents, fileName, clientCount };

        std::thread t([param]() {
            handleClient(static_cast<void*>(param));
            delete param; 
            });

        threads.push_back(std::move(t));
    }
    
}

void Server::waitForClients() {
    for (auto& t : threads) {
        if (t.joinable()) t.join();
    }
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
    
    auto& accessEvents = *p->accessEvents;
    std::string fileName = p->fileName;
    int clientCount = p->clientCount;

    ConnectNamedPipe(pipe, NULL);

    DWORD bytesRead = 0, bytesWritten = 0;
    int message;

    while (true) {
        //get id and operation type
        ReadFile(pipe, &message, sizeof(message), &bytesRead, NULL);

        int id = message / 10;
        int op = message % 10;

        if (op == 1) { //modify

            int curStateRead = -1; 
            int curStateWrite = -1;
            HANDLE evs[2];
            if (id <= employees.size()) {
                evs[0] = accessEvents[id - 1].first;
                evs[1] = accessEvents[id - 1].second;
                //forbid both reading and writing
                curStateRead = WaitForSingleObject(evs[0], 0);
                curStateWrite = WaitForSingleObject(evs[1], 0);
            }
            if (curStateRead == WAIT_TIMEOUT || curStateWrite == WAIT_TIMEOUT ||
                curStateRead == WAIT_FAILED || curStateWrite == WAIT_FAILED || id > employees.size()) {
                int resp = 0;
                DWORD bytes;
                //send status 0
                std::cout << "access denied\n";
                bool send = WriteFile(pipe, &resp, sizeof(int), &bytes, NULL);
                if (!send) {
                    std::cout << "access denied, but client will not recieve response\n";
                }
                if (id >= employees.size()) continue;
                //set events if only one of them was set
                if (curStateRead == WAIT_OBJECT_0)
                    SetEvent(evs[0]);

                if(curStateWrite == WAIT_OBJECT_0)
                    SetEvent(evs[1]);

                continue;
            } else {
                int resp = 1;
                DWORD bytes;
                //send status 1
                std::cout << "access denied\n";
                bool send = WriteFile(pipe, &resp, sizeof(int), &bytes, NULL);
                if (!send) {
                    std::cout << "access confrimed, but client will not recieve a response\n";
                }
            }

            Employee* emp = new Employee;
            *emp = employees[id - 1];
            //send employee's data
            bool written = WriteFile(pipe, emp, sizeof(Employee), &bytesWritten, NULL);
            if (written)
            {
                std::cout << "Data is sent successfully\n";
            }
            else
            {
                std::cout << "Data is NOT sent successfully\n";
            }
            //get new data
            bool read = ReadFile(pipe, emp, sizeof(Employee), &bytesRead, NULL);

            if (!read)
            {
                std::cout << "Unable to read the data\n";
            }
            employees[id - 1] = *emp;
            std::ofstream out(fileName);

            //write new data into the file
            for (auto& emp : employees) {
                out << emp.id << ' ' << emp.name << ' ' << emp.hours << '\n';
            }
            out.close();

            int finish = -1;
            //receive access completion signal
            ReadFile(pipe, &finish, sizeof(int), &bytesRead, NULL);
            if (finish == 1) {
                //allow both reading and writing
                SetEvent(evs[0]);
                SetEvent(evs[1]);
            }
            
        }
        else if (op == 2) { // Read
            int curStateWrite = -1;
            //check if the reading is allowed
            int state = -1;
            if (id <= employees.size()) {
                state = WaitForSingleObject(accessEvents[id - 1].first, 0);
            }
            if (state == WAIT_TIMEOUT || state == WAIT_FAILED || id > employees.size()) {
                int resp = 0;
                DWORD bytes;
                //send status 0
                bool send = WriteFile(pipe, &resp, sizeof(int), &bytes, NULL);
                if (!send) {
                    std::cout << "reading forbidden, but client will not recieve a response\n";
                }
                continue;
            }
            else {
                //check if writing was allowed
                curStateWrite = WaitForSingleObject(accessEvents[id - 1].second, 0);
                //allow reading back
                SetEvent(accessEvents[id - 1].first);
                //forbid writing
                ResetEvent(accessEvents[id - 1].second);
                int resp = 1;
                DWORD bytes;
                //send status 1
                bool send = WriteFile(pipe, &resp, sizeof(int), &bytes, NULL);
                if (!send) {
                    std::cout << "reading allowed, but client will not recieve a response\n";
                }
            }
            
            //send employee's data
            WriteFile(pipe, &employees[id - 1], sizeof(Employee), &bytesWritten, NULL);

            int finish = -1;
            //receive access completion signal
            ReadFile(pipe, &finish, sizeof(int), &bytesRead, NULL);
            if (finish == 1) {
                //allow writing if it was allowed before
                if(curStateWrite == WAIT_OBJECT_0)
                    SetEvent(accessEvents[id - 1].second);
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
