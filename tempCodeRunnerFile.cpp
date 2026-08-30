#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <cmath>
#include <cctype>
#include <algorithm>
#include <conio.h>

double calculateTF(
    const std::unordered_map<std::string, int>& wordCount,
    const std::string& word,
    int totalWords)
{
    auto it = wordCount.find(word);
    if (it == wordCount.end())
        return 0.0;

    return static_cast<double>(it->second) / totalWords;
}

double calculateIDF(
    const std::vector<std::unordered_map<std::string, int>>& documents,
    const std::string& word)
{
    int totalDocuments = documents.size();
    int documentsContainingWord = 0;

    for (const auto& document : documents)
    {
        if (document.find(word) != document.end())
            documentsContainingWord++;
    }

    if (documentsContainingWord == 0)
        return 0.0;

    return std::log10(
        static_cast<double>(totalDocuments) / documentsContainingWord
    );
}

bool startsWith(const std::string& word, const std::string& query)
{
    if (query.length() > word.length())
        return false;

    return word.compare(0, query.length(), query) == 0;
}

int countWords(const std::unordered_map<std::string, int>& wordCount)
{
    int total = 0;

    for (const auto& entry : wordCount)
    {
        total += entry.second;
    }

    return total;
}

struct SearchResult
{
    int documentIndex;
    std::string matchedWord;
    double tf;
    double idf;
    double tfidf;
};

