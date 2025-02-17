#include <iostream>
#include <fstream>
#include <conio.h>
#include <Employee.h>

int main(int argc, char* argv[]) {

    std::cout << "Creator started\n";

    if (argc != 3) {
        throw std::exception("creator.cpp recieved invalid number of arguments");
    }

    char* bin_file_name = argv[1];
    int num_records = std::atoi(argv[2]);

    std::fstream bf(bin_file_name, std::ios::binary | std::ios::out);

    if (!bf) {
        throw std::exception("Unable to create file");
    }

    for (int i = 0; i < num_records; i++) {
        employee emp;
        std::string name;
        std::cout << "Enter ID\n";
        std::cin >> emp.num;
        std::cout << "Enter name\n";
        std::cin >> name;
        std::cout << "Enter number of hours\n";
        std::cin >> emp.hours;
        strncpy_s(emp.name, name.c_str(), sizeof(emp.name) - 1);
        bf.write(reinterpret_cast<char*>(&emp), sizeof(emp));

    }
    std::cout << "creator finished successfully\n";
    std::cout << "press any key to continue...\n";

    bf.close();
    _getch();
    return 0;
}
