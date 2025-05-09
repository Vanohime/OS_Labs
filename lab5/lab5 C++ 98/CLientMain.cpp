#include "Client\Client.h"
#include <iostream>

int main(int argc, char* argv[]) {
    setlocale(LC_ALL, "ru");

    if (argc < 2) {
        std::cerr << "��������� ��������: ����� �������\n";
        return 1;
    }

    int pipeIndex = std::stoi(argv[1]);

    try {
        Client client(pipeIndex);
        client.run();

    }
    catch (...) {
        std::cerr << "������ �������";
        return 1;
    }

    return 0;
}
