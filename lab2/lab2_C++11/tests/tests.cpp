#include "../src/my_threads.h"
#include <cassert>
#include <iostream>
#include <vector>
#include <memory>

void test1() {
    auto data = std::make_unique<ArrayData>(std::vector<int>{1, 2, 3, 4});
    min_max(*data);
    average(*data);
    assert(data->max == 4 && data->min == 1 && data->average == 2.5);
}

void test2() {
    auto data = std::make_unique<ArrayData>(std::vector<int>{1233});
    min_max(*data);
    average(*data);
    assert(data->max == 1233 && data->min == 1233 && data->average == 1233.0);
}

void test3() {
    auto data = std::make_unique<ArrayData>(std::vector<int>{});
    min_max(*data);
    average(*data);
}

int main() {
    test1();
    test2();
    test3();

    std::cout << "All tests ran successfully\n";
    return 0;
}
