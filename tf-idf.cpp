#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <cmath>

double calculateTF(
    const std::unordered_map<std::string, int>& wordCount,
    const std::string& word,
    int totalWords
)
{
    auto it = wordCount.find(word);
    if (it == wordCount.end())
        return 0.0;

    return static_cast<double>(it->second) / totalWords;
}

double calculateIDF(
    const std::vector<std::unordered_map<std::string, int>>& documents,
    const std::string& word
)
{
    int totalDocuments = documents.size();
    int documentsContainingWord = 0;

    for (const auto& document : documents)
    {
        if (document.find(word) != document.end())
        {
            documentsContainingWord++;
        }
    }

    if (documentsContainingWord == 0)
        return 0.0;

    return std::log10(
        static_cast<double>(totalDocuments) / documentsContainingWord
    );
}