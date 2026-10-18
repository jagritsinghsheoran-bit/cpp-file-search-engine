#include <algorithm>
#include <unordered_set>
#include <chrono>
#include <iostream>
#include <thread>
#include <array>
#include "Tokenizer.h"
#include "SearchEngine.h"


SearchEngine::SearchEngine(const std::filesystem::path& directoryPath)
	:m_ranker{ m_index }
{
	//using clock = std::chrono::steady_clock;

	DocumentScanner scanner;

	//const auto scanStart = clock::now();
	m_documents = scanner.scan(directoryPath);
	//const auto scanEnd = clock::now();


	std::array<InvertedIndex, 4>workerIndexes;

	const std::size_t documentCount = m_documents.size();
	const std::size_t midPoint = documentCount / 2;
	const std::size_t quarterPoint1 = midPoint / 2;
	const std::size_t quaretPoint2 = midPoint + quarterPoint1;

	//const auto indexStart = clock::now();
	std::jthread j1([ &midPoint, this, &workerIndexes,&quarterPoint1]() {for (std::size_t i = 0; i < quarterPoint1; i++)
	{
		workerIndexes[0].addDocument(i, m_documents[i].text);
	}});

	std::jthread j2([&documentCount, &midPoint, this, &workerIndexes,&quarterPoint1]() {for (std::size_t i = quarterPoint1; i < midPoint; i++)
	{
		workerIndexes[1].addDocument(i, m_documents[i].text);
	}});

	std::jthread j3([&documentCount, &midPoint, this, &workerIndexes,&quaretPoint2]() {for (std::size_t i = midPoint; i < quaretPoint2; i++)
	{
		workerIndexes[2].addDocument(i, m_documents[i].text);
	}});

	std::jthread j4([&documentCount, &midPoint, this, &workerIndexes,&quaretPoint2]() {for (std::size_t i = quaretPoint2; i < documentCount; i++)
	{
		workerIndexes[3].addDocument(i, m_documents[i].text);
	}});

	



	
	j1.join();
	j2.join();
	j3.join();
	j4.join();

	m_index.mergeFrom(workerIndexes[0]);
	m_index.mergeFrom(workerIndexes[1]);
	m_index.mergeFrom(workerIndexes[2]);
	m_index.mergeFrom(workerIndexes[3]);

	//const auto indexEnd = clock::now();

	//const std::chrono::duration<double> scanTime = scanEnd - scanStart;
	//const std::chrono::duration<double> indexTime = indexEnd - indexStart;
	//std::cout << "Unique terms: " << m_index.getUniqueTermCount() << '\n';
	//std::cout << "Scanning time: " << scanTime.count() << " seconds\n";
	//std::cout << "Index construction time: " << indexTime.count() << " seconds\n";

}

std::vector<SearchResult> SearchEngine::search(const std::string& query)const
{
	std::vector<std::string> testTokens{ tokenize(query) };

	std::unordered_set<std::size_t>docIdSet;

	for (const auto& val : testTokens)
	{
		std::vector<std::size_t>docIdVec = m_index.getDocumentIds(val);
		for (const auto& val : docIdVec)
		{
			docIdSet.insert(val);
		}
	}

	std::vector<std::pair<std::size_t, double>>idScoreVec;//stores id and score 
	for (const auto& val : docIdSet)
	{
		idScoreVec.push_back({ val,m_ranker.scorer(val,query) });
	}

	std::sort(idScoreVec.begin(), idScoreVec.end(), [](const auto& a, const auto& b) { if (a.second != b.second)
		return a.second > b.second;
	return a.first < b.first; });


	std::vector<SearchResult>resultVec;

	for (const auto& val : idScoreVec)
	{
		resultVec.push_back({ val.first,val.second,m_documents[val.first].path });
	}

	return resultVec;
}