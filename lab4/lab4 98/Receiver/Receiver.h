#pragma once
#include <string>
#include <vector>
#include <fstream>
#include <windows.h>
#define private public

class Receiver {
private:
    std::string bin_file_name;
    int amount_of_notes = 0;
    int amount_of_senders = 0;

    HANDLE hInputSemaphore = nullptr;
    HANDLE hOutputSemaphore = nullptr;
    std::vector<HANDLE> hEventStarted;
    std::vector<HANDLE> hProcesses;

    int get_user_choice();
    void read_input();
    void init_synchronization();
    bool create_sender_processes();
    void main_loop();
    void cleanup();

public:

    std::string get_filename();
    Receiver();

    Receiver(const std::string& file_name, int notes, int senders);

    void start();
};
