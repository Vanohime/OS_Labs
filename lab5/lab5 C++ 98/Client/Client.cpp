#include "Client.h"
#include <iostream>
#include <stdexcept>
#include <limits>


Client::Client(int pipeIndex) {
    pipeName = "\\\\.\\pipe\\pipe" + std::to_string(pipeIndex);

    pipe = CreateFile(
        pipeName.data(),
        GENERIC_READ | GENERIC_WRITE,
        0, NULL, OPEN_EXISTING,
        0, NULL
    );

    if (pipe == INVALID_HANDLE_VALUE) {
        throw std::exception("�� ������� ������������ � ������");
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
        std::cout << "\n�������� �����:\n"
            << "1. �������������� ������\n"
            << "2. ��������� ������\n"
            << "3. ��������� �������\n";

        std::cin >> ans;

        if (ans == 1) {
            int id;
            std::cout << "������� ID ����������: ";
            std::cin >> id;
            modifyEmployee(id);
            
        }
        else if (ans == 2) {
            int id;
            std::cout << "������� ID ����������: ";
            std::cin >> id;
            readEmployee(id);
        }
        else if (ans == 3) {
            sendMessage(3);
            break;
        }
        else {
            std::cout << "������������ ����\n";
        }
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
    std::cout << "������� ������: ID:" << emp.id << "Name: " << emp.name << " Hours: " << emp.hours << "\n";

    std::cout << "������� ����� ���: ";
    std::cin >> emp.name;
    std::cout << "������� ����: ";
    std::cin >> emp.hours;

    sendEmployee(emp);
    
    std::cout << "enter char to end access to the record\n";
    char c;
    std::cin >> c;
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
    sendMessage(1); 
}

void Client::sendMessage(int message) {
    DWORD written;
    if (!WriteFile(pipe, &message, sizeof(message), &written, NULL)) {
        std::cerr << "������ �������� ���������. ���: " << GetLastError() << "\n";
        throw std::exception("������ �������� ���������");
    }

}


void Client::sendEmployee(Employee emp) {
    DWORD written;
    if (!WriteFile(pipe, &emp, sizeof(Employee), &written, NULL)) {
        throw std::runtime_error("������ �������� ����������");
    }
}

Employee Client::receiveEmployee() {
    Employee emp;
    DWORD read;
    if (!ReadFile(pipe, &emp, sizeof(Employee), &read, NULL)) {
        std::cerr << "������ ������ ����������. ���: " << GetLastError() << "\n";
        throw std::runtime_error("������ ������ ����������");
    }
    return emp;
}

int Client::getStatus() {
    int message;
    DWORD bytesRead = 0;
    bool success = ReadFile(pipe, &message, sizeof(message), &bytesRead, NULL);
    return success ? message : -1;
}