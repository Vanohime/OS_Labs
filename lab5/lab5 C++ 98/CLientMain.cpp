#include "Client\Client.h"
#include <iostream>

int main(int argc, char* argv[]) {
    setlocale(LC_ALL, "ru");

    if (argc < 2) {
        std::cerr << "Expected at least 1 argument\n";
        return 1;
    }

    int pipeIndex = std::stoi(argv[1]);


    Client client(pipeIndex);
    client.run();

    return 0;
}
