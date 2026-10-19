#include <algorithm>
#include <unordered_set>
#include <chrono>
#include <iostream>
#include <thread>
#include <array>
#include "Tokenizer.h"
#include "SearchEngine.h"




SearchEngine::SearchEngine(const std::filesystem::path& directoryPath)
    : m_ranker{ m_index }
{
    DocumentScanner scanner;
    m_documents = scanner.scan(directoryPath);

    std::array<InvertedIndex, 12> workerIndexes;

    const std::size_t documentCount = m_documents.size();
    const std::size_t documentsPerWorker = documentCount / 12;

    const std::size_t p1 = documentsPerWorker;
    const std::size_t p2 = 2 * documentsPerWorker;
    const std::size_t p3 = 3 * documentsPerWorker;
    const std::size_t p4 = 4 * documentsPerWorker;
    const std::size_t p5 = 5 * documentsPerWorker;
    const std::size_t p6 = 6 * documentsPerWorker;
    const std::size_t p7 = 7 * documentsPerWorker;
    const std::size_t p8 = 8 * documentsPerWorker;
    const std::size_t p9 = 9 * documentsPerWorker;
    const std::size_t p10 = 10 * documentsPerWorker;
    const std::size_t p11 = 11 * documentsPerWorker;

    std::jthread j1([this, &workerIndexes, &p1]()
        {
            for (std::size_t i = 0; i < p1; ++i)
                workerIndexes[0].addDocument(i, m_documents[i].text);
        });

    std::jthread j2([this, &workerIndexes, &p1, &p2]()
        {
            for (std::size_t i = p1; i < p2; ++i)
                workerIndexes[1].addDocument(i, m_documents[i].text);
        });

    std::jthread j3([this, &workerIndexes, &p2, &p3]()
        {
            for (std::size_t i = p2; i < p3; ++i)
                workerIndexes[2].addDocument(i, m_documents[i].text);
        });

    std::jthread j4([this, &workerIndexes, &p3, &p4]()
        {
            for (std::size_t i = p3; i < p4; ++i)
                workerIndexes[3].addDocument(i, m_documents[i].text);
        });

    std::jthread j5([this, &workerIndexes, &p4, &p5]()
        {
            for (std::size_t i = p4; i < p5; ++i)
                workerIndexes[4].addDocument(i, m_documents[i].text);
        });

    std::jthread j6([this, &workerIndexes, &p5, &p6]()
        {
            for (std::size_t i = p5; i < p6; ++i)
                workerIndexes[5].addDocument(i, m_documents[i].text);
        });

    std::jthread j7([this, &workerIndexes, &p6, &p7]()
        {
            for (std::size_t i = p6; i < p7; ++i)
                workerIndexes[6].addDocument(i, m_documents[i].text);
        });

    std::jthread j8([this, &workerIndexes, &p7, &p8]()
        {
            for (std::size_t i = p7; i < p8; ++i)
                workerIndexes[7].addDocument(i, m_documents[i].text);
        });

    std::jthread j9([this, &workerIndexes, &p8, &p9]()
        {
            for (std::size_t i = p8; i < p9; ++i)
                workerIndexes[8].addDocument(i, m_documents[i].text);
        });

    std::jthread j10([this, &workerIndexes, &p9, &p10]()
        {
            for (std::size_t i = p9; i < p10; ++i)
                workerIndexes[9].addDocument(i, m_documents[i].text);
        });

    std::jthread j11([this, &workerIndexes, &p10, &p11]()
        {
            for (std::size_t i = p10; i < p11; ++i)
                workerIndexes[10].addDocument(i, m_documents[i].text);
        });

    std::jthread j12([this, &workerIndexes, &p11, &documentCount]()
        {
            for (std::size_t i = p11; i < documentCount; ++i)
                workerIndexes[11].addDocument(i, m_documents[i].text);
        });

    j1.join();
    j2.join();
    j3.join();
    j4.join();
    j5.join();
    j6.join();
    j7.join();
    j8.join();
    j9.join();
    j10.join();
    j11.join();
    j12.join();

    for ( auto& workerIndex : workerIndexes)
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