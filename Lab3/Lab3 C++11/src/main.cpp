#include "marker.h"
#include <unordered_set>
#include <vector>
#include <iostream>

int main() {
    int size, numThreads;
    std::cout << "Enter array size: ";
    std::cin >> size;

    std::vector<int> array(size, 0);  // Use vector instead of raw array

    std::cout << "Enter number of marker threads: ";
    std::cin >> numThreads;

    std::vector<HANDLE> threads(numThreads);
    std::vector<MarkerData> params(numThreads);
    HANDLE startEvent = CreateEvent(nullptr, TRUE, FALSE, nullptr);
    std::vector<HANDLE> stopEvents(numThreads);
    std::vector<HANDLE> resumeEvents(numThreads);
    std::vector<HANDLE> exitEvents(numThreads);

    std::unordered_set<int> active_threads;
    for (int i = 0; i < numThreads; i++)
        active_threads.insert(i);

    CRITICAL_SECTION cs;
    InitializeCriticalSection(&cs);

    for (int i = 0; i < numThreads; i++) {
        stopEvents[i] = CreateEvent(nullptr, TRUE, FALSE, nullptr);
        resumeEvents[i] = CreateEvent(nullptr, TRUE, FALSE, nullptr);
        exitEvents[i] = CreateEvent(nullptr, TRUE, FALSE, nullptr);

        params[i] = { i + 1, size, array.data(), startEvent, stopEvents[i], resumeEvents[i], exitEvents[i], cs };
        threads[i] = CreateThread(nullptr, 0, marker, &params[i], 0, nullptr);
    }

    SetEvent(startEvent);
    while (!active_threads.empty()) {

        // wait for all markers to run
        WaitForMultipleObjects(numThreads, stopEvents.data(), TRUE, INFINITE);

        // print array
        EnterCriticalSection(&cs);
        std::cout << "Array: ";
        for (const auto& elem : array) {
            std::cout << elem << " ";
        }
        std::cout << "\n";
        LeaveCriticalSection(&cs);

        // ask user which marker to stop
        int threadToStop;
        std::cout << "Enter marker thread number to stop: ";
        std::cin >> threadToStop;

        // tell it to exit
        SetEvent(exitEvents[threadToStop - 1]);

        // tell it to resume (and find out that it has to exit)
        SetEvent(resumeEvents[threadToStop - 1]);

        // delete it from active
        active_threads.erase(threadToStop - 1);

        // tell every active marker (NOT including the one to be exited) to resume
        // and reset their stop events
        for (int i : active_threads) {
            ResetEvent(stopEvents[i]);
            SetEvent(resumeEvents[i]);
        }

        // wait for marker to be exited, then close its handle
        WaitForSingleObject(threads[threadToStop - 1], INFINITE);
        CloseHandle(threads[threadToStop - 1]);

        // print updated array
        EnterCriticalSection(&cs);
        std::cout << "Updated Array: ";
        for (const auto& elem : array) {
            std::cout << elem << " ";
        }
        std::cout << "\n";
        LeaveCriticalSection(&cs);
    }

    DeleteCriticalSection(&cs);
    return 0;
}
