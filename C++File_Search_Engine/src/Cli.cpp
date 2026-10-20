#include "Cli.h"
#include <string>
#include <vector>
#include <algorithm>
namespace
{
    // Counts .txt files and their total size; returns {0, 0} if the folder can't be read
    std::pair<std::size_t, std::uintmax_t> getCorpusStats(const std::filesystem::path& directoryPath)
    {
        std::size_t documentCount = 0;
        std::uintmax_t totalBytes = 0;

        std::error_code ec;
        for (const auto& entry : std::filesystem::directory_iterator(directoryPath, ec))
        {
            if (entry.is_regular_file(ec) && entry.path().extension() == ".txt")
            {
                ++documentCount;
                totalBytes += entry.file_size(ec);
            }
        }

        return { documentCount, totalBytes };
    }
}
Cli::Cli(std::istream& in, std::ostream& out)
	:m_in{in},m_out{out}
{

}

void Cli::printBanner()const
{
	constexpr std::string_view kBanner = R"BANNER(
  ____                      _       _____             _
 / ___|  ___  __ _ _ __ ___| |__   | ____|_ __   __ _(_)_ __   ___
 \___ \ / _ \/ _` | '__/ __| '_ \  |  _| | '_ \ / _` | | '_ \ / _ \
  ___) |  __/ (_| | | | (__| | | | | |___| | | | (_| | | | | |  __/
 |____/ \___|\__,_|_|  \___|_| |_| |_____|_| |_|\__, |_|_| |_|\___|
                                                |___/

   Inverted index  *  BM25 ranking  *  12-thread indexing  *  C++20
)BANNER";
	m_out << kBanner;
}

void Cli::printMenu()const
{
	constexpr std::string_view kMenu = R"MENU(
   +----------------------------------+
   |  [1]  Search documents           |
   |  [2]  Index a folder             |
   |  [3]  Show index statistics      |
   |  [4]  Exit                       |
   +----------------------------------+
)MENU";
    m_out << kMenu << "\n   search-engine> " << std::flush;
}


std::optional<int>Cli::readChoice()
{
    std::string line;
    if (!std::getline(m_in, line))          
    {
        m_out << "\n   Input closed. Exiting.\n";
        return std::nullopt;
    }

    const auto first = line.find_first_not_of(" \t");//function to trim spaces at start and at end.
    const auto last = line.find_last_not_of(" \t");

    line = (first == std::string::npos) ? "" : line.substr(first, last - first + 1);

    if (line == "1") return 1;
    else if (line == "2") return 2;
    else if (line == "3") return 3;
    else if (line == "4") return 4;

    m_out << "   \"" << line << "\" is not an option. Choose 1, 2, 3 or 4.\n";
    return 0;
}


int Cli::run()
{
    printBanner();

    while (true)
    {
        printMenu();              
        const std::optional<int> choice = readChoice();
        if (!choice)                       // input ended
            return 0;

        

        switch (*choice)
        {
        case(1):
            searchDocuments();
            break;
        case(2):
            indexDocuments();
            break;
        case(3):
            showStats();
            break;
        case 4: m_out << "   Goodbye.\n"; return 0;
       
        default: break;
        }
    }

}


void Cli::searchDocuments()
{
    if (!m_engine)
    {
        m_out<< "   No index loaded - choose [2] first.\n";
        return;
    }
    m_out << "\n   Enter a query (empty line returns to the menu)\n";
    std::string query;
    while (true)
    {
        m_out << "   query> " << std::flush;

        if (!std::getline(m_in, query) || query.empty())//error or empty query return
        {
            return;
        }

        const std::vector<SearchResult> results = m_engine->search(query);

        if (results.empty())
        {
            m_out << "   No results found.\n";
            continue;
        }

        const std::size_t shown = std::min<std::size_t>(results.size(), 10);
        m_out << '\n';
        for (std::size_t i = 0; i < shown; ++i)
            m_out << std::format("   {:>2}.  {:<24}{:>8.4f}\n",
                i + 1, results[i].filePath.filename().string(), results[i].bm25Score);

        if (results.size() > shown)
            m_out << "       ...and " << (results.size() - shown) << " more\n";
        m_out << '\n';
    }
}

