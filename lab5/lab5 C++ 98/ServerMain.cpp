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

    std::cout << "Press Y if you want to set custom I/O streams for the client process. Otherwise press N\n";
    char c;
    std::cin >> c;
    std::string custom_in = "";
    std::string custom_out = "";
    if (c == 'y' || c == 'Y') {
        std::cout << "Enter input file name\n";
        std::cin >> custom_in;
        std::cout << "Enter output file name\n";
        std::cin >> custom_out;
    }

    Server server(fileName, empCount, clientCount, custom_in, custom_out);
    server.initializeEmployees();
    server.startClients();
    server.startPipeListeners();
    server.waitForClients();
    server.printFile();
    return 0;
}