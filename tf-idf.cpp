#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <cmath>
#include <cctype>
#include <algorithm>
#include <conio.h>

// =CALCULATE TF=
// TF tells us how often a word appears in one document.
// Formula: number of times the word appears / total words.

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


// =CALCULATE IDF=
// IDF tells us how rare or common a word is across all documents.
// A word found in many documents gets a lower IDF.

double calculateIDF(
    const std::vector<std::unordered_map<std::string, int>>& documents,
    const std::string& word)
{
    int totalDocuments = documents.size();
    int documentsContainingWord = 0;

    // Count how many documents contain the word
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


// =CHECK WORD MATCH=
// Allows the user to search using part of a word.
// Example: "cat" can match "cats".

bool startsWith(const std::string& word, const std::string& query)
{
    if (query.length() > word.length())
        return false;

    return word.compare(0, query.length(), query) == 0;
}


// =COUNT TOTAL WORDS=
// Gets the total number of words in one document.
// This is needed when calculating TF.

int countWords(const std::unordered_map<std::string, int>& wordCount)
{
    int total = 0;

    for (const auto& entry : wordCount)
    {
        total += entry.second;
    }

    return total;
}


// =SEARCH RESULT=
// Stores the information needed to display and rank a result.

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
    // =DOCUMENTS=

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

        // Separate each document into individual words
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
                // Convert letters to lowercase
                word += std::tolower(static_cast<unsigned char>(c));
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
        std::cout << "[ CHOOSE 1-3 ]: ";

        char choice;
        std::cin >> choice;


        // =EXIT=

        if (choice == '3')
        {
            system("cls");

            std::cout << "\n";
            std::cout << "====================================================\n";
            std::cout << "                 SEARCH ENGINE CLOSED              \n";
            std::cout << "====================================================\n";

            break;
        }


        // =VIEW DOCUMENTS=

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
                std::cout << "[0] Back     [3] Exit\n";

                char key = _getch();


                // Press 0 to go back to the main menu
                if (key == '0')
                {
                    break;
                }


                // Press 3 to exit
                if (key == '3')
                {
                    system("cls");

                    std::cout << "\n";
                    std::cout << "====================================================\n";
                    std::cout << "                 SEARCH ENGINE CLOSED              \n";
                    std::cout << "====================================================\n";

                    return 0;
                }
            }

            // Return to main menu
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

                std::cout << "[<]  Search: " << query << "\n";
                std::cout << "----------------------------------------------------\n";

                std::vector<SearchResult> results;


                // Check every document for the searched word
                for (int i = 0; i < wordCounts.size(); i++)
                {
                    int totalWords = countWords(wordCounts[i]);

                    double bestTFIDF = 0.0;
                    std::string bestMatchedWord;

                    double bestTF = 0.0;
                    double bestIDF = 0.0;

                    // Separates "not found" from a TF-IDF score of 0
                    bool foundMatch = false;

                    for (const auto& entry : wordCounts[i])
                    {
                        if (!startsWith(entry.first, query))
                            continue;


                        // =TF-IDF CALCULATION=

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


                        // Keep the matching word with the highest TF-IDF
                        if (!foundMatch || tfidf > bestTFIDF)
                        {
                            bestTFIDF = tfidf;
                            bestMatchedWord = entry.first;
                            bestTF = tf;
                            bestIDF = idf;

                            foundMatch = true;
                        }
                    }

                    if (!foundMatch)
                        continue;

                    results.push_back({
                        i,
                        bestMatchedWord,
                        bestTF,
                        bestIDF,
                        bestTFIDF
                    });
                }


                // =RANK RESULTS=
                // Highest TF-IDF is placed first.

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
                    std::cout << "Search Results for: " << query << "\n\n";

                    for (int i = 0; i < results.size(); i++)
                    {
                        const SearchResult& result = results[i];

                        if (i == 0)
                        {
                            std::cout << "MOST RELEVANT RESULT\n";
                        }
                        else
                        {
                            std::cout << "RESULT " << i + 1 << "\n";
                        }

                        std::cout << "\n";
                        std::cout << documents[result.documentIndex] << "\n\n";

                        // Show the values used to rank the result
                        std::cout << "Matched word: "
                                  << result.matchedWord << '\n';

                        std::cout << "TF: " << result.tf << '\n';
                        std::cout << "IDF: " << result.idf << '\n';
                        std::cout << "TF-IDF: "
                                  << result.tfidf << "\n\n";

                        if (i < results.size() - 1)
                        {
                            std::cout
                                << "....................................................\n\n";
                        }
                    }
                }


                // =SEARCH NAVIGATION=

                std::cout << "\n";
                std::cout << "----------------------------------------------------\n";
                std::cout << "[0] Back     [3] Exit\n";


                // =SEARCH INPUT=

                char key = _getch();


                // Press 0 to return to the main menu
                if (key == '0')
                {
                    break;
                }


                // Press 3 to exit
                if (key == '3')
                {
                    system("cls");

                    std::cout << "\n";
                    std::cout << "====================================================\n";
                    std::cout << "                 SEARCH ENGINE CLOSED              \n";
                    std::cout << "====================================================\n";

                    return 0;
                }


                // Backspace removes the last character
                if (key == 8)
                {
                    if (!query.empty())
                        query.pop_back();
                }


                // Letters are added to the search query
                else if (std::isalpha(
                    static_cast<unsigned char>(key)))
                {
                    query += std::tolower(
                        static_cast<unsigned char>(key)
                    );
                }
            }

            // Return to main menu
            continue;
        }


        // =INVALID INPUT=

        std::cout << "\nInvalid choice. Please select 1, 2, or 3.\n";
        std::cout << "Press any key to continue...";

        _getch();
    }

    return 0;
}