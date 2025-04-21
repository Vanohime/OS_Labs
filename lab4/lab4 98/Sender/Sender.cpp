#include "Sender.h"
#include <iostream>
#include <cstdlib>

Sender::Sender(const std::string& file_name)
    : file_name(file_name) {}

int Sender::get_user_choice() {
    std::string input;
    int ans;

    while (true) {
        std::cout << "Что вы желаете cделать?\n\t1 - Ввести сообщение.\n\t0 - Завершить выполнение процесса.\n";
        std::cin >> input;
        ans = std::atoi(input.c_str());

        if (input == "0" || input == "1") {
            return ans;
        }
        else {
            std::cout << "Ошибка: введите 0 или 1.\n";
        }
    }
}

bool Sender::open_synchronization_objects() {
    hStartEvent = OpenEvent(EVENT_MODIFY_STATE, FALSE, "Process Started");
    hInputSemaphore = OpenSemaphore(EVENT_ALL_ACCESS, FALSE, "Input Semaphore started");
    hOutputSemaphore = OpenSemaphore(EVENT_ALL_ACCESS, FALSE, "Output Semaphore started");

    if (!hStartEvent || !hInputSemaphore || !hOutputSemaphore) {
        std::cerr << "Не удалось открыть объекты синхронизации.\n";
        return false;
    }

    SetEvent(hStartEvent);
    return true;
}

void Sender::send_loop() {
    int ans = get_user_choice();

    while (true) {
        if (ans == 1) {
            file.open(file_name, std::ios::binary | std::ios::out | std::ios::app);

            std::string input_message;
            char message[20] = { 0 };

            std::cout << "Введите сообщение:\n";
            std::cin.ignore();
            std::getline(std::cin, input_message);
            strncpy_s(message, input_message.c_str(), sizeof(message) - 1);

            WaitForSingleObject(hOutputSemaphore, INFINITE);

            file.write(message, sizeof(message));

            ReleaseSemaphore(hInputSemaphore, 1, NULL);

            file.close();
            ans = get_user_choice();
        }
        else if (ans == 0) {
            std::cout << "Завершение процесса...\n";
            break;
        }
    }
}


void Sender::start() {
    setlocale(LC_ALL, "ru");

    if (!open_synchronization_objects()) {
        std::cerr << "Sender is unable to open synchronization objects\n";
        return;
    }
    
    send_loop();
}

std::string Sender::get_filename() {
    return this->file_name;
}
