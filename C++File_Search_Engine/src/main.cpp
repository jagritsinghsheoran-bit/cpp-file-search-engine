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
#include <unordered_set>
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
	InvertedIndex index;

	std::vector<Document> documents = scanner.scan(directoryPath);

	for (std::size_t i=0; i < documents.size(); ++i)
	{
		index.addDocument(i, documents[i].text);
	}

	std::string query = "cars electric";
	std::vector<std::string> testTokens{ tokenize(query) };

	std::unordered_set<std::size_t>docIdSet;

	for (const auto& val : testTokens)
	{
		std::vector<std::size_t>docIdVec = index.getDocumentIds(val);
		for (const auto& val : docIdVec)
		{
			docIdSet.insert(val);
		}
	}
	for (const auto& docId : docIdSet)
	{
		std::cout << "ID: " << docId << '\n';
		std::cout << "File: "
			<< documents[docId].path.filename().string()
			<< "\n\n";
	}
	return 0;
}

