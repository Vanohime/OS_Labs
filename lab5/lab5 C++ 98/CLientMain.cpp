#include "Client\Client.h"
#include <iostream>

int main() {
    
    setlocale(LC_ALL, "ru");

    try {
        Client client;
        client.run();
    }
    catch (const std::exception& ex) {
        std::cerr << "Произошла ошибка: " << ex.what() << std::endl;
        return 1;
    }

    return 0;
}
