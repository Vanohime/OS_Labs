#include "Receiver.h"
#include <iostream>
#include <cstdlib>
#include <conio.h>

Receiver::Receiver() {}

Receiver::Receiver(const std::string& file_name, int notes, int senders)
    : bin_file_name(file_name), amount_of_notes(notes), amount_of_senders(senders) {}

int Receiver::get_user_choice() {
    std::string input;
    int ans;

    while (true) {
        std::cout << "Что вы желаете cделать?\n\t1 - Вывести сообщение.\n\t0 - Завершить выполнение процесса.\n";
        std::cin >> input;
        ans = std::atoi(input.c_str());

        if (input == "0" || input == "1") {
            break;
        }
        else {
            std::cout << "Ошибка: введите 0 или 1.\n";
        }
    }
    return ans;
}

void Receiver::read_input() {
    std::cout << "Введите имя бинарного файла:\n";
    std::cin >> bin_file_name;

    std::cout << "Введите количество записей:\n";
    std::cin >> amount_of_notes;

    std::cout << "Введите количество процессов Sender:\n";
    std::cin >> amount_of_senders;

    std::cin.clear();
    
}

void Receiver::init_synchronization() {
    hInputSemaphore = CreateSemaphore(NULL, 0, amount_of_notes, "Input Semaphore started");
    hOutputSemaphore = CreateSemaphore(NULL, amount_of_notes, amount_of_notes, "Output Semaphore started");

    hEventStarted.resize(amount_of_senders);
    hProcesses.resize(amount_of_senders);
}

bool Receiver::create_sender_processes() {

    STARTUPINFO si;
    PROCESS_INFORMATION pi;
    for (int i = 0; i < amount_of_senders; i++)
    {
        std::string sender_cmd = "Sender.exe " + bin_file_name;

        ZeroMemory(&si, sizeof(STARTUPINFO));
        si.cb = sizeof(STARTUPINFO);
        if (!CreateProcess(NULL, sender_cmd.data(), NULL, NULL, TRUE, CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi)) {
            std::cout << "Процесс Sender не запущен.\n";
            return false;
        }   

        hEventStarted[i] = CreateEvent(NULL, FALSE, FALSE, "Process Started");

        hProcesses[i] = pi.hProcess;
    }

    WaitForMultipleObjects(amount_of_senders, hEventStarted.data(), TRUE, INFINITE);
    return true;
}


void Receiver::main_loop() {
    int ans = get_user_choice();
    std::ifstream file(bin_file_name, std::ios::binary);
    while (true)
    {
        if (ans == 0)
        {
            std::cout << "Завершение программы...";
            break;
        }
        else if (ans == 1)
        {
            char message[20];

            WaitForSingleObject(hInputSemaphore, INFINITE);

            file.read(message, sizeof(message));
            std::cout << "Полученное сообщение: " << message << '\n';

            ReleaseSemaphore(hOutputSemaphore, 1, NULL);

            ans = get_user_choice();
        }
    }
    file.clear();
    file.close();
}

void Receiver::cleanup() {
    if (hInputSemaphore) CloseHandle(hInputSemaphore);
    if (hOutputSemaphore) CloseHandle(hOutputSemaphore);

    for (int i = 0; i < amount_of_senders; ++i) {
        CloseHandle(hEventStarted[i]);
        TerminateProcess(hProcesses[i], 0);
        CloseHandle(hProcesses[i]);
    }
}

void Receiver::start() {
    setlocale(LC_ALL, "ru");

    read_input();
    init_synchronization();

    if (!create_sender_processes()) {
        std::cout << "unable to create sender processes, abort\n";
        return;
    }

    main_loop();
    cleanup();

    std::cout << "\n";
    system("pause");
}

std::string Receiver::get_filename() {
    return this->bin_file_name;
}
