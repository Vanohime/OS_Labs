#include <iostream>
#include <vector>
#include <Windows.h>
#include "my_threads.h"
#include "ArrayData.h"

void start_thread(DWORD(*thread)(LPVOID), void* data) {
    HANDLE thread_handle;

    thread_handle = CreateThread(NULL, 0, thread, data, 0, NULL);

    if (thread_handle == NULL) {
        std::cout << GetLastError();
        return;
    }

    WaitForSingleObject(thread_handle, INFINITE);
    CloseHandle(thread_handle);
}

int main() {
    setlocale(LC_ALL, "ru");

    std::vector<int> arr;
    int n;

    std::cout << "Введите размерность массива\n";
    std::cin >> n;

    arr.resize(n);

    std::cout << "Введите элементы массива\n";
    for (int i = 0; i < n; i++) {
        std::cin >> arr[i];
    }

    ArrayData* data = new ArrayData(arr);

    start_thread(min_max, (void*)data);
    start_thread(average, (void*)data);

    delete data; 

    return 0;
}
