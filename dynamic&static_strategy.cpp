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

	DynamicStrategy::dynamicArray<int> data2( std::make_unique<DynamicStrategy::mutexLock>() );
	data2.add(5);
	data2.print();

	for (int i = 0; i < 100; ++i) {
		data.add(i);
	}
	std::thread t{ Operate };
	std::this_thread::sleep_for(500ms);

	data.setPrinter([](const auto& data) {
		for (const auto& element : data) {
			std::cout << element << ' ';
		}
		std::cout << std::endl;
	});
	data.print();
	t.join();

	return 0;
}
