#include "Server\Server.h"

int main() {
    std::string fileName;
    int employeeCount, clientCount;
    setlocale(LC_ALL, "ru");
    std::cout << "¬ведите им€ файла: ";
    std::cin >> fileName;
    std::cout << "¬ведите кол-во сотрудников: ";
    std::cin >> employeeCount;
    std::cout << "¬ведите кол-во клиентов: ";
    std::cin >> clientCount;

    Server server(fileName, employeeCount, clientCount);
    server.initializeEmployees();
    server.startClients();
    server.startPipeListeners();
    server.waitForClients();

    return 0;
}
