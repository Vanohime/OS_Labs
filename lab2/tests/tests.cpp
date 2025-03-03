#include "../src/my_threads.h"
#include <cassert>
#include <iostream>

void test1() {
	ArrayData* data = new ArrayData({ 1, 2, 3, 4 });
	min_max((void*)data);
	average((void*)data);
	assert(data->max == 4 && data->min == 1 && data->average == 2.5);
	delete data;
}

void test2() {
	ArrayData* data = new ArrayData({1233});
	min_max((void*)data);
	average((void*)data);
	assert(data->max == 1233 && data->min == 1233 && data->average == 1233.0);
	delete data;
}

void test3() {
	ArrayData* data = new ArrayData({});
	DWORD x1 = min_max((void*)data);
	DWORD x2 = average((void*)data);
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
