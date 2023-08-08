#include "TextParser.h"
#include <algorithm>

void TextParser::Parse(const std::vector<char> chunk)
{
    std::string word;
    bool endOfWord = false;
    for (auto it = chunk.begin(); it != chunk.end(); ++it) {
        if(std::isalpha(*it)) {
            word.push_back(*it);
            endOfWord = true;
        }
        else if (endOfWord) {
            SetWordCount(std::move(word));
            word.clear();
            endOfWord = false;
        }
    }

    if (endOfWord) {
        SetWordCount(word);
    }
}

uint64_t TextParser::GetWordsCount()
{
    return m_wordsToCount.size();
}

void TextParser::SetWordCount(std::string word)
{
    std::lock_guard<std::mutex> lock(this->m_writeMutex);
    m_wordsToCount.insert(std::move(word));
}
