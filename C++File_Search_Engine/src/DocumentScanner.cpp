#include "DocumentScanner.h"
#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>

std::vector<Document> DocumentScanner::scan(const std::filesystem::path& directoryPath)const
{
	

	std::vector<std::filesystem::path> paths;

	for (const auto& dir_entry : std::filesystem::directory_iterator{directoryPath })
	{
		if (dir_entry.is_regular_file())
		{
			if (dir_entry.path().extension() == ".txt")
			{
				paths.push_back(dir_entry.path());
			}
		}
	}

	std::sort(paths.begin(), paths.end());

	std::vector<Document>doc;
	for (const auto& val : paths)
	{
		std::ifstream file(val);
		if (!file) {
			std::cerr << "Could not open the file!\n";
		}
		else
		{
			std::stringstream buffer;
			buffer << file.rdbuf(); // Read the entire file buffer into the stream

			std::string file_contents = buffer.str(); // Extract the string

			doc.push_back(Document{ val, file_contents });
		}
		
	}

	return doc;
}