void Cli::indexDocuments()
{
    constexpr std::string_view kFolderPrompt = "\n   Folder to index: ";
    constexpr std::string_view kFolderPromptLast = "\n   Folder to index [{}]: ";
    constexpr std::string_view kNoFolderGiven = "   No folder entered. Returning to the menu.\n";
    constexpr std::string_view kNotAFolder = "   Not a folder: {}\n";
    constexpr std::string_view kReindexConfirm = "   An index of {} is already loaded. Re-index? (y/n): ";
    constexpr std::string_view kReindexCancelled = "   Kept the current index.\n";
    constexpr std::string_view kIndexing = "   Indexing {} ...\n";
    constexpr std::string_view kIndexDone = "   Indexed {} documents in {:.2f} s.\n";
    constexpr std::string_view kIndexEmpty = "   No .txt files found in {}.\n";
    constexpr std::string_view kIndexFailed = "   Indexing failed: {}\n";

    if (m_lastFolder.empty())
    {
        m_out << kFolderPrompt << std::flush;
    }
    else
    {
        m_out << std::format(kFolderPromptLast, m_lastFolder.string()) << std::flush;
    }

    std::string input;
    if (!std::getline(m_in, input))
    {
        return;
    }
                                                       
    std::filesystem::path folder;

    if (!input.empty())
    {
        folder = input;                                       
    }
    else if (!m_lastFolder.empty())
    {
        folder = m_lastFolder;                                 
    }
    else
    {
        m_out << kNoFolderGiven;                               
        return;
    }


    // 2. Make sure it is a real folder
    std::error_code ec;
    if (!std::filesystem::is_directory(folder, ec))
    {
        m_out << std::format(kNotAFolder, folder.string());
        return;
    }

    if (m_engine && folder == m_lastFolder)
    {
        m_out << std::format(kReindexConfirm, folder.string()) << std::flush;

        std::string answer;
        if (!std::getline(m_in, answer))
            return;

        const auto first = answer.find_first_not_of(" \t");//function to trim spaces at start and at end.
        const auto last = answer.find_last_not_of(" \t");

        answer = (first == std::string::npos) ? "" : answer.substr(first, last - first + 1);
        if (answer != "y" && answer != "Y")
        {
            m_out << kReindexCancelled;
            return;
        }
    }

    
    m_out << std::format(kIndexing, folder.string()) << std::flush;

    try
    {
        const auto start = std::chrono::steady_clock::now();
        m_engine.emplace(folder);
        const std::chrono::duration<double> elapsed = std::chrono::steady_clock::now() - start;

        m_lastFolder = folder;
        m_lastIndexSeconds = elapsed.count();

        if (m_engine->getIndex().getDocumentCount() == 0)
            m_out << std::format(kIndexEmpty, folder.string());
        else
            m_out << std::format(kIndexDone, m_engine->getIndex().getDocumentCount(), elapsed.count());
    }
    catch (const std::exception& e)
    {
        m_engine.reset();
        m_out << std::format(kIndexFailed, e.what());
    }

}



void Cli::showStats() const
{
    if (!m_engine)
    {
        m_out << "   No index loaded - choose [2] first.\n";
        return;
    }

    const InvertedIndex& index = m_engine->getIndex();
    const std::size_t documents = index.getDocumentCount();
    const double averageLength = index.getAverageDocumentLength();

    const auto [fileCount, totalBytes] = getCorpusStats(m_lastFolder);
    const double mib = static_cast<double>(totalBytes) / (1024.0 * 1024.0);
    const double seconds = m_lastIndexSeconds;

    m_out << "\n   Index statistics\n"
        << "   ----------------------------------------\n"
        << std::format("   Folder            {}\n", m_lastFolder.string())
        << std::format("   Documents         {}\n", documents)
        << std::format("   Corpus size       {:.2f} MiB\n", mib)
        << std::format("   Unique terms      {}\n", index.getUniqueTermCount())
        << std::format("   Total tokens      {:.0f}\n", averageLength * static_cast<double>(documents))
        << std::format("   Avg doc length    {:.1f} tokens\n", averageLength)
        << "   ----------------------------------------\n"
        << std::format("   Indexing time     {:.2f} s\n", seconds);

    if (seconds > 0.0)
    {
        m_out << std::format("   Documents/sec     {:.0f}\n", static_cast<double>(documents) / seconds)
            << std::format("   MiB/sec           {:.2f}\n", mib / seconds);
    }

    m_out << "   ----------------------------------------\n";

    if (fileCount != documents)
        m_out << std::format("   Note: the folder now has {} .txt files; choose [2] to re-index.\n", fileCount);
}