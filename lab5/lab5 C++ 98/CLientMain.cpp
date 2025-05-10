#include "Client\Client.h"
#include <iostream>
#include <fstream>

int main(int argc, char* argv[]) {
    setlocale(LC_ALL, "ru");

    if (argc < 2) {
        std::cerr << "Expected at least 1 argument\n";
        return 1;
    }

    int pipeIndex = std::stoi(argv[1]);
    std::streambuf* cout_buf  = nullptr;
    std::streambuf* cin_buf = nullptr;
    if (argc == 4) {
        std::string new_stdin_name = argv[2];
        std::string new_stdout_name = argv[3];
        std::ifstream in(new_stdin_name);
        std::ofstream out(new_stdout_name);
        cout_buf = std::cout.rdbuf();
        cin_buf = std::cin.rdbuf();
        std::cout.rdbuf(out.rdbuf());
        std::cin.rdbuf(in.rdbuf());
    }

    Client client(pipeIndex);
    client.run();

    if (argc == 4) {
        std::cout.flush();
        std::cout.rdbuf(cout_buf);
        std::cin.rdbuf(cin_buf);
    }
    return 0;
}
