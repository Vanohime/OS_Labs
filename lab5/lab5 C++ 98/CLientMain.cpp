#include "Client\Client.h"
#include <iostream>
#include <fstream>

const int sleepBeforeStart = 10;

int main(int argc, char* argv[]) {
    setlocale(LC_ALL, "ru");

    if (argc < 2) {
        std::cerr << "Expected at least 1 argument\n";
        return 1;
    }

    int pipeIndex = std::stoi(argv[1]);

    std::streambuf* cout_buf = nullptr;
    std::streambuf* cin_buf = nullptr;
    std::ifstream in;
    std::ofstream out;

    if (argc == 4) {
        std::string new_stdin_name = argv[2];
        std::string new_stdout_name = argv[3];
        in.open(new_stdin_name, std::ios::in);
        out.open(new_stdout_name, std::ios::out);

        if (!in.is_open() || !out.is_open()) {
            std::cerr << "Failed to open input or output file.\n";
            return 2;
        }
        cout_buf = std::cout.rdbuf();
        cin_buf = std::cin.rdbuf();
        std::cout.rdbuf(out.rdbuf());
        std::cin.rdbuf(in.rdbuf());
    }

    if (pipeIndex == 0) {
        Sleep(sleepBeforeStart);
    }
    Client client(pipeIndex);
    client.run();

    if (argc == 4) {
        std::cout.rdbuf(cout_buf);
        std::cin.rdbuf(cin_buf);
    }
    return 0;
}
