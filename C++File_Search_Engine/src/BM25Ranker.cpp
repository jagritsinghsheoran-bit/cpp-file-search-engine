#include "BM25Ranker.h"
#include "InvertedIndex.h"
#include "Tokenizer.h"
#include <cmath>
#include <unordered_set>



BM25Ranker::BM25Ranker(const InvertedIndex& index)
	:m_index{ index }
{

}

double BM25Ranker::avgDocLength()const
{
	return m_index.getAverageDocumentLength();
}
double BM25Ranker::inverseDocumentFrequency(const std::string& term)const
{
	double idf{0.0};
	double n = static_cast<double>(m_index.getDocumentCount());
	double dft = static_cast<double>(m_index.getDocumentFrequency(term));
	if (dft == 0.0)
	{
		return 0.0;
	}
	idf = std::log(1 + ((n - dft + 0.5) / (dft + 0.5)));
	return idf;
}

double BM25Ranker::termFrequencyScore(const std::string& term, std::size_t documentId)const
{
	double tfd{0.0};
	double ftd = static_cast<double>(m_index.getTermFrequency(term, documentId));
	double d = static_cast<double>(m_index.getDocumentLength(documentId));
	double avgdl = avgDocLength();

	if (ftd == 0.0)
	{
		return 0.0;
	}
	else if (avgdl == 0.0)
	{
		return 0.0;
	}
	tfd = (ftd * (k1 + 1)) / (ftd + k1 * (1 - b + b * (d / avgdl)));
	return tfd;


}


double BM25Ranker::scorer(const std::size_t documentID, const std::string& query)const
{
	double totalScore{ 0.0 };
	std::vector<std::string>tokens{ tokenize(query) };
	
	std::unordered_set<std::string>tokenSet{tokens.begin(), tokens.end()};//make sure tokens dont get counted twice(dedup)

	for (const auto& token : tokenSet)
	{
		totalScore += inverseDocumentFrequency(token)*termFrequencyScore(token,documentID);

	}
	return totalScore;
}