#include <iostream>
#include "Server\Server.h"

int main() {
    setlocale(LC_ALL, "ru");
    std::string fileName;
    int empCount, clientCount;

    std::cout << "������� ��� �����: ";
    std::cin >> fileName;

    std::cout << "������� ���������� �����������: ";
    std::cin >> empCount;

    std::cout << "������� ���������� ��������: ";
    std::cin >> clientCount;

    Server server(fileName, empCount, clientCount);
    server.initializeEmployees();
    server.startClients();
    server.startPipeListeners();
    server.waitForClients();
    server.printFile();
    return 0;
}