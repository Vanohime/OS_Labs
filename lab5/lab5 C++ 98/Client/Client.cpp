#include "Client.h"
#include <iostream>

Client::Client() {
    HANDLE hEvent = OpenEvent(EVENT_MODIFY_STATE, FALSE, "Process Started");
    SetEvent(hEvent);
    pipe = CreateFile("\\\\.\\pipe\\pipeName", GENERIC_WRITE | GENERIC_READ, 0, NULL, OPEN_EXISTING, 0, NULL);
}

Client::~Client() {
    DisconnectNamedPipe(pipe);
    CloseHandle(pipe);
}

void Client::run() {
    int ans;
    while (true) {
        std::cout << "�������� �����\n1. �������������� ������\n2. ��������� ������\n3. ��������� �������\n";
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
    Employee emp = receiveEmployee();

    std::cout << "ID: " << emp.id << "\n���: " << emp.name << "\n����: " << emp.hours << "\n";

    std::cout << "������� ����� ���: ";
    std::cin >> emp.name;
    std::cout << "������� ����: ";
    std::cin >> emp.hours;

    sendEmployee(emp);
    sendMessage(1);
}

void Client::readEmployee(int id) {
    sendMessage(id * 10 + 2);
    Employee emp = receiveEmployee();

    std::cout << "ID: " << emp.id << "\n���: " << emp.name << "\n����: " << emp.hours << "\n";

    sendMessage(1);
}

void Client::sendMessage(int message) {
    DWORD written;
    WriteFile(pipe, &message, sizeof(message), &written, NULL);
}

void Client::sendEmployee(const Employee& emp) {
    DWORD written;
    WriteFile(pipe, &emp, sizeof(emp), &written, NULL);
}

Employee Client::receiveEmployee() {
    Employee emp;
    DWORD read;
    ReadFile(pipe, &emp, sizeof(emp), &read, NULL);
    return emp;
}
