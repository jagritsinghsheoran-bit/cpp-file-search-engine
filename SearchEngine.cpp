#include <algorithm>
#include <unordered_set>
#include "Tokenizer.h"
#include "SearchEngine.h"


SearchEngine::SearchEngine(const std::filesystem::path& directoryPath)
	:m_ranker{ m_index }
{
	DocumentScanner scanner;

	 m_documents = scanner.scan(directoryPath);

	for (std::size_t i = 0; i < m_documents.size(); ++i)
	{
		m_index.addDocument(i, m_documents[i].text);
	}

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