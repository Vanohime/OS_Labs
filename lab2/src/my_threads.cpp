#include "my_threads.h"

DWORD WINAPI min_max(LPVOID ArrData)
{
	ArrayData* data = static_cast<ArrayData*>(ArrData);

	if (data->arr.empty()) {
		std::cout << "������ ������, ���������� ����� ������� � ��������\n";
		return 1;
	}

	int min = data->arr[0], max = data->arr[0];

	for (int i = 0; i < data->arr.size(); i++) {
		int x = data->arr[i];
		if (min > x)
			min = x;
		if (max < x)
			max = x;
		Sleep(7);
	}

	data->min = min;
	data->max = max;

	std::cout << "����������� ������� ������ " << min << '\n';
	std::cout << "������������ ������� ������ " << max << '\n';

	return 0;

}


DWORD WINAPI average(LPVOID ArrData)
{
	ArrayData* data = static_cast<ArrayData*>(ArrData);

	if (data->arr.empty()) {
		std::cout << "������ ������, ���������� ����� �������\n";
		return 1;
	}

	long long sum = 0;
	for (int x : data->arr) {
		sum += x;
		Sleep(12);
	}

	data->average = (double)sum / data->arr.size();

	std::cout << "������� �������������� ��������� ������ " << data->average << '\n';

	return 0;

}
