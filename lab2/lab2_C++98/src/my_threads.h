#ifndef MY_THREADS_H
#define MY_THREADS_H

#include <iostream>
#include <vector>
#include <Windows.h>
#include "ArrayData.h"

DWORD WINAPI min_max(LPVOID ArrData);
DWORD WINAPI average(LPVOID ArrData);

#endif // MY_THREADS_H
