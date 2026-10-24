#include <algorithm>
#include <unordered_set>
#include <chrono>
#include <iostream>
#include <thread>
#include <vector>
#include "Tokenizer.h"
#include "SearchEngine.h"
#include <atomic>
#include <format>
#include <string>




SearchEngine::SearchEngine(const std::filesystem::path& directoryPath)
    : m_ranker{ m_index }
{
    DocumentScanner scanner;
    m_documents = scanner.scan(directoryPath);

       
    const unsigned int hardwareThreads = std::thread::hardware_concurrency();   // 0 if unknown
    const std::size_t kWorkerCount = hardwareThreads > 1 ? hardwareThreads - 1 : 1;//Named kWorkerCount (not compile time)
    const std::size_t documentCount = m_documents.size();
    const std::size_t documentsPerWorker = documentCount / kWorkerCount;

    std::vector<InvertedIndex> workerIndexes(kWorkerCount);
    std::vector<std::jthread> workerThreads;
    workerThreads.reserve(kWorkerCount);

    std::atomic<std::size_t> progress{ 0 };

    for (std::size_t i = 0; i < kWorkerCount; ++i)
    {
        const std::size_t begin = i * documentsPerWorker;
        const std::size_t end= (i == kWorkerCount - 1) ? documentCount : begin + documentsPerWorker;
        workerThreads.emplace_back([this, &workerIndexes, &progress, i, begin, end]()
            {
                for (std::size_t j = begin; j < end; ++j)
                {
                    workerIndexes[i].addDocument(j, m_documents[j].text);
                    progress.fetch_add(1, std::memory_order_relaxed);
                }
            });

    }
    constexpr std::size_t kBarWidth = 30;
    std::cout << std::format("   Using {} worker threads\n", kWorkerCount);
    while (true)
    {
        const std::size_t done = progress.load(std::memory_order_relaxed);
        const std::size_t filled = documentCount ? done * kBarWidth / documentCount : kBarWidth;
        const std::size_t percent = documentCount ? done * 100 / documentCount : 100;

        std::cout << "\r   [" << std::string(filled, '#') << std::string(kBarWidth - filled, '-') << "] "
            << std::format("{:>3}%  {} / {}", percent, done, documentCount) << std::flush;

        if (done >= documentCount)       // checked after drawing, so the 100% frame is always shown
            break;

        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }
    std::cout << '\n';
    for (auto& thread : workerThreads)
        thread.join();

    for (auto& workerIndex : workerIndexes)
        m_index.mergeFrom(workerIndex);
}

std::vector<SearchResult> SearchEngine::search(const std::string& query) const
{
    std::vector<std::string> testTokens{ tokenize(query) };

    std::unordered_set<std::size_t> docIdSet;

    for (const auto& token : testTokens)
    {
        std::vector<std::size_t> docIdVec = m_index.getDocumentIds(token);

        for (const auto& docId : docIdVec)
        {
            docIdSet.insert(docId);
        }
    }

    std::vector<std::pair<std::size_t, double>> idScoreVec;

    for (const auto& docId : docIdSet)
    {
        idScoreVec.push_back({ docId, m_ranker.scorer(docId, query) });
    }

    std::sort(idScoreVec.begin(), idScoreVec.end(),
        [](const auto& a, const auto& b)
        {
            if (a.second != b.second)
                return a.second > b.second;

            return a.first < b.first;
        });

    std::vector<SearchResult> resultVec;

    for (const auto& val : idScoreVec)
    {
        resultVec.push_back(
            { val.first, val.second, m_documents[val.first].path });
    }

    return resultVec;
}


