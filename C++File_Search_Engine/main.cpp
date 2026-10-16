#include <iostream>//(always do this basicaly c++ library)(iostream=input output stream)
#include <string>//do this to inlcude std::string and its counterparts
#include <string_view>
#include <cmath>
#include <chrono>
#include <random>
#include <limits>
#include <cassert>
#include <vector>
#include <array>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <filesystem>
using std::cout;
using std::cin;
using std::endl;


int main()
{
	std::filesystem::path directoryPath = R"(C:\Users\jagri\OneDrive\Desktop\SearchEngineFile)";

	std::filesystem::path filepath = directoryPath / "flowers.txt";

	for (const auto& dir_entry : std::filesystem::directory_iterator{ directoryPath })
	{
		if (dir_entry.is_regular_file())
		{
			if (dir_entry.path().extension() == ".txt")
			{
				cout << dir_entry.path() << "\n";
			}
		}
		else
		{
			cout << "Error";
		}
	}
	return 0;
}