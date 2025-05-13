#include <..\cxxtest/TestSuite.h>
#include <fstream>
#include <iostream>
#include <string>
#include <cstdio>
#include <sstream>
#include "..\Server\Server.h"

void startServer() {
    
    std::string fileName;
    int empCount, clientCount;


    std::cin >> fileName;

    std::cin >> empCount;

    
    std::cin >> clientCount;

    char c;
    std::cout << "Would you like to enter custom input/output file names? (y/n)\n";
    std::cin >> c;
    std::vector<std::pair<std::string, std::string>> file_pairs(clientCount);
    if (c == 'y' || c == 'Y') {

        for (int i = 0; i < clientCount; ++i) {
            std::string custom_in, custom_out;
            std::cout << "Enter input file name #" << (i + 1) << ":\n";
            std::cin >> custom_in;
            std::cout << "Enter output file name #" << (i + 1) << ":\n";
            std::cin >> custom_out;
            file_pairs[i] = { custom_in, custom_out };
        }
    }

    Server server(fileName, empCount, clientCount, file_pairs);
    server.initializeEmployees();
    server.startClients();
    server.startPipeListeners();
    server.waitForClients();
    server.printFile();
    std::cout << "Everything is fine";
}

std::string getFileContent(const std::string& filename, const std::string& sep = "") {
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Unable to open file : " + filename);
    }

    std::ostringstream contentStream;
    std::string line;
    bool firstLine = true;

    while (std::getline(file, line)) {
        if (!firstLine) {
            contentStream << sep;
        }
        contentStream << line;
        firstLine = false;
    }

    return contentStream.str();
}


class ServerTestSuite : public CxxTest::TestSuite {
public:
    void redirectIO(const std::string& inputFile, const std::string& outputFile) {
        
        static std::ifstream inFile(inputFile);
        std::cin.rdbuf(inFile.rdbuf());

        
        static std::ofstream outFile(outputFile);
        std::cout.rdbuf(outFile.rdbuf());
    }

    void testServerScenario1() {
        redirectIO("test1_server_in.txt", "test1_server_out.txt");

        
        startServer();

        
        std::string actual1 = getFileContent("test1_cl0_true_out.txt");
        std::string actual2 = getFileContent("test1_cl1_true_out.txt");

        
        std::string expected1 = getFileContent("test1_cl0_out.txt");
        std::string expected2 = getFileContent("test1_cl2_out.txt");


        TS_ASSERT_EQUALS(actual1, expected1);
        TS_ASSERT_EQUALS(actual2, expected2);
    }

    void testServerScenario2() {
        redirectIO("test2_server_in.txt", "test2_server_out.txt");

        
        startServer();

        
        std::string actual1 = getFileContent("test2_cl0_true_out.txt");
        std::string actual2 = getFileContent("test2_cl1_true_out.txt");

        
        std::string expected1 = getFileContent("test2_cl0_out.txt");
        std::string expected2 = getFileContent("test2_cl2_out.txt");

        TS_ASSERT_EQUALS(actual1, expected1);
        TS_ASSERT_EQUALS(actual2, expected2);
    }
};
