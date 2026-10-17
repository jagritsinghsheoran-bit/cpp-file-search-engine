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
#include "BM25Ranker.h"

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
	/*
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
	*/

	InvertedIndex index;

	index.addDocument(0, "spaceship");
	index.addDocument(1, "spaceship spaceship spaceship");
	index.addDocument(2, "car bus train plane");

	BM25Ranker ranker{ index };

	std::cout << ranker.scorer(0, "spaceship") << '\n';
	std::cout << ranker.scorer(1, "spaceship") << '\n';
	std::cout << ranker.scorer(2, "spaceship") << '\n';
	return 0;
}

