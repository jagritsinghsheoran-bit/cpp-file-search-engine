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
#include <fstream>
using std::cout;
using std::cin;
using std::endl;


#include "Tokenizer.h"
#include "InvertedIndex.h"
#include "DocumentScanner.h"

int main()
{
	
	/*
	InvertedIndex index;

	std::cout << index.addDocument(0, "cat dog cat") << '\n';
	std::cout << index.addDocument(0, "bird fish") << '\n';

	std::cout << index.getTermFrequency("cat", 0) << '\n';
	std::cout << index.getTermFrequency("bird", 0) << '\n';
	std::cout << index.getDocumentLength(0) << '\n';
	*/

	std::filesystem::path directoryPath= R"(C:\Users\jagri\OneDrive\Desktop\SearchEngineFile)";
	
	DocumentScanner scanner;
	std::vector<Document> documents = scanner.scan(directoryPath);

	InvertedIndex index;

	for (std::size_t i=0; i < documents.size(); ++i)
	{
		index.addDocument(i, documents[i].text);
	}
	

	std::cout << index.getTermFrequency("cars", 0) << '\n';
	std::cout << index.getDocumentLength(0) << '\n';
	std::cout << index.getTermFrequency("spaceship", 0) << '\n';
	std::cout << index.getDocumentFrequency("cars") << '\n';

	return 0;
}

