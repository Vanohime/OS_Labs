#include <iostream>
#include <vector>
#include <memory>
#include <thread>
#include "my_threads.h"
#include "ArrayData.h"

void start_thread(void(*thread)(ArrayData&), ArrayData& data) {
    std::thread t(thread, std::ref(data));
    t.join();
}

int main() {
    setlocale(LC_ALL, "ru");

    int n;
    std::cout << "Введите размерность массива\n";
    std::cin >> n;

    std::vector<int> arr(n);
    std::cout << "Введите элементы массива\n";
    for (auto& x : arr) {
        std::cin >> x;
    }

    auto data = std::make_unique<ArrayData>(arr);

    start_thread(min_max, *data);
    start_thread(average, *data);

    return 0;
}
