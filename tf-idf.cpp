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
int main(){

    std::vector<std::string> documents =
    {
        "cat dog cat",
        "dog mouse dog",
        "cat mouse"
    };
    std::vector<std::unordered_map<std::string, int>> wordCounts;

    for (const std::string& document : documents)
    {
        std::unordered_map<std::string, int> counts;
        std::string word;

        for (char c : document)
        {
            if (c == ' ')
            {
                if (!word.empty())
                {
                    counts[word]++;
                    word.clear();
                }
            }
            else
            {
                word += c;
            }
        }

        if (!word.empty())
        {
            counts[word]++;
        }

        wordCounts.push_back(counts);
    }

    //Calculate TF-IDF for two words
    std::string word1 = "cat";
    std::string word2 = "dog";

    int totalWords = 3;

    //Word 1
    double tf1 = calculateTF(
        wordCounts[0],
        word1,
        totalWords
    );

    double idf1 = calculateIDF(
        wordCounts,
        word1
    );

    double tfidf1 = tf1 * idf1;

    std::cout << "Word: " << word1 << '\n';
    std::cout << "TF: " << tf1 << '\n';
    std::cout << "IDF: " << idf1 << '\n';
    std::cout << "TF-IDF: " << tfidf1 << "\n\n";

    //Word 2
    double tf2 = calculateTF(
        wordCounts[0],
        word2,
        totalWords
    );

    double idf2 = calculateIDF(
        wordCounts,
        word2
    );

    double tfidf2 = tf2 * idf2;

    std::cout << "Word: " << word2 << '\n';
    std::cout << "TF: " << tf2 << '\n';
    std::cout << "IDF: " << idf2 << '\n';
    std::cout << "TF-IDF: " << tfidf2 << "\n\n";

    //Comparison
    std::cout << "Comparison:\n";

    if (tfidf1 > tfidf2)
    {
        std::cout << word1 << " has the higher TF-IDF.\n";
    }
    else if (tfidf2 > tfidf1)
    {
        std::cout << word2 << " has the higher TF-IDF.\n";
    }
    else
    {
        std::cout << "Both words have the same TF-IDF.\n";
    }
}

//future improvements: 
//show documents first
//show word-frequency table
//show document frequency
//show actual formulas
//add common word
//maybe let user choose document