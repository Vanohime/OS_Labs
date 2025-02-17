#include <iostream>
#include <fstream>
#include <conio.h>
#include <vector>
#include <Employee.h>

int main(int argc, char* argv[]) {

    std::cout << "Reporter started\n";

    if (argc != 4) {
        throw std::exception("reporter.cpp recieved invalid number of arguments");
    }
    char* bin_file_name = argv[1];
    char* report_file_name = argv[2];
    int price_per_hour = std::atoi(argv[3]);

    std::fstream bf(bin_file_name, std::ios::binary | std::ios::in);
    std::fstream out_file(report_file_name, std::ios::out);

    out_file << "Report on binary file " << bin_file_name << std::endl;

    if (bf.is_open()) {
        employee emp;
        while (bf.read(reinterpret_cast<char*>(&emp), sizeof(employee))) {
            out_file << "ID: " << emp.num << " Name: " << emp.name << " Working hours: "
                << emp.hours << " Wage: " << emp.hours * price_per_hour << std::endl;
        }
        bf.close();
    }
    else {
        throw std::exception("Error! Binary file cannot be opened!");
    }
    std::cout << "reporter finished successfully\n";
    std::cout << "press any key to continue...\n";

    bf.close();
    out_file.close();

    _getch();
    return 0;
}
