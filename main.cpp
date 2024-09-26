#include <thread>
#include "dynamicArray.h"

dynamicArray<int, mutexLock> data;

void Operate() {
	data[0] = 100;
	for (int i = 0; i < 50; ++i) {
		data.insert(i, i * 100);
	}
}
int main()
{
	using namespace std::chrono_literals;

	for (int i = 0; i < 100; ++i) {
		data.add(i);
	}
	std::thread t{ Operate };
	//std::this_thread::sleep_for(500ms);
	data.print();
	t.join();

	return 0;
}