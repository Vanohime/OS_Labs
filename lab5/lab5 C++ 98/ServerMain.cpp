#include <iostream>
#include "Server\Server.h"

int main() {
    setlocale(LC_ALL, "ru");
    std::string fileName;
    int empCount, clientCount;

    std::cout << "Введите имя файла: ";
    std::cin >> fileName;

    std::cout << "Введите количество сотрудников: ";
    std::cin >> empCount;

    std::cout << "Введите количество клиентов: ";
    std::cin >> clientCount;

    Server server(fileName, empCount, clientCount);
    server.initializeEmployees();
    server.startClients();
    server.startPipeListeners();
    server.waitForClients();
    server.printFile();
    return 0;
}