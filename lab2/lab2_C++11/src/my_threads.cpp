#include "my_threads.h"
#include <thread>
#include <chrono>

void min_max(ArrayData& data) {
    if (data.arr.empty()) {
        std::cout << "Empty array, can't find min and max\n";
        return;
    }

    int min = data.arr[0], max = data.arr[0];

    for (const auto& x : data.arr) {
        if (min > x) min = x;
        if (max < x) max = x;
        std::this_thread::sleep_for(std::chrono::milliseconds(7));
    }

    data.min = min;
    data.max = max;

    std::cout << "Min: " << min << '\n';
    std::cout << "Max: " << max << '\n';
}

void average(ArrayData& data) {
    if (data.arr.empty()) {
        std::cout << "Empty array, can't find average\n";
        return;
    }

    long long sum = 0;
    for (const auto& x : data.arr) {
        sum += x;
        std::this_thread::sleep_for(std::chrono::milliseconds(12));
    }

    data.average = static_cast<double>(sum) / data.arr.size();
    std::cout << "Average: " << data.average << '\n';
}
