#include "Client.h"
#include <iostream>
#include <stdexcept>
#include <limits>

const int sleepBeforeEndAccess = 1000;
Client::Client(int pipeIndex) {
    pipeName = "\\\\.\\pipe\\pipe" + std::to_string(pipeIndex);

    pipe = CreateFile(
        pipeName.data(),
        GENERIC_READ | GENERIC_WRITE,
        0, NULL, OPEN_EXISTING,
        0, NULL
    );

    if (pipe == INVALID_HANDLE_VALUE) {
        throw std::exception("Unable to connect pipe\n");
    }
}

Client::~Client() {
    if (pipe != INVALID_HANDLE_VALUE) {
        //CloseHandle(pipe);
    }
}

void Client::run() {
    int ans = 0;

    while (true) {
        std::cout << "\nChoose action:\n"
            << "1. Modify record\n"
            << "2. Read record\n"
            << "3. Exit this process\n";

        std::cin >> ans;

        if (ans == 1) {
            int id;
            std::cout << "Enter ID: ";
            std::cin >> id;
            modifyEmployee(id);
            
        }
        else if (ans == 2) {
            int id;
            std::cout << "Enter ID: ";
            std::cin >> id;
            readEmployee(id);
        }
        else if (ans == 3) {
            sendMessage(3);
            break;
        }
        else {
            std::cout << "Invalid input\n";
        }
        //Sleep(1000);
    }
}

void Client::modifyEmployee(int id) {
    sendMessage(id * 10 + 1);
    int status = getStatus();
    if (status == 1) {
        std::cout << "access confrimed\n";
    }
    else if (status == 0) {
        std::cout << "access denied\n";
        return;
    }
    else if (status == -1) {
        std::cout << "cannot get status\n";
        return;
    }
    else {
        std::cout << "client sent some piece of shit, idk what to do\n";
        return;
    }
    Employee emp = receiveEmployee();
    std::cout << "Current: ID: " << emp.id << " Name: " << emp.name << " Hours: " << emp.hours << "\n";

    std::cout << "Enter new name: ";
    std::cin >> emp.name;
    
    std::cout << "Enter hours: ";
    std::cin >> emp.hours;

    sendEmployee(emp);
    
    std::cout << "enter char to end access to the record\n";
    char c;
    std::cin >> c;
    if (this->pipeName[pipeName.size() - 1] == '1') {
        std::cerr << "Sleeping\n";
        Sleep(sleepBeforeEndAccess);
    }
    sendMessage(1); 
}

void Client::readEmployee(int id) {
    sendMessage(id * 10 + 2);
    int status = getStatus();
    if (status == 1) {
        std::cout << "access confrimed\n";
    }
    else if (status == 0) {
        std::cout << "access denied\n";
        return;
    }
    else if (status == -1) {
        std::cout << "cannot get status\n";
        return;
    }
    else {
        std::cout << "client sent some piece of shit, idk what to do\n";
        return;
    }
    Employee emp = receiveEmployee();
    std::cout << "Current ID:" << emp.id << "Name: " << emp.name << " Hours: " << emp.hours << "\n";

    std::cout << "enter char to end access to the record\n";
    char c;
    std::cin >> c;
    if (this->pipeName[pipeName.size() - 1] == '1') {
        std::cerr << "Sleeping\n";
        Sleep(sleepBeforeEndAccess);
    }
    sendMessage(1); 
}

void Client::sendMessage(int message) {
    DWORD written;
    if (!WriteFile(pipe, &message, sizeof(message), &written, NULL)) {
        std::cerr << "Error sending message\n";
    }

}


void Client::sendEmployee(Employee emp) {
    DWORD written;
    if (!WriteFile(pipe, &emp, sizeof(Employee), &written, NULL)) {
        std::cerr << "Error sending employee\n";
    }
}

Employee Client::receiveEmployee() {
    Employee emp;
    DWORD read;
    if (!ReadFile(pipe, &emp, sizeof(Employee), &read, NULL)) {
        std::cerr << "Error reading employee\n";
    }
    return emp;
}

int Client::getStatus() {
    int message;
    DWORD bytesRead = 0;
    bool success = ReadFile(pipe, &message, sizeof(message), &bytesRead, NULL);
    return success ? message : -1;
}