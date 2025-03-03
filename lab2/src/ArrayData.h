#pragma once
#include <vector>

class ArrayData {
public:
	std::vector<int> arr;
	int min;
	int max;
	double average;
	ArrayData(std::vector<int> _arr) {
		arr.resize(_arr.size());
		for (int i = 0; i < _arr.size(); ++i) {
			arr[i] = _arr[i];
		}
		min = max = average = 0;
	}

};