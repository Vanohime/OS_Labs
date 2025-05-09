#include "Client\Client.h"
#include <iostream>

int main(int argc, char* argv[]) {
    setlocale(LC_ALL, "ru");

    if (argc < 2) {
        std::cerr << "Ожидается аргумент: номер клиента\n";
        return 1;
    }

    int pipeIndex = std::stoi(argv[1]);

    try {
        Client client(pipeIndex);
        client.run();

    }
    catch (...) {
        std::cerr << "Ошибка клиента";
        return 1;
    }

    return 0;
}
