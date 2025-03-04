#ifndef ARRAYDATA_H
#define ARRAYDATA_H

#include <vector>
#include <cstddef>

class ArrayData {
public:
    std::vector<int> arr;
    int min;
    int max;
    double average;
    ArrayData() {
        min = max = 0;
        average = 0;
    }
    ArrayData(const std::vector<int>& _arr) {
        arr.resize(_arr.size());
        for (size_t i = 0; i < _arr.size(); ++i) {
            arr[i] = _arr[i];
        }
        min = max = 0;
        average = 0.0;
    }
};

#endif // ARRAYDATA_H
