#include <iostream>
#include <string>
#include <vector>
#include <filesystem>
#include <cassert>

#include "SearchEngine.h"

using std::cout;
using std::cin;
using std::string;

void runQueries(const SearchEngine& engine,
    const std::vector<string>& queries)
{
    for (const auto& query : queries)
    {
        cout << "\nQuery: \"" << query << "\"\n";

        const std::vector<SearchResult> results = engine.search(query);

        if (results.empty())
        {
            cout << "No results found.\n";
            continue;
        }

        for (const auto& result : results)
            cout << result;
    }
}

int main()
{
    const std::filesystem::path directoryPath =
        R"(C:\Users\jagri\OneDrive\Desktop\SearchEngineFile)";

    SearchEngine engine{ directoryPath };

    std::vector<string> queries;
    string query;

    cout << "Enter queries (type 'exit' to finish):\n";

    while (std::getline(cin, query))
    {
        if (query == "exit")
            break;

        queries.push_back(query);
    }

    runQueries(engine, queries);

    return 0;
}