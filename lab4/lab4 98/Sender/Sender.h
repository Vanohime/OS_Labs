#pragma once
#include <string>
#include <windows.h>
#include <fstream>

class Sender {
private:
    std::string file_name;
    std::fstream file;

    HANDLE hStartEvent = nullptr;
    HANDLE hInputSemaphore = nullptr;
    HANDLE hOutputSemaphore = nullptr;

    int get_user_choice();
    void open_synchronization_objects();
    void send_loop();

public:
    Sender(const std::string& file_name);
    void start();
};