int main()
{
    std::vector<std::string> documents =
    {
        "Cats are domesticated mammals that are often kept as pets. "
        "They are known for their independence and hunting abilities.",

        "Bananas are edible fruits produced by several kinds of large "
        "herbaceous flowering plants. They are commonly eaten as snacks.",

        "Dogs are domesticated mammals and are commonly kept as pets. "
        "They are known for their loyalty and social behavior."
    };

    // =WORD COUNTS=

    std::vector<std::unordered_map<std::string, int>> wordCounts;

    for (const std::string& document : documents)
    {
        std::unordered_map<std::string, int> counts;
        std::string word;

        for (char c : document)
        {
            if (c == ' ' || std::ispunct(static_cast<unsigned char>(c)))
            {
                if (!word.empty())
                {
                    counts[word]++;
                    word.clear();
                }
            }
            else
            {
                word += std::tolower(
                    static_cast<unsigned char>(c)
                );
            }
        }

        if (!word.empty())
            counts[word]++;

        wordCounts.push_back(counts);
    }

    // =MAIN MENU=

    while (true)
    {
        system("cls");

        std::cout << "\n";
        std::cout << "====================================================\n";
        std::cout << "                 MINI SEARCH ENGINE                 \n";
        std::cout << "====================================================\n\n";

        std::cout << "Type a number in the search bar:\n\n";
        std::cout << "  1   View all documents\n";
        std::cout << "  2   Search\n";
        std::cout << "  3   Exit\n\n";

        std::cout << "----------------------------------------------------\n";
        std::cout << "[ Search ]: ";

        char choice;
        std::cin >> choice;

        // =EXIT=

        if (choice == '3' || choice == 'x' || choice == 'X')
        {
            system("cls");

            std::cout << "\n";
            std::cout << "====================================================\n";
            std::cout << "                 SEARCH ENGINE CLOSED              \n";
            std::cout << "====================================================\n";

            break;
        }

        // =VIEW ALL DOCUMENTS=

        if (choice == '1')
        {
            while (true)
            {
                system("cls");

                std::cout << "\n";
                std::cout << "====================================================\n";
                std::cout << "                   ALL DOCUMENTS                   \n";
                std::cout << "====================================================\n\n";

                for (int i = 0; i < documents.size(); i++)
                {
                    std::cout << "Document " << i + 1 << "\n";
                    std::cout << documents[i] << "\n\n";
                }

                std::cout << "----------------------------------------------------\n";
                std::cout << "[B] Back to Main Menu     [X] Exit\n";

                char key = _getch();

                if (key == 'b' || key == 'B')
                    break;

                if (key == 'x' || key == 'X')
                {
                    system("cls");

                    std::cout << "\n";
                    std::cout << "====================================================\n";
                    std::cout << "                 SEARCH ENGINE CLOSED              \n";
                    std::cout << "====================================================\n";

                    return 0;
                }
            }

            continue;
        }

        // =SEARCH=

        if (choice == '2')
        {
            std::string query;

            while (true)
            {
                system("cls");

                std::cout << "\n";
                std::cout << "====================================================\n";
                std::cout << "                  MINI SEARCH ENGINE               \n";
                std::cout << "====================================================\n\n";

                std::cout << "[<]  Search: "
                          << query
                          << "                                      [X]\n";

                std::cout << "----------------------------------------------------\n";

                std::vector<SearchResult> results;

                for (int i = 0; i < wordCounts.size(); i++)
                {
                    int totalWords = countWords(wordCounts[i]);

                    double bestTFIDF = 0.0;
                    std::string bestMatchedWord;

                    double bestTF = 0.0;
                    double bestIDF = 0.0;

                    for (const auto& entry : wordCounts[i])
                    {
                        if (!startsWith(entry.first, query))
                            continue;

                        double tf = calculateTF(
                            wordCounts[i],
                            entry.first,
                            totalWords
                        );

                        double idf = calculateIDF(
                            wordCounts,
                            entry.first
                        );

                        double tfidf = tf * idf;

                        if (tfidf > bestTFIDF)
                        {
                            bestTFIDF = tfidf;
                            bestMatchedWord = entry.first;
                            bestTF = tf;
                            bestIDF = idf;
                        }
                    }

                    if (bestMatchedWord.empty())
                        continue;

                    results.push_back({
                        i,
                        bestMatchedWord,
                        bestTF,
                        bestIDF,
                        bestTFIDF
                    });
                }

                std::sort(
                    results.begin(),
                    results.end(),
                    [](const SearchResult& a, const SearchResult& b)
                    {
                        return a.tfidf > b.tfidf;
                    }
                );

                // =SEARCH RESULTS=

                if (query.empty())
                {
                    std::cout << "\n";
                    std::cout << "Type something to search...\n";
                }
                else if (results.empty())
                {
                    std::cout << "\n";
                    std::cout << "No matching documents found.\n";
                }
                else
                {
                    std::cout << "\n";
                    std::cout << "Search Results for: "
                              << query
                              << "\n\n";

                    for (int i = 0; i < results.size(); i++)
                    {
                        const SearchResult& result = results[i];

                        if (i == 0)
                        {
                            std::cout << "MOST RELEVANT RESULT\n";
                        }
                        else
                        {
                            std::cout << "RESULT "
                                      << i + 1
                                      << "\n";
                        }

                        std::cout << "\n";

                        std::cout << documents[
                            result.documentIndex
                        ] << "\n\n";

                        std::cout << "Matched word: "
                                  << result.matchedWord
                                  << '\n';

                        std::cout << "TF: "
                                  << result.tf
                                  << '\n';

                        std::cout << "IDF: "
                                  << result.idf
                                  << '\n';

                        std::cout << "TF-IDF: "
                                  << result.tfidf
                                  << "\n\n";

                        if (i < results.size() - 1)
                        {
                            std::cout << "....................................................\n\n";
                        }
                    }
                }

                std::cout << "\n";
                std::cout << "----------------------------------------------------\n";
                std::cout << "[B] Back to Main Menu     [X] Exit\n";

                // =SEARCH INPUT=

                char key = _getch();

                if (key == 'b' || key == 'B')
                {
                    break;
                }

                if (key == 'x' || key == 'X')
                {
                    system("cls");

                    std::cout << "\n";
                    std::cout << "====================================================\n";
                    std::cout << "                 SEARCH ENGINE CLOSED              \n";
                    std::cout << "====================================================\n";

                    return 0;
                }

                if (key == 8)
                {
                    if (!query.empty())
                        query.pop_back();
                }
                else if (
                    std::isalpha(
                        static_cast<unsigned char>(key)))
                {
                    query += std::tolower(
                        static_cast<unsigned char>(key)
                    );
                }
            }

            continue;
        }

        // =INVALID INPUT=

        std::cout << "\nInvalid choice. Please select 1, 2, or 3.\n";
        std::cout << "Press any key to continue...";

        _getch();
    }

    return 0;
}