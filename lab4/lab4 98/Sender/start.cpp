#include "Sender.h"
#include <iostream>

int main(int argc, char* argv[]) {
    setlocale(LC_ALL, "ru");
    if (argc < 2) {
        std::cerr << "Не указано имя файла.\n";
        return 1;
    }

    Sender sender(argv[1]);
    sender.start();
    system("pause");

    return 0;
}
