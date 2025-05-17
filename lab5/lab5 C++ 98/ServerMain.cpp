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

    char c;
    std::cout << "Would you like to enter custom input/output file names? (y/n)\n";
    std::cin >> c;
    std::vector<std::pair<std::string, std::string>> file_pairs (clientCount);
    if (c == 'y' || c == 'Y') {

        for (int i = 0; i < clientCount; ++i) {
            std::string custom_in, custom_out;
            std::cout << "Enter input file name #" << (i + 1) << ":\n";
            std::cin >> custom_in;
            std::cout << "Enter output file name #" << (i + 1) << ":\n";
            std::cin >> custom_out;
            file_pairs[i] = { custom_in, custom_out };
        }
    }

    Server server(fileName, empCount, clientCount, file_pairs);
    server.initializeEmployees();
    server.startClients();
    server.startPipeListeners();
    server.waitForClients();
    server.printFile();
    return 0;
}