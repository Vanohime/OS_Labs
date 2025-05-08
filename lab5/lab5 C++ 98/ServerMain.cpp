#include "Server\Server.h"

int main() {
    std::string fileName;
    int employeeCount, clientCount;
    setlocale(LC_ALL, "ru");
    std::cout << "������� ��� �����: ";
    std::cin >> fileName;
    std::cout << "������� ���-�� �����������: ";
    std::cin >> employeeCount;
    std::cout << "������� ���-�� ��������: ";
    std::cin >> clientCount;

    Server server(fileName, employeeCount, clientCount);
    server.initializeEmployees();
    server.startClients();
    server.startPipeListeners();
    server.waitForClients();

    return 0;
}
