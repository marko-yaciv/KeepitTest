#include "TextParser.h"
#include <algorithm>
#include <fstream>
void TextParser::Parse(const std::vector<char> chunk)
{
    std::string word;
    std::unordered_set<std::string> localWords;
    bool endOfWord = false;
    for (auto it = chunk.begin(); it != chunk.end(); ++it) 
    {
        if(std::isalpha(*it)) 
        {
            word.push_back(*it);
            endOfWord = true;
        }
        else if (endOfWord) 
        {
            localWords.insert(std::move(word));
            word.clear();
            endOfWord = false;
        }
    }

    if (endOfWord) 
    {
        localWords.insert(std::move(word));
    }

    UpdateWordCount(std::move(localWords));
}

uint64_t TextParser::GetWordsCount()
{
    return m_wordsToCount.size();
}

void TextParser::UpdateWordCount(std::unordered_set<std::string>&& localWords)
{
    std::lock_guard<std::mutex> lock(this->m_writeMutex);
    m_wordsToCount.merge(std::move(localWords));
}
