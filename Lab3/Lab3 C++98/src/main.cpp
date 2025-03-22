#include "marker.h"
#include <list>
#include <iostream>

int main() {
    int size, numThreads;
    std::cout << "Enter array size: ";
    std::cin >> size;

    int* array = new int[size];
    for (int i = 0; i < size; ++i) {
        array[i] = 0;
    }

    std::cout << "Enter number of marker threads: ";
    std::cin >> numThreads;

    HANDLE* threads = new HANDLE[numThreads];
    MarkerData* params = new MarkerData[numThreads];
    HANDLE startEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
    HANDLE* stopEvents = new HANDLE[numThreads];
    HANDLE* resumeEvents = new HANDLE[numThreads];
    HANDLE* exitEvents = new HANDLE[numThreads];

    std::list<int> active_threads;
    for (int i = 0; i < numThreads; i++) {
        active_threads.push_back(i);
    }

    CRITICAL_SECTION cs;
    InitializeCriticalSection(&cs);

    for (int i = 0; i < numThreads; i++) {
        stopEvents[i] = CreateEvent(NULL, TRUE, FALSE, NULL);
        resumeEvents[i] = CreateEvent(NULL, TRUE, FALSE, NULL);
        exitEvents[i] = CreateEvent(NULL, TRUE, FALSE, NULL);
        params[i].marker_index = i + 1;
        params[i].size = size;
        params[i].arr = array;
        params[i].startEvent = startEvent;
        params[i].stopEvent = stopEvents[i];
        params[i].resumeEvent = resumeEvents[i];
        params[i].exitEvent = exitEvents[i];
        params[i].cs = cs;
        threads[i] = CreateThread(NULL, 0, marker, &params[i], 0, NULL);
    }

    SetEvent(startEvent);
    while (!active_threads.empty()) {

        // wait for all markers to run
        WaitForMultipleObjects(numThreads, stopEvents, TRUE, INFINITE);

        // print array
        EnterCriticalSection(&cs);
        std::cout << "Array: ";
        for (int i = 0; i < size; i++) {
            std::cout << array[i] << " ";
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
        std::list<int>::iterator it = active_threads.begin();
        for (int i = 0; i < active_threads.size(); i++, ++it) {
            if (*it == threadToStop - 1) {
                active_threads.erase(it);
                break;
            }
        }

        // tell every active marker (NOT including the one to be exited) to resume
        // and reset their stop events
        it = active_threads.begin();
        for (int i = 0; i < active_threads.size(); i++, ++it) {
            ResetEvent(stopEvents[*it]);
            SetEvent(resumeEvents[*it]);
        }

        // wait for marker to be exited, then close its handle
        WaitForSingleObject(threads[threadToStop - 1], INFINITE);
        CloseHandle(threads[threadToStop - 1]);

        // print updated array
        EnterCriticalSection(&cs);
        std::cout << "Updated Array: ";
        for (int i = 0; i < size; i++) {
            std::cout << array[i] << " ";
        }
        std::cout << "\n";
        LeaveCriticalSection(&cs);
    }

    DeleteCriticalSection(&cs);
    delete[] array;
    delete[] threads;
    delete[] params;
    delete[] stopEvents;
    delete[] resumeEvents;
    delete[] exitEvents;

    return 0;
}
