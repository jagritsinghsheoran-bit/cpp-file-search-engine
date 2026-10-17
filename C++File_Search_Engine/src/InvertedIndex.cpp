#include "InvertedIndex.h"
#include "Tokenizer.h"



bool InvertedIndex::addDocument(std::size_t documentId, const std::string& text)
{
	auto it = m_documentLengths.find(documentId);
	if (it != m_documentLengths.end())
	{
		return false;
	}
	std::vector<std::string>tokens= tokenize(text) ;

	m_documentLengths[documentId] = tokens.size();

	for (const auto& token : tokens)
	{
		m_postings[token][documentId]++;
	}
	return true;
}

std::size_t InvertedIndex::getTermFrequency(const std::string& term,std::size_t documentId)const
{
	auto it = m_postings.find(term);
	if (it != m_postings.end())
	{
		auto documentIt = it->second.find(documentId);
		if (documentIt != it->second.end())
		{
			return documentIt->second;
		}
		else
		{
			return 0;
		}

	}
	else
	{
		return 0;
	}
}

std::size_t InvertedIndex::getDocumentFrequency(const std::string& term)const
{
	auto it = m_postings.find(term);
	if (it != m_postings.end())
	{
		return it->second.size();
	}
	else
	{
		return 0;
	}
}

std::size_t InvertedIndex::getDocumentLength(std::size_t documentId)const
{
	auto it = m_documentLengths.find(documentId);
	if (it != m_documentLengths.end())
	{
		return it->second;
	}
	else
	{
		
		return 0;
	}
}