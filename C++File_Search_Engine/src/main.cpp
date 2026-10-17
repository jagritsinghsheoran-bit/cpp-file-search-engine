#include <iostream>
#include <string>
#include <vector>
#include <filesystem>



using std::cout;
using std::cin;
using std::endl;


#include "SearchEngine.h"

void runQueries(const SearchEngine& engine,const std::vector<std::string>& queries)
{
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

}

int main()
{
	std::filesystem::path directoryPath= R"(C:\Users\jagri\OneDrive\Desktop\SearchEngineFile)";
	SearchEngine engine{ directoryPath };

    std::vector<std::string> queries = {};
    std::string query="";
    cout << "Enter queries (type 'exit' to finish):\n";

    while (std::getline(std::cin, query))
    {
        if (query == "exit")
            break;

        queries.push_back(query);
    }

    runQueries(engine, queries);
    

	return 0;
}

