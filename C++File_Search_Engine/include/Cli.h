#pragma once

#include <iostream>
#include <optional>
#include "SearchEngine.h"
#include "InvertedIndex.h"


class Cli
{
private:
	std::istream& m_in;
	std::ostream& m_out;

	
	std::optional<SearchEngine>m_engine;
	std::filesystem::path m_lastFolder;
	double m_lastIndexSeconds{ 0.0 };


	void searchDocuments();//[1]
	void indexDocuments();//[2]
	void showStats()const;//[3]

	void printBanner()const;
	void printMenu()const;
	std::optional<int>readChoice();

public:
	Cli(std::istream& in, std::ostream& out);
	int run();
};