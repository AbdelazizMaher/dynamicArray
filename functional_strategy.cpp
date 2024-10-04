#include <Windows.h>
#include <sstream>
#include <fstream>
#include "dynamicArray.h"


void winPrinter(const std::vector<std::string>& names) {
	std::ostringstream out{};
	for (const auto& str : names) {
		out << str << '\n';
	}
	MessageBoxA(nullptr, out.str().c_str(), "Laliga", MB_OK);
}

class logger {
private:
	std::ofstream outStream{"elements.txt"};
public:
	void log(const std::vector<std::string>& data) {
		for (const auto& element : data) {
			outStream << element << '\n';
		}
	}
};

struct simplePrinter {
	void operator()(const std::vector<std::string>& data)const {
		for (const auto& element : data) {
			std::cout << element << '\n';
		}
	}
};

int main()
//int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{

	AllocConsole();
	freopen("CONOUT$", "w", stdout);

	dynamicArray<std::string> data;
	data.add("RealMadrid");
	data.add("Barcelona");
	data.add("AtleticoMadrid");
	data.add("Espanyol");
	data.add("Valencia");

	logger Logger{};
	auto fnc = std::bind(&logger::log, &Logger, std::placeholders::_1);
	data.setPrinter([&Logger](const auto& data) {
		Logger.log(data);
	});
	data.print();

	//data.setPrinter(winPrinter);
	//data.print();

	//data.setPrinter(fnc);
	//data.setPrinter(simplePrinter{});
	data.setPrinter([](const auto& data) {
		for (const auto& element : data) {
			std::cout << element << '\n';
		}
	});
	data.print();
	return 0;
}
