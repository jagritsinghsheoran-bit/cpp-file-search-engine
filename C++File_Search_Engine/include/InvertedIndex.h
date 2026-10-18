#pragma once
#include <string>
#include <unordered_map>
#include <vector>
#include <cstddef>

class InvertedIndex
{
private:

	std::unordered_map<std::string, std::unordered_map<std::size_t, std::size_t>>m_postings;//For every term, store the document IDs containing it and the number of occurrences in each document.

	std::unordered_map<std::size_t, std::size_t>m_documentLengths;//document id and no of tokens in it

public:
	double getTotalTokenizeSeconds() const;
	double getTotalPostingInsertionSeconds() const;

	std::size_t getUniqueTermCount() const;
	
	bool addDocument(std::size_t documentId,const std::string& text);

	std::size_t getTermFrequency(const std::string& term,std::size_t documentId)const ;//How many times a term appears in a document

	std::size_t getDocumentFrequency(const std::string& term)const;//How many documents contain a term

	std::size_t getDocumentLength(std::size_t documentId)const;

	std::vector<std::size_t> getDocumentIds(const std::string& token)const;//returns ID of all documents containing that term

	std::size_t getDocumentCount()const;



	void mergeFrom(const InvertedIndex& other);
};

