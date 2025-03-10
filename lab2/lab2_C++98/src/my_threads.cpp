#include "my_threads.h"

DWORD WINAPI min_max(LPVOID ArrData)
{
    ArrayData* data = static_cast<ArrayData*>(ArrData);

    if (data->arr.size() == 0) { 
        std::cout << "Empty array, can't find min and max\n";
        return 1;
    }

    int min = data->arr[0], max = data->arr[0];

    for (size_t i = 0; i < data->arr.size(); i++) { 
        int x = data->arr[i];
        if (min > x)
            min = x;
        if (max < x)
            max = x;
        Sleep(7);
    }

    data->min = min;
    data->max = max;

    std::cout << "Min: " << min << '\n';
    std::cout << "Max: " << max << '\n';

    return 0;
}

DWORD WINAPI average(LPVOID ArrData)
{
    ArrayData* data = static_cast<ArrayData*>(ArrData);

    if (data->arr.size() == 0) {
        std::cout << "Empty array, can't find average\n";
        return 1;
    }

    long sum = 0; 
    for (size_t i = 0; i < data->arr.size(); i++) {
        sum += data->arr[i];
        Sleep(12);
    }

    data->average = static_cast<double>(sum) / data->arr.size();

    std::cout << "Average: " << data->average << '\n';

    return 0;
}
