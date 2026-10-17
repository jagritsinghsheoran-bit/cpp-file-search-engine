#pragma once
#include "InvertedIndex.h"
#include <string>

class BM25Ranker
{
private:
	const double k1 = 1.2;
	const double b= 0.75;
	const InvertedIndex& m_index;

	double inverseDocumentFrequency(const std::string& term)const;
	double termFrequencyScore(const std::string& term, std::size_t documentId)const;
public:
	BM25Ranker(const InvertedIndex& index);

	double scorer(const std::size_t documentID, const std::string& query)const;

	double avgDocLength()const;
};