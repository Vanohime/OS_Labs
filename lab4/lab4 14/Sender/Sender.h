#pragma once
#include <string>
#include <windows.h>
#include <fstream>
#define private public

class Sender {
private:
    std::string file_name;
    std::fstream file;

    HANDLE hStartEvent = nullptr;
    HANDLE hInputSemaphore = nullptr;
    HANDLE hOutputSemaphore = nullptr;

    int get_user_choice();
    bool open_synchronization_objects();
    void send_loop();

public:
    std::string get_filename();
    Sender(const std::string& file_name = "");
    void start();
};
