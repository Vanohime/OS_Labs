#include "../src/my_threads.h"
#include <cassert>
#include <iostream>
#include <vector>

void test1() {
    std::vector<int> test_data;
    test_data.push_back(1);
    test_data.push_back(2);
    test_data.push_back(3);
    test_data.push_back(4);

    ArrayData* data = new ArrayData(test_data);
    min_max(static_cast<void*>(data));
    average(static_cast<void*>(data));

    assert(data->max == 4 && data->min == 1 && data->average == 2.5);
    delete data;
}

void test2() {
    std::vector<int> test_data;
    test_data.push_back(1233);

    ArrayData* data = new ArrayData(test_data);
    min_max(static_cast<void*>(data));
    average(static_cast<void*>(data));

    assert(data->max == 1233 && data->min == 1233 && data->average == 1233.0);
    delete data;
}

void test3() {
    std::vector<int> test_data; 

    ArrayData* data = new ArrayData(test_data);
    DWORD x1 = min_max(static_cast<void*>(data));
    DWORD x2 = average(static_cast<void*>(data));

    assert(x1 == 1 && x2 == 1);
    delete data;
}

int main() {
    test1();
    test2();
    test3();

    std::cout << "All tests ran successfully\n";

    return 0;
}
