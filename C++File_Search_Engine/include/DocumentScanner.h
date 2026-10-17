#pragma once
#include <filesystem>
#include <string>
#include <vector>
struct Document
{
	std::filesystem::path path;
	std::string text;
};

class DocumentScanner
{
public:
	std::vector<Document> scan(const std::filesystem::path& directoryPath)const ;
};