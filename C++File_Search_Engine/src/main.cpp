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
#include <utility>
using std::cout;
using std::cin;
using std::endl;


#include "Tokenizer.h"
#include "InvertedIndex.h"
#include "DocumentScanner.h"
#include "BM25Ranker.h"

int main()
{
	
	
	
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
	
	std::vector<std::pair<std::size_t, double>>scoreVec;
	BM25Ranker ranker{ index };
	for (const auto& docId : docIdSet)
	{
		double score = ranker.scorer(docId, query);
		scoreVec.push_back({ docId,score });
	}

	std::sort(scoreVec.begin(), scoreVec.end(), [](const auto& a, const auto& b) { if (a.second != b.second)
		return a.second > b.second;
	return a.first < b.first; });

	for (const auto& val : scoreVec)
	{
		cout << "Doc ID : " << val.first << "\n";
		cout << "BM 25 Score : " << val.second << "\n";
		cout << "File name : " << documents[val.first].path.filename() << "\n";
	}

	return 0;
}

