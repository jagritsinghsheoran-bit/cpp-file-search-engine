#pragma once
#include <cstddef>
#include <filesystem>
#include <string>
#include <vector>
#include <ostream>

#include "InvertedIndex.h"
#include "BM25Ranker.h"
#include "DocumentScanner.h"



struct SearchResult
{
	std::size_t documentId;
	double bm25Score;
	std::filesystem::path filePath;

	friend std::ostream& operator<<(std::ostream& os, const SearchResult& result)
	{
		return os << "File: " << result.filePath.filename().string()
			<< "\nDocument ID: " << result.documentId
			<< "\nBM25 Score: " << result.bm25Score
			<< "\n-------------------------\n";
	}
};
class SearchEngine
{
private:
	InvertedIndex m_index;
	std::vector<Document> m_documents;
	BM25Ranker m_ranker;
public:
	SearchEngine(const std::filesystem::path& directoryPath);
	std::vector<SearchResult> search(const std::string& query)const;
	const InvertedIndex& getIndex() const { return m_index; }
};