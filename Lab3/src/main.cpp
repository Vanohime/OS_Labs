#include "marker.h"
#include <unordered_set>


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

    std::vector<HANDLE> threads(numThreads);
    std::vector<MarkerData> params(numThreads);
    HANDLE startEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
    std::vector<HANDLE> stopEvents(numThreads);
    std::vector<HANDLE> resumeEvents(numThreads);
    std::vector<HANDLE> exitEvents(numThreads);
    std::unordered_set<int> active_threads; //stores indeces of not exited threads
    for (int i = 0; i < numThreads; i++)
        active_threads.insert(i);
    CRITICAL_SECTION cs;
    InitializeCriticalSection(&cs);

    for (int i = 0; i < numThreads; i++) {
        stopEvents[i] = CreateEvent(NULL, TRUE, FALSE, NULL);
        resumeEvents[i] = CreateEvent(NULL, TRUE, FALSE, NULL);
        exitEvents[i] = CreateEvent(NULL, TRUE, FALSE, NULL);
        params[i] = { i + 1, size, array, startEvent, stopEvents[i], resumeEvents[i], exitEvents[i], cs};
        threads[i] = CreateThread(NULL, 0, marker, &params[i], 0, NULL);
    }

    SetEvent(startEvent);
    while (!active_threads.empty()) {

        //wait for all markers to run
        WaitForMultipleObjects(numThreads, stopEvents.data(), TRUE, INFINITE);
        //print array
        EnterCriticalSection(&cs);
        std::cout << "Array: ";
        for (int i = 0; i < size; i++) {
            std::cout << array[i] << " ";
        }
        std::cout << "\n";

        //ask user which marker to stop
        int threadToStop;
        std::cout << "Enter marker thread number to stop: ";
        std::cin >> threadToStop;
        LeaveCriticalSection(&cs);
        
        //tell it to exit
        SetEvent(exitEvents[threadToStop - 1]);

        //tell it to resume (and find out that it has to exit)
        SetEvent(resumeEvents[threadToStop - 1]);

        //delete it from active
        active_threads.erase(threadToStop - 1);

        //tell every active marker (NOT including the one to be exited) to resume
        //and reset their stop events
        for (int i : active_threads) {
            ResetEvent(stopEvents[i]);
            SetEvent(resumeEvents[i]);
        }

        //wait for marker to be exited, then close its handle
        WaitForSingleObject(threads[threadToStop - 1], INFINITE);
        CloseHandle(threads[threadToStop - 1]);

        //print updated array
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
    return 0;
}


