#include <iostream>//(always do this basicaly c++ library)(iostream=input output stream)
#include <string>//do this to inlcude std::string and its counterparts
#include <string_view>
#include <cmath>
#include <chrono>
#include <random>
#include <limits>
#include <cassert>
#include <vector>
#include <array>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <filesystem>
#include <fstream>
#include <unordered_set>
#include <utility>
using std::cout;
using std::cin;
using std::endl;


#include "Tokenizer.h"
#include "InvertedIndex.h"
#include "DocumentScanner.h"
#include "BM25Ranker.h"
#include "SearchEngine.h"

int main()
{
	std::filesystem::path directoryPath= R"(C:\Users\jagri\OneDrive\Desktop\SearchEngineFile)";
	SearchEngine engine{ directoryPath };

    std::vector<std::string> queries = {
    "flowers pollinators",
    "keyboard switches",
    "search index",
    "the",
    "spaceshipxyz",
    ""
    };

    for (const auto& query : queries)
    {
        std::cout << "\nQuery: \"" << query << "\"\n";

        std::vector<SearchResult> results = engine.search(query);

        if (results.empty())
        {
            std::cout << "No results found.\n";
            continue;
        }

        for (const auto& result : results)
        {
            std::cout << result;
        }
    }

	return 0;
}

