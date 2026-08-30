#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <cmath>
#include <cctype>
#include <algorithm>

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

    return std::log10(static_cast<double>(totalDocuments) / documentsContainingWord);
}

bool startsWith(const std::string& word, const std::string& query)
{
    if (query.length() > word.length())
        return false;
    return word.compare(0, query.length(), query) == 0;
}

int countWords(const std::unordered_map<std::string, int>& wordCount){
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
            word += std::tolower(static_cast<unsigned char>(c));
        }
    }
    if (!word.empty())
        counts[word]++;

    wordCounts.push_back(counts);
}

// Calculate TF-IDF for two words
// Calculate TF-IDF for two words

std::string word1 = "cats";
std::string word2 = "dogs";

int totalWords1 = countWords(wordCounts[0]);
int totalWords2 = countWords(wordCounts[2]);

double tf1 = calculateTF(wordCounts[0], word1, totalWords1);
double tf2 = calculateTF(wordCounts[2], word2, totalWords2);

double idf1 = calculateIDF(wordCounts, word1);
double idf2 = calculateIDF(wordCounts, word2);

double tfidf1 = tf1 * idf1;
double tfidf2 = tf2 * idf2;

std::cout << "Word: " << word1 << '\n';
std::cout << "TF: " << tf1 << '\n';
std::cout << "IDF: " << idf1 << '\n';
std::cout << "TF-IDF: " << tfidf1 << "\n\n";

std::cout << "Word: " << word2 << '\n';
std::cout << "TF: " << tf2 << '\n';
std::cout << "IDF: " << idf2 << '\n';
std::cout << "TF-IDF: " << tfidf2 << "\n\n";

std::cout << "Comparison:\n";
if (tfidf1 > tfidf2)
    std::cout << word1 << " has the higher TF-IDF.\n";
else if (tfidf2 > tfidf1)
    std::cout << word2 << " has the higher TF-IDF.\n";
else
    std::cout << "Both words have the same TF-IDF.\n";

std::string query;

std::cout << "Search: ";
std::cin >> query;

for (auto& c : query)
    c = std::tolower(static_cast<unsigned char>(c));

std::vector<SearchResult> results;

for (int i = 0; i < wordCounts.size(); i++)
{
    bool found = false;
    std::string matchedWord;

    for (const auto& entry : wordCounts[i])
    {
        if (startsWith(entry.first, query))
        {
            found = true;
            matchedWord = entry.first;
            break;
        }
    }

    if (!found)
        continue;

    int totalWords = countWords(wordCounts[i]);

    double tf = calculateTF(
        wordCounts[i],
        matchedWord,
        totalWords
    );

    double idf = calculateIDF(
        wordCounts,
        matchedWord
    );

    double tfidf = tf * idf;

    results.push_back({
    i,
    matchedWord,
    tfidf
    });

    std::cout << "\nDocument " << i + 1 << "\n";
    std::cout << "Matched word: " << matchedWord << "\n";
    std::cout << "TF-IDF: " << tfidf << "\n";
    std::cout << documents[i] << "\n"; 

std::sort(
    results.begin(),
    results.end(),
    [](const SearchResult& a, const SearchResult& b)
    {
        return a.tfidf > b.tfidf;
    }
);

if (results.empty())
{
    std::cout << "No matching documents found.\n";
}
else
{
    std::cout << "\nSearch Results:\n";

    for (const SearchResult& result : results)
    {
        std::cout << "\nDocument "
                  << result.documentIndex + 1 << "\n";

        std::cout << "Matched word: "
                  << result.matchedWord << "\n";

        std::cout << "TF-IDF: "
                  << result.tfidf << "\n";

        std::cout << documents[result.documentIndex]
                  << "\n";
    }

    std::cout << "\nMost relevant result:\n";
    std::cout << documents[results[0].documentIndex]
              << "\n";
}


return 0;
}