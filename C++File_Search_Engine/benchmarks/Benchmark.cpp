#include <chrono>
#include <filesystem>
#include <iostream>
#include <cstdint>
#include <iomanip>
#include <utility>

#include "SearchEngine.h"
/*
std::pair<std::size_t, std::uintmax_t> getCorpusStats(const std::filesystem::path& directoryPath)
{
    std::size_t documentCount = 0;
    std::uintmax_t totalBytes = 0;

    for (const auto& entry :
        std::filesystem::directory_iterator(directoryPath))
    {
        if (entry.is_regular_file() &&
            entry.path().extension() == ".txt")
        {
            ++documentCount;
            totalBytes += entry.file_size();
        }
    }

    return { documentCount, totalBytes };
}

int main()
{
    const std::filesystem::path directoryPath =
        R"(C:\SearchEngineBenchmarkCorpus)";
    const auto [documentCount, totalBytes] = getCorpusStats(directoryPath);

    const auto start = std::chrono::steady_clock::now();

    SearchEngine engine{ directoryPath };

    const auto end = std::chrono::steady_clock::now();

    const std::chrono::duration<double> elapsed = end - start;
    const double seconds = elapsed.count();
    const double mib = static_cast<double>(totalBytes) / (1024.0 * 1024.0);

    std::cout << std::fixed << std::setprecision(4);
    std::cout << "Documents: " << documentCount << '\n';
    std::cout << "Corpus size: " << mib << " MiB\n";
    std::cout << "Documents/sec: " << documentCount / seconds << '\n';
    std::cout << "MiB/sec: " << mib / seconds << '\n';
    std::cout << "Indexing time: "<< elapsed.count() << " seconds\n";
}
*/
