#include "marker.h"

DWORD WINAPI marker(LPVOID marker_data) {
	MarkerData* data = static_cast<MarkerData*>(marker_data);

	WaitForSingleObject(data->startEvent, INFINITE);

	srand(data->marker_index);
	std::vector<int> marked_ind;
	int num_marked = 0;

	while (true) {

		int x = rand() % data->size;

		EnterCriticalSection(&(data->cs));
		if (data->arr[x] == 0) {

			Sleep(5);
			data->arr[x] = data->marker_index;
			Sleep(5);
			num_marked++;
			marked_ind.push_back(x);

			LeaveCriticalSection(&(data->cs));
		}
		else {
			std::cout << "I am makrer number " << data->marker_index << '\n'
				<< "I marked " << num_marked << " elements\n"
				<< "I cannot mark element with index " << x << '\n';

			/*for (int i = 0; i < data->size; ++i) {
				std::cout << data->arr[i] << ' ';
			}*/

			std::cout << '\n';
			SetEvent(data->stopEvent);

			WaitForSingleObject(data->resumeEvent, INFINITE);
			ResetEvent(data->resumeEvent);

			DWORD res = WaitForSingleObject(data->exitEvent, 0);

			if (res == WAIT_OBJECT_0) {
				for (int ind : marked_ind) {
					data->arr[ind] = 0;
				}
				LeaveCriticalSection(&(data->cs));
				return 0;
			}
			LeaveCriticalSection(&(data->cs));
		}

	}
	return 0;
}
